using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Reflection;

public class Main : MonoBehaviour {

    void PrintStacktraceOrdinary()
    {
        Debug.LogFormat("Stacktrace (ordinary): \n{0}", Environment.StackTrace);
    }

    void PrintStacktraceInsideUserDLL()
    {
        // put `test_stacktrace_dll.dll` in `Assets` folder and uncomment this line
        //Debug.LogFormat("Stacktrace (inside user dll): \n{0}", test_stacktrace_dll.Foo.GetStacktraceInDLL());
    }

    Type _dynType = null;

    void PrintStacktraceDynamically()
    {
        string localAssmlyPath = "Assets/test_stacktrace_dll.bin";
        byte[] src = File.ReadAllBytes(localAssmlyPath);

        string localSymbolPath = "Assets/test_stacktrace_dll.bin.mdb";
        byte[] symbolBytes = File.ReadAllBytes(localSymbolPath);

        Assembly assembly = Assembly.Load(src, symbolBytes);
        Type type = assembly.GetType("test_stacktrace_dll.Foo");

        MethodInfo getStacktrace = type.GetMethod("GetStacktraceInDLL", BindingFlags.Public | BindingFlags.Static);
        Debug.LogFormat("Stacktrace (dynamically): \n{0}", getStacktrace.Invoke(null, null));

        _dynType = type;
    }

    // Use this for initialization
	void Start () 
    {
        PrintStacktraceOrdinary();

        PrintStacktraceInsideUserDLL();

        PrintStacktraceDynamically();

        // produce a crash manually
        if (_dynType != null)
        {
            MethodInfo fn = _dynType.GetMethod("CrashManually", BindingFlags.Public | BindingFlags.Static);
            if (fn != null)
                fn.Invoke(null, null);
        }
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
