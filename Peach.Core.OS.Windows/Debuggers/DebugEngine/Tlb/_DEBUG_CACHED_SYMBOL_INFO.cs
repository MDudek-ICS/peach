namespace Peach.Core.Debuggers.DebugEngine.Tlb
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential, Pack=8)]
    public struct _DEBUG_CACHED_SYMBOL_INFO
    {
        public ulong ModBase;
        public ulong Arg1;
        public ulong Arg2;
        public uint Id;
        public uint Arg3;
    }
}

