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
//   Michael Eddington (mike@dejavusecurity.com)

// $Id$

using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Compression;
using System.IO;
using Peach.Core.Dom;
using Peach.Core.IO;

namespace Peach.Core.Transformers
{
	[Description("Evaluate a statement.")]
	[Transformer("Eval", true)]
	[Transformer("misc.Eval")]
	[Parameter("eval", typeof(string), "Formatter for data.")]
	[Serializable]
	public class Eval : Transformer
	{
		//Dictionary<string, Variant> m_args;

		public Eval(Dictionary<string, Variant> args)
			: base(args)
		{
			//m_args = args;
		}

		protected override BitwiseStream internalEncode(BitwiseStream data)
		{
			//string format;
			//if (m_args.ContainsKey("eval"))
			//    format = (string)(m_args["eval"]);

			return data;
		}

		protected override BitStream internalDecode(BitStream data)
		{
			throw new NotImplementedException();
		}
	}
}

// end
