﻿
//
// Copyright (c) Michael Eddington
//
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
// copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in	
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

// Authors:
//   Mikhail Davidov (sirus@haxsys.net)

// $Id$

using System;
using System.Collections.Generic;
using System.Text;
using Peach.Core.Dom;
using Peach.Core.IO;

namespace Peach.Core.Transformers.Encode
{
    [Description("Encode on output from a string representation of a SID to bytes. (Format: S-1-5-21-2127521184-1604012920-1887927527-1712781)")]
    [Transformer("SidStringToBytes", true)]
    [Transformer("encode.SidStringToBytes")]
    [Serializable]
    public class SidStringToBytes : Transformer
    {
        public SidStringToBytes(Dictionary<string,Variant> args) : base(args)
        {
        }

        protected override BitwiseStream internalEncode(BitwiseStream data)
        {
            var sids = new BitReader(data).ReadString();

            try
            {
                //Hopefully this is in mono...
                var sid = new System.Security.Principal.SecurityIdentifier(sids);
                byte[] bsid = new byte[sid.BinaryLength];
                sid.GetBinaryForm(bsid, 0);

                var ret = new BitStream();
                ret.Write(bsid, 0, bsid.Length);
                ret.Seek(0, System.IO.SeekOrigin.Begin);
                return ret;
            }
            catch(Exception ex)
            {
                throw new SoftException("Error, cannot convert string '" + sids + "' to SID.", ex);
            }
        }

        protected override BitStream internalDecode(BitStream data)
        {
            var len = data.Length;
            var buf = new BitReader(data).ReadBytes((int)len);
            var sid = new System.Security.Principal.SecurityIdentifier(buf, 0);
            var ret = new BitStream();
            var writer = new BitWriter(ret);
            writer.WriteString(sid.ToString());
            ret.Seek(0, System.IO.SeekOrigin.Begin);
            return ret;
        }
    }
}

// end
