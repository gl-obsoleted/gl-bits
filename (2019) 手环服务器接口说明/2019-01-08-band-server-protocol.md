
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

## 新的 6 个业务逻辑接口及对应的查询接口

- 由于这些操作会耗时完成，所有的接口均为异步接口
- 这些接口统一返回一个 id，利用该 id 可查询对应操作的进展。

-----------------

- **SetTrackingInterval** (SNs, interval)
    + 批量或单个设备设置定位间隔，其中 interval 为间隔的秒数
- **PowerOff** (SNs)
    + 批量或单个设备关机
- **SetSleepPeriods** (SNs, periods)
    +  批量或单个设置休眠时段，其中 periods 为时段
- **ToggleAlertStaying** (SNs, on, alertDuration)
    +  批量或单个设置停留报警，其中 on 为是否开启，alertDuration 为报警时长
- **GetFullStatus** (SNs)
    +  批量或单个设备返回当前设备状态
- **GetFullHistory** (SNs)
    +  批量或部分设备下载轨迹

-----------------

- **QueryOperProgress** (id)
    + 查询指定操作的进展，如果完成将附带对应操作的结果

