
# 手环服务器接口说明

**代码及地址**

- 部署地址: 138.68.109.151 
- 代码仓库（需权限）： https://gitlab.com/mc-gulu/sp_band_server

**细节**

- 端口 3333 为手环服务提供端口，请使用短信修改手环至连接此端口
- 端口 8080 为业务逻辑端口，请 tcp 到此端口，提供五个业务查询的功能 
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

**其它**

稍晚些可开放以下接口供批量或单独查询，以供后期数据分析和优化：

- 在连接有效的情况下，支持远程重启/升级手环
- 当前/历史 (过去一段时间内的) GSM 信号强度
- 当前/历史 (过去一段时间内的) 卫星个数及 GPS 信号情况
- 当前/历史 (过去一段时间内的) 电池消耗情况

## 新的 9 个业务逻辑接口及对应的查询接口 (2019-01-17 03:17 更新)

- 所有的接口均接受 Key 作为 URL 参数
- 4 个支持批量设置的接口为异步接口，这些接口统一返回一个 id，利用该 id 可查询对应操作的进展。

### 设备管理

- **ListDevices** 
    + **解释**: 列出所有设备
    + **参数**: 除 Key 外无参数
    + **返回**: json 设备表 （设备id，设备名字，状态（在线，离线），定位时间，方向 ，速度，电量）
- **AddDevice** (SN, DeviceName, ContestStatus)
    + **解释**: 添加单个设备
    + **参数**: 设备号，设备名，设备状态
    + **返回**: 返回 `{"Code":"1"}` 成功，其他返回值为失败
- **RemoveDevice** (SN)
    + **解释**: 移除单个设备
    + **参数**: 设备号
    + **返回**: 返回 `{"Code":"1"}` 成功，其他返回值为失败

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

### 其他接口

- **UploadFenceJSON** (MapName)
    + 上传栅栏的 Json 描述文件
    + MapName 为服务器上将保存的文件名
    + 此为 POST，request.body 部分为文件内容
- **QueryOperProgress** (CmdID)
    + 查询指定操作的进展，如果完成将附带对应操作的结果


