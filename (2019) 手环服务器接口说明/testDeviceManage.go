package main

func testDeviceManagement() {
	// 测试全部设备列表
	_get("ListDevices", makeValues(map[string]string{
		"Key": "36fa8efbeb55d14343fefccf0cffd187",
	}))

	// 测试使用 SNs 筛选后的设备列表
	_get("ListDevices", makeValues(map[string]string{
		"Key": "36fa8efbeb55d14343fefccf0cffd187",
		"SNs": "3000009808,3000010000",
	}))

	// 测试模糊查询的设备列表
	_get("ListDevices", makeValues(map[string]string{
		"Key":   "36fa8efbeb55d14343fefccf0cffd187",
		"Fuzzy": "101",
	}))

	_get("AddDevice", makeValues(map[string]string{
		"Key":           "36fa8efbeb55d14343fefccf0cffd187",
		"SN":            "3000009600",
		"DeviceName":    "hellow",
		"ContestStatus": "2",
	}))

	_get("RemoveDevice", makeValues(map[string]string{
		"Key": "36fa8efbeb55d14343fefccf0cffd187",
		"SN":  "3000010000",
	}))

	_get("GetOnlineDevices", makeValues(map[string]string{
		"Key": "36fa8efbeb55d14343fefccf0cffd187",
	}))
}
