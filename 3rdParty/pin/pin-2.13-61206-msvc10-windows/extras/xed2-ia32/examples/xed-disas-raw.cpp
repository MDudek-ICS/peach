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
/// @file disas-raw.cpp


////////////////////////////////////////////////////////////////////////////

extern "C" {
#include "xed-interface.h"
#include "xed-portability.h"
#include "xed-examples-util.h"
}

////////////////////////////////////////////////////////////////////////////



void
xed_disas_raw(xed_decode_file_info_t* fi)
{
    printf("In raw...\n");
    void* region = 0;
    unsigned int len = 0;
    xed_map_region(fi->input_file_name, &region, &len);

    xed_disas_info_t di;
    di.s =  (unsigned char*)region;
    di.a = (unsigned char*)region;
    di.q = (unsigned char*)(region) + len; // end of region
    di.dstate = &fi->dstate;
    di.ninst = fi->ninst;
    di.runtime_vaddr = 0;
    di.runtime_vaddr_disas_start = 0;
    di.runtime_vaddr_disas_end = 0;
    di.decode_only = fi->decode_only;
    di.symfn = 0;
    di.caller_symbol_data = 0;
    di.input_file_name = fi->input_file_name;
    di.line_number_info_fn = 0;
    xed_copy_info(&di, fi);
    xed_disas_test(&di);
    if (fi->xml_format == 0)
        xed_print_decode_stats();
}
 
////////////////////////////////////////////////////////////////////////////
