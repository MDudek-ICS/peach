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
using System.Linq;
using System.Text;
using System.IO;
using Peach.Core.IO;

namespace Peach.Core.Proxy.Web
{
    public abstract class HttpMessage
    {
		byte[] _body = null;
		List<byte[]> _chunks = null;

		public Connection Connection { get; set; }
        public string StartLine { get; set; }
        public byte[] Body
		{
			get
			{
				if(_body == null && _chunks != null)
				{
					var writer = new StreamWriter(new MemoryStream());

					// Build chunks body
					foreach (byte[] chunk in _chunks)
					{
						writer.Write(Convert.ToString(chunk.Length, 16) + "\r\n");
						writer.BaseStream.Write(chunk, 0, chunk.Length);
						writer.Write("\r\n");
					}

					writer.Write("0\r\n\r\n");

					return ((MemoryStream)writer.BaseStream).ToArray();
				}

				return _body;
			}

			set
			{
				_body = value;
			}
		}

        public HttpHeaderCollection Headers { get; set; }
		public List<byte[]> Chunks
		{
			get
			{
				return _chunks;
			}

			set
			{
				_chunks = value;
			}
		}

		protected static string ReadLine(BitStream data)
		{
			int currentPosition = (int)data.Position;

			try
			{
				List<byte> lineData = new List<byte>();

				while (true)
				{
					int value = data.ReadByte();
					if (value == -1)
						break;

					byte b1 = (byte)value;

					value = data.ReadByte();
					if (value == -1)
						break;

					byte b2 = (byte)value;

					if (b1 == '\r' && b2 == '\n')
					{
						lineData.Add(b1);
						lineData.Add(b2);

						return ASCIIEncoding.ASCII.GetString(lineData.ToArray());
					}
					else
					{
						lineData.Add(b1);
						data.Seek(-1, SeekOrigin.Current);
					}
				}
			}
			catch
			{
			}

			data.Seek(currentPosition, SeekOrigin.Begin);
			return null;
		}
    }
}
