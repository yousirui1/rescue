## 2.26
1. 增加tftp 安装方式
2. 修改启动参数错误
3. 安装默认桌面在安装完客户端程序之后执行

## 2.25
1. 修改所以模板下发接口， 种子修改成http 下发方式，改动比较大，涉及所以模板下发部分
2. bt 下发任务时上传管理平台进度由1s改成10s一次
3. 维护模式情况下，检测本地是否存在桌面，不存在则自动下发默认桌面
4. 增加alt + F2 清空所有桌面并下发默认桌面 （单次启动只能执行一次）
5. 下发中断自动重连下发

## 2.24
1. nvme u盘安装
2. http 安装vmlinuz 文件名错误
3. 去掉日志上传
4. 桌面组资源获取不到，重连之后重新请求，防止没获取到桌面枚举资源无法下载问题， 但是会导致重复自动更新桌面。
5. 获取不到dhcp时采用静态ip
6. 修改pxe 安装文件丢失的问题

## 2.23
1. p2v 上传容量为0
2. deskop_group_list 接口 丢失共享盘处理

## 2.22
1. 删除指定桌面diff 5 没有删除bug

## 2.21
1. 删除指定桌面增加字段用于管理平台显示
2. 删除桌面枚举接口调试打印防止8个桌面以上导致内存溢出崩溃
3. usb 安装方式下载默认桌面

## 2.20
1. 增加了u盘搜索分区的功能，防止分区号不是1导致挂载失败然后安装失败

## 2.19
1. 修改tcp 下载后存在diff 3 磁盘数据导致数据版本不同步 和 tcp 下发中断问题

## 2.18
1. 修改bt 下载超时时间为20 pack
2. 初始化时不请求默认桌面下载
3. 自动更新时不更新desktop配置文件
4. 终端增加http方式安装

## 2.17 & 2.16
1. 修改磁盘不足时弹框异常导致的崩溃问题 (pipe thread 刷新UI界面导致的 改用qt 信号量)
2. 增加coredump 用于调试
3. bt 超时时间修改为120s

## 2.15
1. 修改下载完成写desktop_config.ini 防止efi离线不能显示桌面
2. 增加bt 下载超时60s 超时之后重新下载
3. 增加http 下载桌面功能
4. 增加删除指定桌面功能

## 2.14
1. 修改配置参数提交 9006 接口去掉 9004 增加参数
2. 修改下载数据不完整导致磁盘启动失败

## 2.13
1. PXE 自动部署 当设置服务器ip 时 自动安装底层客户端并下载默认模板桌面
2. 正常流程安装完底层客户端不重启并下载默认桌面
3. nvme 磁盘可能格式失败问题

## 2.12 
1. 覆盖模式和增量模式来回多次切换导致系统异常问题和不更新下载桌面的问题

## 2.11
1. bt 重复下载先删除torrent_handle 然后在重新下载防止位置错误导致蓝盘
2. 模板后添加数据盘不会自动更新问题
3. 先查询本地系统然后在下载共享盘
4. 800x600分辨率能正常上传模板

## 2.10
1. bt download 99% return bug

## 2.9
1. netmask to server 255.255.255.0 bug
2. 增加pxe带参数启动 存在server_ip自动安装终端和默认下载桌面

## v2.8
1. 合并差分下错位置导致合并异常
2. 日志上传功能

## v2.7
1. 修改模式切换时diff_mode 错误

## v2.6
1. 修改中断下载进度归0
2. 修改无系统盘不自动更新共享盘

## v2.5
1. 修改下载流程
2. 修改了磁盘不足之后报错之后不会跳部署界面，和更新软件超时不会跳转到部署界面
3. 去掉F2 网络配置功能

## v2.4
1. 增加I219-LM PCI-E 网卡支持 e1000e.ko

## v2.3
1. 管理平台主动下发共享盘需要比较本地共享盘是否存在和操作号
2. 更新请求共享盘接口种子增加磁盘类型
3. 修改第一次安装时磁盘节点不存打印节点信息在导致崩溃

## v2.2
1. 修改下载进度100% 不继续往下执行，导致卡死的问题

## v2.1
1. 修改磁盘链接bug

## v2.0
1. 修改磁盘节点信息，不兼容旧版本
2. 增加SD和emmc磁盘的支持
3. 界面增加了版本信息和增量模式和覆盖模式
4. 增加数据盘+1M size 防止未初始化打开异常导致数据盘

## v1.30
1. 修改bt下载导致下载时可能崩溃, 取消bt为单独线程不退出使用, 改为全局变量bt客户端task 

## v1.29 
1. 心跳包未回复离线重连
2. 模式切换 存储在磁盘节点1上 
3. 模式切换后不生效问题
4. 增加login flag 只做一次磁盘检测更新或更新桌面组数据后更新,防止反复重连导致多次更新处下发磁盘
5. 下载显示说明数据盘系统盘， 共享盘   
6. 修改bt为单独线程不退出使用, 保证下载完成后能继续上传数据
7. 取消下发然后还在下发
8. 共享盘 
9. 升级失败
10. 容量不足后仍在下发

