using UnityEngine;
using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.Collections.Generic;

public class UCpuUsageInfo
{
    public float lastTimestamp = 0.0f;
    public float lastUsagestamp = 0.0f;
    public float ratio = 0.0f;

    public bool IsValid { get { return lastTimestamp > 0.0f; } }

    public void Update(float time, float usage)
    {
        if (IsValid)
        {
            ratio = 100.0f * ((usage - lastUsagestamp) / (time - lastTimestamp));
        }

        lastTimestamp = time;
        lastUsagestamp = usage;
    }
}

public class Testbed : MonoBehaviour {

    GUILayoutOption[] options = new GUILayoutOption[2];
    List<byte[]> _memFiller = new List<byte[]>();

    UCpuUsageInfo _lastFrame = new UCpuUsageInfo();
    UCpuUsageInfo _last3Sec = new UCpuUsageInfo();

    bool _fillMem = false;

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
        float curTime = Time.realtimeSinceStartup;
        float curUsage = UPerfQuery.getThreadCpuUsageInSecAndroid();

        if (_fillMem)
        {
            if ((int)curTime % 3 == 0)
            {
                _memFiller.Add(new byte[1024 * 1024]);
            }
        }
        else
        {
            _memFiller.Clear();
            System.GC.Collect();
        }

        _lastFrame.Update(curTime, curUsage);
        
        if (curTime - _last3Sec.lastTimestamp > 3.0f)
            _last3Sec.Update(curTime, curUsage);
    }

	void OnGUI() {
        SetGUISkinFontSize(50);

        options[0] = GUILayout.MinWidth(800);
        options[1] = GUILayout.MinHeight(60);

        GUILayout.Label(string.Format("Cpp used: {0}", UPerfQuery.getNativeUsedSizeAndroid()), options);
        GUILayout.Label(string.Format("Cpp heap: {0}", UPerfQuery.getNativeHeapSizeAndroid()), options);
        GUILayout.Label(string.Format("C# used: {0}", UPerfQuery.getGCUsedSize()), options);
        GUILayout.Label(string.Format("C# heap: {0}", UPerfQuery.getGCHeapSize()), options);
        GUILayout.Label(string.Format("System.GC: {0}", UPerfQuery.getSysGCTotal()), options);
        GUILayout.Label(string.Format("last frame: {0:0.00}%", _lastFrame.ratio), options);
        GUILayout.Label(string.Format("last 3 sec: {0:0.00}%", _last3Sec.ratio), options);

        if (GUILayout.Button("fill mem"))
        {
            _fillMem = true;
        }

        if (GUILayout.Button("clear mem"))
        {
            _fillMem = false;
        }
    }

	void SetGUISkinFontSize(int fontSize) {
        GUI.skin.button.fontSize = fontSize;
        GUI.skin.label.fontSize = fontSize;
        GUI.skin.toggle.fontSize = fontSize;
        GUI.skin.textArea.fontSize = fontSize;
        GUI.skin.textField.fontSize = fontSize;
    }
}
