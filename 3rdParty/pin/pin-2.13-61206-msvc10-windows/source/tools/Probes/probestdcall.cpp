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

/*! @file
 *  This is an example of probe mode function replacement of an stdcall function
 */

#include "pin.H"

#include <stdio.h>
#include "probe_stdcall_fastcall.h"





typedef void (__stdcall* StdCallFunctionToBeReplacedByPinType)(char c1, 
                                                               int num1,
                                                               char c2,
                                                               int num2);

// the calling convention of ReplacementFunc should be the default calling convention.
void /*__stdcall*/ ReplacementFunc(StdCallFunctionToBeReplacedByPinType originalFunc, 
                                 char c1, 
                                 int num1,
                                 char c2,
                                 int num2)


{  
    
    printf ( "Inside Probe ReplacementFunc\n");
    if (c1 != CHAR_VAL1)
    {
        printf ("ERROR in ReplacementFunc: c1 is %c and not as expected\n", c1);
    }
    if (c2 != CHAR_VAL2)
    {
        printf ("ERROR in ReplacementFunc: c2 is %c and not as expected\n", c2);
    }
    if (num1 != INT_VAL1)
    {
        printf ("ERROR in ReplacementFunc: num1 is %d and not as expected\n", num1);
    }
    if (num2 != INT_VAL2)
    {
        printf ("ERROR in ReplacementFunc: num2 is %d and not as expected\n", num2);
    }
    originalFunc( c1+CHAR_ADD_VAL1, num1+INT_ADD_VAL1, c2+CHAR_ADD_VAL2, num2+INT_ADD_VAL2 );
}


BOOL foundFunc = false;
void DoReplacementFunc( IMG img, char * funcName)
{
    RTN rtnToReplace;
    printf ("Image %s\n", IMG_Name(img).c_str());

    for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
    {     
        for (RTN rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn))
        {
           printf ("  Rtn: %s  %s\n", RTN_Name(rtn).c_str(), funcName);
           if (strstr( RTN_Name(rtn).c_str(), funcName))
           {
               //printf ("    found\n");
               foundFunc = true;
               rtnToReplace = rtn;
               break;
           }
        }
        if (foundFunc)
        {
            break;
        }
    }
    if (!foundFunc)
    {
        return;
    }

    printf ( "Found %s %x\n", funcName, RTN_Address(rtnToReplace));
    // commented out so that absence of pdb file will not cause failure
    if (RTN_IsSafeForProbedReplacement(rtnToReplace))
    {  
        printf ( "RTN_ReplaceSignatureProbed on %s\n", funcName);
        foundFunc = true;
        
        PROTO protoOfStdCallFunction1ToBeReplacedByPin 
            = PROTO_Allocate( PIN_PARG(void *), 
                              CALLINGSTD_STDCALL,
                              "protoOfStdCallFunction1ToBeReplacedByPin", 
                              PIN_PARG(char), 
                              PIN_PARG(int),
                              PIN_PARG(char), 
                              PIN_PARG(int),
                              PIN_PARG_END() );
        
        RTN_ReplaceSignatureProbed(rtnToReplace, AFUNPTR(ReplacementFunc),
            IARG_PROTOTYPE, protoOfStdCallFunction1ToBeReplacedByPin,
            IARG_ORIG_FUNCPTR,
            IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
            IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
            IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
            IARG_FUNCARG_ENTRYPOINT_VALUE, 3,
            IARG_END);
        PROTO_Free( protoOfStdCallFunction1ToBeReplacedByPin );
    }    
}


/*
 * process_loaded_image: Called every time when new image is loaded.
 */
static VOID process_loaded_image(IMG image, VOID *value)
{
    if ( !IMG_Valid(image) || foundFunc)
        return;
    
    DoReplacementFunc( image, "StdCallFunctionToBeReplacedByPin");
    
}



int main(int argc, char **argv)
{
    PIN_InitSymbols();  
    if ( PIN_Init(argc, argv) )
        return -1;

    IMG_AddInstrumentFunction(process_loaded_image, 0); 
    PIN_StartProbedProgram();
}

