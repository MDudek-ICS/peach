/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2013 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
/// @file xed-category-enum.h

// This file was automatically generated.
// Do not edit this file.

#if !defined(_XED_CATEGORY_ENUM_H_)
# define _XED_CATEGORY_ENUM_H_
#include "xed-common-hdrs.h"
typedef enum {
  XED_CATEGORY_INVALID,
  XED_CATEGORY_3DNOW,
  XED_CATEGORY_AES,
  XED_CATEGORY_AVX,
  XED_CATEGORY_AVX2,
  XED_CATEGORY_AVX2GATHER,
  XED_CATEGORY_BDW,
  XED_CATEGORY_BINARY,
  XED_CATEGORY_BITBYTE,
  XED_CATEGORY_BMI1,
  XED_CATEGORY_BMI2,
  XED_CATEGORY_BROADCAST,
  XED_CATEGORY_CALL,
  XED_CATEGORY_CMOV,
  XED_CATEGORY_COND_BR,
  XED_CATEGORY_CONVERT,
  XED_CATEGORY_DATAXFER,
  XED_CATEGORY_DECIMAL,
  XED_CATEGORY_FCMOV,
  XED_CATEGORY_FLAGOP,
  XED_CATEGORY_FMA4,
  XED_CATEGORY_INTERRUPT,
  XED_CATEGORY_IO,
  XED_CATEGORY_IOSTRINGOP,
  XED_CATEGORY_LOGICAL,
  XED_CATEGORY_LZCNT,
  XED_CATEGORY_MISC,
  XED_CATEGORY_MMX,
  XED_CATEGORY_NOP,
  XED_CATEGORY_PCLMULQDQ,
  XED_CATEGORY_POP,
  XED_CATEGORY_PREFETCH,
  XED_CATEGORY_PUSH,
  XED_CATEGORY_RDRAND,
  XED_CATEGORY_RDSEED,
  XED_CATEGORY_RDWRFSGS,
  XED_CATEGORY_RET,
  XED_CATEGORY_ROTATE,
  XED_CATEGORY_SEGOP,
  XED_CATEGORY_SEMAPHORE,
  XED_CATEGORY_SHIFT,
  XED_CATEGORY_SMAP,
  XED_CATEGORY_SSE,
  XED_CATEGORY_STRINGOP,
  XED_CATEGORY_STTNI,
  XED_CATEGORY_SYSCALL,
  XED_CATEGORY_SYSRET,
  XED_CATEGORY_SYSTEM,
  XED_CATEGORY_TBM,
  XED_CATEGORY_UNCOND_BR,
  XED_CATEGORY_VFMA,
  XED_CATEGORY_VTX,
  XED_CATEGORY_WIDENOP,
  XED_CATEGORY_X87_ALU,
  XED_CATEGORY_XOP,
  XED_CATEGORY_XSAVE,
  XED_CATEGORY_XSAVEOPT,
  XED_CATEGORY_LAST
} xed_category_enum_t;

/// This converts strings to #xed_category_enum_t types.
/// @param s A C-string.
/// @return #xed_category_enum_t
/// @ingroup ENUM
XED_DLL_EXPORT xed_category_enum_t str2xed_category_enum_t(const char* s);
/// This converts strings to #xed_category_enum_t types.
/// @param p An enumeration element of type xed_category_enum_t.
/// @return string
/// @ingroup ENUM
XED_DLL_EXPORT const char* xed_category_enum_t2str(const xed_category_enum_t p);

/// Returns the last element of the enumeration
/// @return xed_category_enum_t The last element of the enumeration.
/// @ingroup ENUM
XED_DLL_EXPORT xed_category_enum_t xed_category_enum_t_last(void);
#endif
