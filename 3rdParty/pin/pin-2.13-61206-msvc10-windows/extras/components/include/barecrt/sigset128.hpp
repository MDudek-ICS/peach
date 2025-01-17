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
// <ORIGINAL-AUTHOR>: Ady Tal
// <COMPONENT>: barecrt
// <FILE-TYPE>: component public header

#ifndef BARECRT_SIGSET128_HPP
#define BARECRT_SIGSET128_HPP

#include "fund.hpp"

/*
 * These macros were copied from the FreeBSD header files
 */
#define BARECRT_SIG_WORDS  4
#define BARECRT_SIG_MAXSIG 128
#define BARECRT_SIG_IDX(sig)   ((sig) - 1)
#define BARECRT_SIG_WORD(sig)  (BARECRT_SIG_IDX(sig) >> 5)
#define BARECRT_SIG_BIT(sig)   (1 << (BARECRT_SIG_IDX(sig) & 31))

namespace BARECRT {


/*!
 * Represents a set of signals in the format expected by the kernel.
 * NOTE: This is not necessarily the same as a libc sigset_t!
 */
struct /*<POD>*/ SIGSET
{
public:
    // This is declared public to make this a POD.  Don't access directly!
    //
    FUND::UINT32 _set[BARECRT_SIG_WORDS];

public:
    /*!
     * Initialize the set to contain no signals.
     */
    void Empty()
    {
        for (FUND::UINT32 i = 0; i < BARECRT_SIG_WORDS; i++)
            _set[i] = 0;
    }

    /*!
     * Initialize the set to contain all signals.
     */
    void Fill()
    {
        for (FUND::UINT32 i = 0; i < BARECRT_SIG_WORDS; i++)
            _set[i] = ~0U;
    }

    /*!
     * Add a signal to the set.
     *
     *  @param[in] signal   The signal.
     */
    void Add(int signal)
    {
        _set[BARECRT_SIG_WORD(signal)] |= BARECRT_SIG_BIT(signal);
    }

    /*!
     * Remove a signal from the set.
     *
     *  @param[in] signal   The signal.
     */
    void Remove(int signal)
    {
        _set[BARECRT_SIG_WORD(signal)] &= ~BARECRT_SIG_BIT(signal);
    }

    /*!
     * Tells if a signal is a member of a set.
     *
     *  @param[in] signal   The signal.
     *
     * @return  TRUE if \a signal is in the set.
     */
    bool IsMember(int signal) const
    {
        return (_set[BARECRT_SIG_WORD(signal)] & BARECRT_SIG_BIT(signal)) != 0;
    }

    /*!
     * Add all signals from \a other to this set.
     *
     *  @param[in] other    The signal set.
     */
    void AddSet(const SIGSET *other)
    {
        for (FUND::UINT32 i = 0; i < BARECRT_SIG_WORDS; i++)
            _set[i] |= other->_set[i];
    }

    /*!
     * Remove all signals from \a other from this set.
     *
     *  @param[in] other    The signal set.
     */
    void RemoveSet(const SIGSET *other)
    {
        for (FUND::UINT32 i = 0; i < BARECRT_SIG_WORDS; i++)
            _set[i] &= ~(other->_set[i]);
    }

    /*!
     * Return one word of the mask (representing 32 signals).
     *
     *  @param[in] i    Tells which word to return: 0 reprsents signals 1-32,
     *                   1 represents signals 33-64, etc.  The lowest signal
     *                   number corresponds to the least significant bit.
     *
     * @return  The mask word.
     */
    FUND::UINT32 GetMaskWord(unsigned i) const
    {
        switch (i)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            return _set[i];

        default:
            return 0;
        }
    }

    /*!
     * Set one word of the mast (representing 32 signals).
     *
     *  @param[in] i        Tells which word to set: 0 corresponds to signals 1-32,
     *                       1 to signals 33-64, etc.
     *  @param[in] word     The mask word to set.  The lowest bit represents
     *                       the smallest signal.
     */
    void SetMaskWord(unsigned i, FUND::UINT32 word)
    {
        switch (i)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            _set[i] = word;
            break;
        }
    }
};


} // namespace
#endif // file guard
