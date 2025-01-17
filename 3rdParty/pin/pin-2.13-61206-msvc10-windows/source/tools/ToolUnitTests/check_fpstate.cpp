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

#include "pin.H"
#include <iostream>
#include <fstream>

/* ===================================================================== */
/* Names of malloc and free */
/* ===================================================================== */
#if defined(TARGET_MAC)
#define FN "_trap_me"
#else
#define FN "trap_me"
#endif

/* ===================================================================== */
/* Global Variables */
/* ===================================================================== */

std::ofstream out;

/* ===================================================================== */
/* Commandline Switches */
/* ===================================================================== */

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "check_fpstate.out", "specify output file name");

/* ===================================================================== */

static CONTEXT saveCtxt;
static BOOL enable = FALSE;


/* ===================================================================== */
/* Analysis routines                                                     */
/* ===================================================================== */
 
VOID CompreContext(CONTEXT *c1, CONTEXT *c2)
{
    struct {
        REG reg;
        const char *name;
    } fields[] = {
        { REG_FPIP_OFF, "ipoff" },
        { REG_FPIP_SEL, "ipsel" },
        { REG_FPOPCODE, "opcod" },
        { REG_FPDP_OFF, "dpoff" },
        { REG_FPDP_SEL, "dpsel" }
    };

    for (unsigned int i = 0; i < sizeof(fields)/sizeof(fields[0]); i++)
    {
        REGVAL regval;
        UINT64 v1, v2;

        PIN_GetContextRegval(c1, fields[i].reg, &regval);
        PIN_ReadRegvalQWord(&regval, &v1, 0);

        PIN_GetContextRegval(c2, fields[i].reg, &regval);
        PIN_ReadRegvalQWord(&regval, &v2, 0);

        if (v1 == v2) 
            out << "Same ";
        else
            out << "Mismatch ";
        out << fields[i].name << " " << v1 << " " << v2 << endl;
    }
}

VOID Enable()
{
    enable = TRUE;
}

VOID SaveContext(CONTEXT *ctxt)
{
    if(!enable)
        return;

    PIN_SaveContext(ctxt, &saveCtxt);
}

VOID SyscallEntry(THREADID tid, CONTEXT *ctxt, SYSCALL_STANDARD std, VOID *v)
{
    if (!enable)
        return;

    CompreContext(ctxt, &saveCtxt);

    enable = FALSE;
}

/* ===================================================================== */
/* Instrumentation routines                                              */
/* ===================================================================== */
   
VOID Image(IMG img, VOID *v)
{
    // Instrument the trapme function to activate the analysis.
    RTN rtn = RTN_FindByName(img, FN);
    if (RTN_Valid(rtn))
    {
        RTN_Open(rtn);

        RTN_InsertCall(rtn, IPOINT_BEFORE, (AFUNPTR)Enable, IARG_END);

        RTN_Close(rtn);
    }
}

VOID Trace(TRACE trace, VOID *v)
{
    for (BBL bbl=TRACE_BblHead(trace); BBL_Valid(bbl); bbl=BBL_Next(bbl))
    {
        for (INS ins=BBL_InsHead(bbl); INS_Valid(ins); ins=INS_Next(ins))
        {
            if (INS_IsSyscall(ins))
            {
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)SaveContext,
                               IARG_CONTEXT, IARG_END);
            }
        }
    }
}

/* ===================================================================== */

VOID Fini(INT32 code, VOID *v)
{
    out.close();
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */
   
INT32 Usage()
{
    cerr << "This tool produces a trace of calls to malloc." << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char *argv[])
{
    // Initialize pin & symbol manager
    PIN_InitSymbols();
    if( PIN_Init(argc,argv) )
    {
        return Usage();
    }
    
    // Write to a file since cout and cerr maybe closed by the application
    out.open(KnobOutputFile.Value().c_str());
    out << hex;
    out.setf(ios::showbase);
    
    IMG_AddInstrumentFunction(Image, 0);
    TRACE_AddInstrumentFunction(Trace, 0);
    PIN_AddSyscallEntryFunction(SyscallEntry, 0);
    PIN_AddFiniFunction(Fini, 0);

    // Never returns
    PIN_StartProgram();
    
    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
