/************************************************************************************//**
* \file         port/WINDOWS/tbxport.c
* \brief        Port specifics source file.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2019 by Feaser     www.feaser.com     All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* \endinternal
****************************************************************************************/

/****************************************************************************************
* Include files
****************************************************************************************/
#include "microtbx.h"                            /* MicroTBX global header             */
#include <windows.h>                             /* Windows specific functions.        */


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Flag to determine if the critical section object was already initialized. */
static volatile uint8_t criticalSectionInitialized = TBX_FALSE;

/** \brief Critical section object. */
static CRITICAL_SECTION criticalSection;


/************************************************************************************//**
** \brief     Stores the current state of the CPU status register and then disables the
**            generation of global interrupts. The status register contains information
**            about the interrupts being disable/enabled before they get disabled. This
**            is needed to later on restore the state. Note that this function should
**            always be used pair wise together with TbxPortInterruptsRestore(). So
**            a call to TbxPortInterruptsDisable() should always be followed with a call
**            to TbxPortInterruptsRestore() eventually.
**            Prototype: 
**              tTbxPortCpuSR TbxPortInterruptsDisable(void);
** \return    The current value of the CPU status register.
**
****************************************************************************************/
tTbxPortCpuSR TbxPortInterruptsDisable(void)
{
  tTbxPortCpuSR result;

  /* Initialize the result. Note that this value is don't care for this port, as under
   * Windows, the already available CriticalSection API is used.
   */
  result = 0;
  
  /* Make sure the critical section object is initialized. */
  if (criticalSectionInitialized == TBX_FALSE)
  {
    /* Initialize the critical section object. */
    InitializeCriticalSection((CRITICAL_SECTION *)&criticalSection);
    /* Set initialized flag. */
    criticalSectionInitialized = TBX_TRUE;
  }

  /* Enter the critical section. */
  EnterCriticalSection((CRITICAL_SECTION *)&criticalSection);
  
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxPortInterruptsDisable ***/


/************************************************************************************//**
** \brief     Restores the interrupts enabled/disabled state to the state it was when
**            function TbxPortInterruptsDisable() was previously called. It does this
**            by writing the value of the CPU status register that was returned by
**            TbxPortInterruptsDisable(). Note that this function should always be used
**            pair wise together with TbxPortInterruptsDisable(). So a call to
**            TbxPortInterruptsDisable() should always be followed with a call to
**            TbxPortInterruptsRestore() eventually.
**            Prototype: 
**              void TbxPortInterruptsRestore(tTbxPortCpuSR prev_cpu_sr);
** \param     prev_cpu_sr The previous value of the CPU status register from right before
**            the interrupts where disabled. This value is returned by function
**            TbxPortInterruptsDisable().
**
****************************************************************************************/
void TbxPortInterruptsRestore(tTbxPortCpuSR prev_cpu_sr)
{
  /* The parameter is not used for this port, because under Windows, the already
   * available CriticalSection API is used. 
   */
  TBX_UNUSED_ARG(prev_cpu_sr);

  /* Make sure the critical section object was initialized. */
  TBX_ASSERT(criticalSectionInitialized == TBX_TRUE);

  /* Leave the critical section. */
  LeaveCriticalSection((CRITICAL_SECTION *)&criticalSection);
} /*** end of TbxPortInterruptsRestore ***/


/*********************************** end of tbxport.c **********************************/
