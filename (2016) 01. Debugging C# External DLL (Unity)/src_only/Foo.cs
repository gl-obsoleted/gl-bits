using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace test_stacktrace_dll
{
    public class Foo
    {
        public static int Bar { get { return 42; } }

        public static string GetStacktraceInDLL()
        {
            return Environment.StackTrace;
        }

        int _foo = 12;
        public static int CrashManually()
        {
            Foo a = null;
            return a._foo;
        }
    }
}
