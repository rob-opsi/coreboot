/**
 * @file
 *
 * mnflowdr.c
 *
 * Deerhound initializer for MCT and DCT
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Main)
 * @e \$Revision: 7081 $ @e \$Date: 2008-07-31 01:47:27 -0500 (Thu, 31 Jul 2008) $
 *
 **/
/*****************************************************************************
*
* Copyright (c) 2011, Advanced Micro Devices, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Advanced Micro Devices, Inc. nor the names of
*       its contributors may be used to endorse or promote products derived
*       from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* ***************************************************************************
*
*/

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */



#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "OptionMemory.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mndr.h"
#include "Filecode.h"
#define FILECODE PROC_MEM_NB_DR_MNFLOWDR_FILECODE
/* features */
#include "mftds.h"
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
extern MEM_PLAT_SPEC_CFG* memPlatSpecFFInstalledDR[MAX_FF_TYPES];

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function initializes the platform specific block
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE - AGESA_SUCCESS at least one dorm factor was found
 *     @return          FALSE - AGESA_UNSUPPORTED - Error indicating that no form factors were found
 */

BOOLEAN
MemNPlatformSpecificFormFactorInitDr (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;
  UINT8 f;
  UINT8 ErrUnSuppFFCount;
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_DR; Dct++) {
    NBPtr->SwitchDCT (NBPtr, Dct);
    if (NBPtr->ChannelPtr->ChDimmValid != 0) {
      ErrUnSuppFFCount = 0;
      for (f = 0; f < MAX_FF_TYPES; f++) {
        ASSERT (memPlatSpecFFInstalledDR[f] != NULL);
        if (memPlatSpecFFInstalledDR[f] (NBPtr->MemPtr, NBPtr->ChannelPtr, NBPtr->PsPtr) == AGESA_UNSUPPORTED) {
          ErrUnSuppFFCount++; //Count the number of AGESA_UNSUPPORTED errors
        } else {
          break;
        }
      }
      if (ErrUnSuppFFCount == MAX_FF_TYPES) {
        return FALSE; // No FF types are supported
      }
    }
  }
  return TRUE;
}
/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */



