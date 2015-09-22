using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System;

public class UPerfQuery 
{
#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport("__Internal")]
#else
    [DllImport("mono")]
#endif
    static extern long mono_gc_get_used_size();  //Amount Used

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport("__Internal")]
#else
    [DllImport("mono")]
#endif
    static extern long mono_gc_get_heap_size(); //Total Amount

    public static long getSysGCTotal()
    {
        return System.GC.GetTotalMemory(false);
    }

    public static long getGCHeapSize()
    {
        return mono_gc_get_heap_size();
    }

    public static long getGCUsedSize()
    {
        return mono_gc_get_used_size();
    }

    public static long getNativeHeapSizeAndroid()
    {
#if UNITY_ANDROID && !UNITY_EDITOR
        try
        {
            IntPtr cls = AndroidJNI.FindClass("android/os/Debug");
            IntPtr heapSize = AndroidJNI.GetStaticMethodID(cls, "getNativeHeapSize", "()J");
            return AndroidJNI.CallStaticLongMethod(cls, heapSize, new jvalue[0]);
        }
        catch (Exception)
        {
            return 0;
        }
#else
        return 0;
#endif
    }

    public static long getNativeUsedSizeAndroid()
    {
#if UNITY_ANDROID && !UNITY_EDITOR
        try
        {
            IntPtr cls = AndroidJNI.FindClass("android/os/Debug");
            IntPtr allocSize = AndroidJNI.GetStaticMethodID(cls, "getNativeHeapAllocatedSize", "()J");
            return AndroidJNI.CallStaticLongMethod(cls, allocSize, new jvalue[0]);
        }
        catch (Exception)
        {
            return 0;
        }
#else
        return 0;
#endif
    }

    public static long getThreadCpuUsageInNanoAndroid()
    {
#if UNITY_ANDROID && !UNITY_EDITOR
        try
        {
            IntPtr cls = AndroidJNI.FindClass("android/os/Debug");
            IntPtr allocSize = AndroidJNI.GetStaticMethodID(cls, "threadCpuTimeNanos", "()J");
            return AndroidJNI.CallStaticLongMethod(cls, allocSize, new jvalue[0]);
        }
        catch (Exception)
        {
            return 0;
        }
#else
        return 0;
#endif
    }

    public static float getThreadCpuUsageInSecAndroid()
    {
#if UNITY_ANDROID && !UNITY_EDITOR
        return (float)((double)getThreadCpuUsageInNanoAndroid() * (double)0.000000001);
#else
        return 0;
#endif
    }
}
