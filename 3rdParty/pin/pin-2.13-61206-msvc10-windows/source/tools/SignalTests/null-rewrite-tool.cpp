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
/*
 * This tool rewrites every memory instruction using Pin's INS_RewriteMemoryOperand() API,
 * but all instructions are rewritten to use their original effective addresses.
 */

#include <iostream>
#include <pin.H>


static void InstrumentIns(INS, VOID *);
static REG GetScratchReg(UINT32);
static ADDRINT GetMemAddress(ADDRINT);


int main(int argc, char * argv[])
{
    PIN_Init(argc, argv);
    INS_AddInstrumentFunction(InstrumentIns, 0);
    PIN_StartProgram();
    return 0;
}

static void InstrumentIns(INS ins, VOID *)
{
    for (UINT32 memIndex = 0;  memIndex < INS_MemoryOperandCount(ins);  memIndex++)
    {
        REG scratchReg = GetScratchReg(memIndex);
        INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(GetMemAddress),
            IARG_MEMORYOP_EA, memIndex,
            IARG_RETURN_REGS, scratchReg,
            IARG_END);
        INS_RewriteMemoryOperand(ins, memIndex, scratchReg); 
    }
}

static REG GetScratchReg(UINT32 index)
{
    static std::vector<REG> regs;

    while (index >= regs.size())
    {
        REG reg = PIN_ClaimToolRegister();
        if (reg == REG_INVALID())
        {
            std::cerr << "*** Ran out of tool registers" << std::endl;
            PIN_ExitProcess(1);
            /* does not return */
        }
        regs.push_back(reg);
    }

    return regs[index];
}

static ADDRINT GetMemAddress(ADDRINT ea)
{
    return ea;
}
