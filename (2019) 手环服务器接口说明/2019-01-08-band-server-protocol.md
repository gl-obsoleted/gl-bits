
# 手环服务器接口说明

**地址**

- 部署地址: 138.68.109.151 
    + 端口 3333 为手环服务提供端口，请使用短信修改手环至连接此端口
    + 端口 8080 为业务逻辑端口，请 http 到此端口
    + (获取/更新设备参数，获取多设备当前/历史定位，获取警告列表)
    + (具体按照 "接口说明与调试.txt" 提供数据，格式为 json)
- UpdateDevice 需要 POST 其他接口是 GET
- 已有的手环数据已从后台导出 excel 并导入到新的后台，可供查询
- 已增加 locationType 字段 (2019-01-06)
    + locationType 目前只支持 1. GPS (其他两种 lbs/wifi 目前不支持)
- MapType 目前暂时没用 (还未做坐标转换，返回的坐标是原始数据) 

**举例**

- 协议 GetDevice
    + http://138.68.109.151:8080/GetDevice?SN=3000011451&Key=36fa8efbeb55d14343fefccf0cffd187
- 返回： `{"Code":"1","DeviceName":"设备名称","ContestStatus":"3"}`

## 老的 5 个接口

- GetDevice / GetTrackingBySNs / GetDevicesHistoryBySNs / GetExceptionMessageBySNs 全部是 GET 参数见范例
- **UpdateDevice**  POST 接口
    + **解释**: 
        * 更改设备名 DeviceName 和竞赛状态 ContestStatus
        * DeviceName 为空、 ContestStatus < 0 表示不改变
    + **参数**: 
        * URL 内提供
            - Key 
        * Form 内提供
            - SN
            - DeviceName
            - ContestStatus
    + **返回**
        * 返回 `{"Code":"1"}` 成功
- 使用例子见：[test_classic.go](./test_classic.go)

## 新的 14 个业务逻辑接口及对应的查询接口 

- 所有的接口均接受 Key 作为 URL 参数
- 4 个支持批量设置的接口为异步接口，这些接口统一返回一个 id，利用该 id 可查询对应操作的进展。

### 设备管理

- **ListDevices** (SNs)
    + **解释**: 列出所有设备或筛选后的设备
    + **参数**: SNs 如果有指定，列出指定的设备
    + **返回**: 设备列表（设备id，设备名字，在线状态（在线，离线），竞赛状态，定位时间，方向 ，速度，电量）
- **AddDevice** (SN, DeviceName, ContestStatus)
    + **解释**: 添加单个设备
    + **参数**: 设备号，设备名，设备状态
    + **返回**: 返回 `{"Code":"1"}` 成功，其他返回值为失败
- **RemoveDevice** (SN)
    + **解释**: 移除单个设备
    + **参数**: 设备号
    + **返回**: 返回 `{"Code":"1"}` 成功，其他返回值为失败
- **IsDeviceOnline** (SN)
    + **解释**: 查看单个设备是否在线
    + **参数**: 设备号
    + **返回**: 返回 `{"Code":"1","IsOnline":"1"}` 表示在线

-------------------------

### 批量设置

- **SetTrackingInterval** (SNs)
    + 批量或单个设备设置定位间隔
    + 此为 POST，需在 Form 中指定 Interval 为间隔秒数
- **PowerOff** (SNs)
    + 批量或单个设备关机
    + 此为 POST
- **SetSleepPeriods** (SNs)
    + 批量或单个设置休眠时段，其中 periods 为时段
    + 此为 POST，需在 Form 中指定 Sleep1/Sleep2/Sleep3 为休眠时段
    + 具体设置代码形如：“`v.Set("Sleep1", "08:00-12:00")`”
- **ToggleAlertStaying** (SNs)
    + 批量或单个设置停留报警
    + 此为 POST
    + 需在 Form 中指定 Toggle 为 1 设置 2 取消
    + 需在 Form 中指定 Duration 为 n 意为报警时间持续 n 秒

-------------------------

### 围栏接口

- GET **ListFences** (Type) 列出所有围栏
    + Type 为可选参数，1普通，2禁区，用于按类型筛选
- GET **GetFence** (ID)
    + 列出指定 ID 的围栏
- POST **AddFence** 添加围栏
    + PostForm 参数
        * Name 围栏名
        * Type 围栏类型
        * Plots 围栏数据类型为 json 字符串
    + 返回新创建围栏的 ID
- POST **RemoveFence** 删除指定 ID 的围栏
    + PostForm 参数
        * ID 待删除的围栏 ID
- POST **UpdateFence** 更新指定 ID 的围栏
    + PostForm 参数
        * ID 待更新的围栏 ID
        * Name 围栏名
        * Type 围栏类型
        * Plots 围栏数据类型为 json 字符串

### 其他接口

- **QueryOperProgress** (CmdID)
    + 查询指定操作的进展，如果完成将附带对应操作的结果


