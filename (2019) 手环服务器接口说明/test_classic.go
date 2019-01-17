package main

func testClassicInterfaces() {

	_get("GetDevice", makeValues(map[string]string{
		"Key": "36fa8efbeb55d14343fefccf0cffd187",
		"SNs": "3000009808",
	}))

	_get("GetTrackingBySNs", makeValues(map[string]string{
		"Key": "36fa8efbeb55d14343fefccf0cffd187",
		"SNs": "3000009808",
	}))

	_get("GetDevicesHistoryBySNs", makeValues(map[string]string{
		"Key":        "36fa8efbeb55d14343fefccf0cffd187",
		"SNs":        "3000010902, 3000009597",
		"StartStrs":  "2019-01-07 11:41:00, 2019-01-06 11:41:00",
		"End":        "2019-01-09 11:42:00",
		"PointCount": "1000",
		"ShowLBS":    "0",
		"MapType":    "Baidu",
	}))

	_get("GetExceptionMessageBySNs", makeValues(map[string]string{
		"Key":               "36fa8efbeb55d14343fefccf0cffd187",
		"SNs":               "3000010377",
		"MapType":           "Baidu",
		"StartTime":         "2019-01-05 11:41:00",
		"EndTime":           "2019-01-07 11:42:00",
		"NotificationTypes": "3,5",
	}))

	_postform("UpdateDevice",
		makeValues(map[string]string{
			"Key": "36fa8efbeb55d14343fefccf0cffd187",
		}),
		makeFormValues(map[string]string{
			"DeviceName":    "又改了名字",
			"ContestStatus": "1",
			"SN":            "3000009600",
		}))
}
