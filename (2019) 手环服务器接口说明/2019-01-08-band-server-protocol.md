
# 手环服务器接口说明

**地址**

- 部署地址
    + 国服： 118.24.205.20
    + 外服： 138.68.109.151 
    + 端口 3333 为手环服务提供端口，请使用短信修改手环至连接此端口
    + 端口 8080 为业务逻辑端口，请 http 到此端口

## 老的 5 个接口

- 具体按照 "接口说明与调试.txt" 提供数据，格式为 json
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
- 使用例子
    + 协议 GetDevice
        * http://138.68.109.151:8080/GetDevice?SN=3000011451&Key=36fa8efbeb55d14343fefccf0cffd187
        * 返回： `{"Code":"1","DeviceName":"设备名称","ContestStatus":"3"}`
    + 更多例子见：[test_classic.go](./test_classic.go)

## 新的业务逻辑接口

- 所有的接口均接受 Key 作为 URL 参数
- 4 个支持批量设置的接口为异步接口，这些接口统一返回一个 id，利用该 id 可查询对应操作的进展。

### 设备管理

- **ListDevices** (SNs 或 Fuzzy)
    + **解释**: 列出所有设备或筛选后的设备
    + **参数**: SNs 如果有指定，列出指定的设备
        * 注意，可以不传递 SNs 而是指定 Fuzzy 作为模糊查询，如指定 "101" 则返回 3000010127,3000010113 等含有 "101" 的设备
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
- **GetOnlineDevices** ()
    + **解释**: 返回当前所有在线设备的列表
    + **参数**: (无)
    + **返回**: 在 OnlineDevices 内返回设备列表 
        * `{"Code":"1","OnlineDevices":[3000009600,3000010377,3000010062,3000010127,3000010113,3000010115,3000009465,3000009597,3000009586,3000009808,3000011451,3000010902,3000012849,3000013496,3000010911]}` 

### 批量设置

- POST **SetTrackingInterval** (SNs)
    + 批量或单个设备设置定位间隔
    + 需在 Form 中指定 Interval 为间隔秒数
- POST **PowerOff** (SNs)
    + 批量或单个设备关机
- POST **SetSleepPeriods** (SNs)
    + 批量或单个设置休眠时段，其中 periods 为时段
    + 需在 Form 中指定 Sleep1/Sleep2/Sleep3 为休眠时段
    + 具体设置代码形如：“`v.Set("Sleep1", "08:00-12:00")`”
- POST **ToggleAlertStaying** (SNs)
    + 批量或单个设置停留报警
    + 需在 Form 中指定 Toggle 为 1 设置 2 取消
    + 需在 Form 中指定 Duration 为 n 意为报警时间持续 n 秒

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
        * SNs 围栏绑定的手环设备号数组
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
        * SNs 围栏绑定的手环设备号数组

### 其他接口

- **QueryOperProgress** (CmdID)
    + 查询指定操作的进展，如果完成将附带对应操作的结果

### Change History

- 2019-01-25
    + feat: 迁移服务器到国内腾讯云(重庆) 118.24.205.20
    + feat: 围栏支持设备绑定
    + fix: IsDeviceOnline 当找不到设备时应返回恰当的错误
    + fix: 接口有返回中文的时候，编码不是utf-8的 (经调查没有问题)
    + fix: 新增的去重代码未处理新建一个打点失败的情况
- 2019-01-23
    + feat: **数据去重** 时间重合或更早的数据直接拦掉
        * 增加测试用例 `01_reject_dup`
    + feat: **报警按通知类型筛选** 返回对应的报警信息，如果参数为空，就返回所有。
    + feat: **新增接口 GetOnlineDevices** - 返回当前所有在线设备的列表
    + feat: **硬件坐标转高德坐标** 使用高德的 API
    + feat: 支持 ListDevices 的模糊查询，具体见文档
    + fix: 修复 tcp 服务器高 cpu 占用率的 bug

