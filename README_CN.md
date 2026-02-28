1. 项目定位与整体架构
定位：
OpenFIRE-App-cn 是 OpenFIRE 光枪系统的 桌面配置与调试工具，用来：
识别接入的 OpenFIRE 板（RP2040 / ESP32-S3）。
读取板子当前所有配置（引脚、各种布尔/数值设置、按钮映射、Profile、TinyUSB ID……）。
在图形界面中修改这些配置。
通过 USB 串口 将修改写回板子 Flash，并触发重启 / 进入 bootloader。
进行光枪校准、IR 测试、FFB（震动、电磁铁）和 LED 测试。
与 OpenFIRE-Firmware-ESP32 的关系：
App 与固件共享一套协议与常量定义：boards/OpenFIREshared.h 中的 OF_Const。
对 ESP32 系列板卡（如 esp32-s3-devkitc-1）来说，这个 App 就是官方“控制台”：
配置编辑器（所有参数）
校准工具（大部分逻辑在固件 + PC GUI 配合完成）
引脚布局/板卡文档展示
固件升级辅助（重启到 bootloader）
通信方式：
完全基于 USB 虚拟串口（CDC）：
使用 Qt 的 QSerialPort / QSerialPortInfo。
波特率固定为 9600。
没有 TCP/UDP/WebSocket、没有 BLE、没有 HTTP。
2. 目录结构与主要模块（按你开发时需要关注的）
根目录
CMakeLists.txt：构建 Qt Widgets 桌面应用 OpenFIREapp。
README.md：项目介绍。
.github/：CI（Linux / Windows 构建）。
boards/：协议与板卡定义（和固件强相关，后续你扩展功能改这里最多）。
src/：桌面应用 C++ 源码。
img/：SVG 图标/板卡图/校准图案。
translation/：Qt 翻译文件（目前主要是英文）。
boards/（最关键：与固件共享）
OpenFIREshared.h（类 OF_Const）：
引脚功能枚举、功能名字符串表。
各类设置的枚举：
布尔设置 boolTypes_e + boolTypes_Strings（诸如 customPins, rumble, solenoid, autofire…）。
数值设置 settingsTypes_e + settingsTypes_Strings（RumbPwr, RumbTime, SolOn, SolOff, StaticPixels, AnalogMode…）。
Profile 字段 profSyncTypes_e + profSettingTypes_Strings（偏移、IR 灵敏度、颜色、宽高比、名称等）。
串口命令枚举 serialCmdTypes_e（App→固件 和 固件→App 的编解码命令）。
板卡相关：
boardTypes_e / boardArchs / boardNames。
boardsPresetsMap：各板默认功能→GPIO 布局；可加自定义 preset。
boardsBoxPositions：板卡 SVG 上的引脚坐标映射，用于 UI 可视化。
TinyUSB 设备 ID 结构体、默认 PID/Name。
这是你以后增加新命令、配置项、板卡布局时必改的地方，同样固件那边也有一个对应副本。
src/ 核心模块
main.cpp：Qt 应用入口。
appmainwindow.{h,cpp,ui}：
主窗口类 guiWindow。
负责：
串口设备列表刷新 / 选择。
调用 AppSerial 从板子读取所有配置 → 填充 App_Common。
把 App_Common 映射到各 UI 控件，反向也一样。
展示板卡引脚布局，按钮映射、设置页、Profile/校准页、测试页。
打开校准窗口、板卡预览窗口、调试窗口、关于窗口等。
appserial.{h,cpp}：
串口通信类 AppSerial（和协议绑定最紧的模块）。
负责：
枚举、过滤 OpenFIRE 设备（VID=0xF143）。
建立连接（含 dock 握手）。
批量获取 toggles/pins/settings/buttons/profiles/TinyUSB ID。
批量发送上述所有数据并保存到 Flash。
处理清 EEPROM、重启到 bootloader 等命令。
appcommon.h：
全局静态结构 App_Common：
板卡信息 board（type/arch/version/selectedProfile…）。
所有布尔设置、数值设置双缓冲（Current vs Orig）。
按钮映射表、引脚 map、Profile 表、TinyUSB ID 表。
键盘/鼠标/手柄映射 map（用于 UI combobox 选项）。
appcali.{h,cpp,ui}：
校准/IR 对齐/IR 测试全屏窗口 AppCaliWindow。
通过串口命令驱动固件进入不同模式，接收坐标/阶段更新来绘制画面。
apppreviewer.{h,cpp,ui}：
板卡预览窗口 AppBoardsPreviewer。
从 OF_Const 中读取板信息和 pin 能力，配合 SVG 显示布局。
appdebug.{h,cpp,ui}：
Debug 用的串口日志窗口（只在 Debug 构建或者定义宏时出现）。
appabout.{h,cpp,ui}：
“关于”窗口，展示版本号和 Qt 版本。
3. 启动与运行流程（你要找入口和数据流时，看这里）
入口：src/main.cpp
创建 QApplication。
加载翻译。
创建并显示 guiWindow。
进入事件循环。
主窗口初始化（guiWindow 构造函数在 appmainwindow.cpp）大致步骤：
ui->setupUi(this); 加载 UI。
连接 AppSerial 的状态/进度/数据到 MainWindow 的槽函数。
设置串口监控定时器 aliveTimer，定期调用 AppSerial::SearchPorts()：
如检测到新的 OpenFIRE 设备，刷新端口列表。
构建按钮映射 UI：
为每个逻辑按钮+模式创建三个 combobox（Mouse/Keyboard/Gamepad 输出）。
combobox 的内容从 App_Common 的 mapping 表构建。
构建测试 UI（按键测试 label、模拟摇杆显示）。
初始时禁用所有 Tab（未连接板子前不可操作）。
选择串口 → 同步数据：
用户从 comPortSelector 选一个设备 → 槽 on_comPortSelector_currentTextChanged：
调用 AppSerial::GetSettings(portName)。
成功后：
App_Common::* 填充完整。
UI 中各控件设置成这些值。
TabWidget 启用，用户可以编辑。
用户修改配置、点击“发送到板子”：
MainWindow 比较 Current vs Orig，如有更改则启用确认按钮。
点击后调用 AppSerial::CommitSettings()：
分批发：布尔设置 → Pins → 数值设置 → 按钮映射 → Profile → TinyUSB ID → Save to Flash。
每一步都有校验（固件回显）和进度条显示。
成功后，将 Current 复制为 Orig。
4. 与固件（包括 ESP32）通信的机制与协议要点
设备发现
用 QSerialPortInfo::availablePorts() 列出全部串口。
通过 vendorIdentifier == 0xF143 过滤出 OpenFIRE 设备。
对每个设备显示友好的名字供选择。
建立连接 + Dock 握手（AppSerial::GetSettings）
打开串口，9600 波特。
发送 sDock1 和 sDock2 命令字。
固件回应：
版本字符串（固件版本）。
板卡标识字符串（与 OpenFIREshared.h 中的 board 类型一致）。
TinyUSB ID 结构体。
可选错误标志（如摄像头问题）。
App 据此设置 board.type、board.arch、tinyUSBtable 等。
批量读取配置（只读阶段）
依次发送命令：
sGetToggles → App_Common::boolSettings[dataCurrent]
sGetPins → inputsMap（当 customPins 开启）
sGetSettings → settingsTable[dataCurrent]
sGetBtns → inputFuncTable[dataCurrent]
sGetProfile → profilesTable
固件按“字段名 + 数据”连续发送，App 用 OF_Const 中的字符串表查找对应 index 填入。
所有 Current 表复制一份为 Orig，用于后续 diff。
批量写回配置（Commit 阶段）
最前面发送 sCommitStart，等固件回 ACK。
对每个设置类型调用 BatchSendSettings：
每条数据都带上 字段名字符串，这样协议对扩展很友好。
Profile 写入时带上 Profile 索引。
TinyUSB ID 用 sCommitID 独立命令写入并校验。
最后发送 sSave，确保固件把新配置写入 Flash。
测试 / 校准命令
App 通过 AppSerial 或 MainWindow 直接写串口：
FFB 测试（震动、电磁铁、LED）。
清 EEPROM、复位波特率。
进入 IR 测试 / 校准模式等（链接 AppCaliWindow 中 UI）。
固件通过状态命令 sBtnPressed/sBtnReleased, sAnalogPosUpd, sTestCoords, sCaliStageUpd, sCaliInfoUpd 等回报状态，App 更新测试页和校准页。
重启到 bootloader（用于刷 ESP32 固件）
RP 平台：1200 波特技巧。
ESP32：发送两字节 { sRebootToBootloader, sRebootToBootloader }，由固件负责重启进入 bootloader。
5. 配置和状态是怎么管理的（对你设计新参数很关键）
所有长期配置只保存在 “板子 Flash” 上：
App 不用 QSettings 也不保本地 JSON/DB。
每次连接板子时全量读取，修改后再全量写回。
整个协议就是“固件是单一真相源”。
App 端的内存模型：App_Common
把固件里的配置映射为 C++ 结构体和数组：
board：板类型、架构、固件版本、当前/上一个 profile。
boolSettings[2][boolTypesCount]：一份 Current，一份 Orig。
settingsTable[2][settingsTypesCount]：同理。
inputsMap / inputsMap_orig：函数 → GPIO 编号。
inputFuncTable[2][BUTTON_COUNT][...]：按钮映射。
profilesTable / profilesTable_orig：Profile 数组。
tinyUSBtable / _orig：USB ID 配置。
UI 控件只操作 Current 表，确认写回后才覆盖 Orig。
这种结构对你后续加新配置项非常友好：只要保持 OF_Const 的枚举和字符串表与固件一致，再把 UI 跟 App_Common 新字段绑起来即可。
6. 主要 UI / 页面功能概览（你以后要改界面或加控件，优先找这里）
主窗口 guiWindow 的几个 Tab：
串口与状态栏（顶部）
串口下拉框、连接状态。
状态栏进度条显示当前通讯阶段。
板卡布局 / Pins 页
显示当前板卡引脚图、每个 pin 的默认功能与能力（ADC、I2C、SPI 等）。
勾选“自定义引脚”后，可调整功能→GPIO 的 map。
支持选择不同 preset（适合不同光枪或线序）。
按钮映射页
给每个逻辑输入定义：
屏幕上时的输出（键盘/鼠标/手柄）。
屏幕外时的输出。
手柄模式时的输出。
说明框对当前选中控件给出详细解释。
Settings 设置页
震动、电磁铁、Autofire、Pause 行为、LED 设置、I2C OLED 等常规功能。
温度告警/保护阈值、电磁铁与震动功率/时间、像素条灯渲染等。
TinyUSB PID/Name。
Profiles / 校准页
显示/编辑每个 Profile 的：
名称、IR 灵敏度、布局（Square/Diamond）、宽高比、颜色等。
入口按钮打开 AppCaliWindow 进行实际的瞄准校准。
测试 / 诊断页
按钮测试（按下/抬起高亮）、模拟轴显示。
发命令测试震动、电磁铁、LED。
扩展命令（如清 EEPROM、复位波特率）、IR 测试等。
其他窗口
AppCaliWindow：校准 / IR 对齐 / IR 测试。
AppBoardsPreviewer：查看所有支持板卡的引脚布局和能力。
AppDebugWindow：调试串口原始收发内容（方便你新增命令时调试）。
AppAbout：关于对话框。
7. 后续扩展功能的推荐切入点
结合上面分析，如果你后面要在 OpenFIRE-Firmware-ESP32 + 这个 App 上一起加功能，推荐这样走：
A. 新增一个配置项（布尔 / 数值）
固件：在 ESP32 工程的 OpenFIREshared.h 同步增加枚举 + 名称字符串。
App 侧修改：
boards/OpenFIREshared.h：对应增加 boolTypes_e / settingsTypes_e 和 *_Strings。
appcommon.h：确认数组大小与枚举数一致（通常不需要改，只要 enum 在中间插就还好，但建议保持尾部追加）。
appmainwindow.ui/.cpp：在 Settings 页增加控件，读写 App_Common::boolSettings 或 settingsTable 的对应 index。
好处：AppSerial 的批量收发逻辑已经是泛型的，不用单独改函数。
B. 新增一个即时串口命令（比如实时调试、日志开关）
boards/OpenFIREshared.h：给 serialCmdTypes_e 增加一个新的命令号。
固件：在串口解析处支持这个命令。
App：
在 appmainwindow.cpp 中添加按钮或菜单项，调用 AppSerial::OneShotSend 或 port.write 发送此命令。
在 AppSerial::DataReceived 或主窗口的 serialPort_readyRead 中解析新的状态码，更新 UI 或写入 AppDebugWindow。
建议先在 Debug 窗口中打印原始数据，调通协议后再做 UI。
C. 为 ESP32 新板卡 / 自制板增加 preset 布局
boards/OpenFIREshared.h：
在 boardsPresetsMap 中添加你的板 ID 和对应 GPIO 布局。
在 boardNames & boardArchs 中登记。
如需要，在 boardsBoxPositions 中加入对应板的 SVG 坐标映射（UI 会用来高亮引脚）。
完成后：
主窗口的引脚布局页和 AppBoardsPreviewer 会自动支持这个新板。
固件也应使用同样的 board ID，以实现一致行为。
D. 扩展 Profile / 校准参数
若你想在校准里加入新参数（例如非线性修正系数）：
boards/OpenFIREshared.h：
扩展 profSyncTypes_e 和 profSettingTypes_Strings。
固件：Profile 结构体加字段，并在同步协议里处理这个字段。
App：
appcommon.h 的 profilesTable_s 结构体加同样字段，并保证顺序、类型兼容。
appcali.cpp / appmainwindow.cpp 增加 UI 控件来显示/编辑这些字段。
E. 增加一个“高级 ESP32 设置”页面
在 appmainwindow.ui 里给 ESP32 单独搞一个 Tab 或 GroupBox。
在 appmainwindow.cpp 控制这个 Tab 只在 board.arch == esp32-s3 时可见。
通过新增配置项 + 串口命令，把你在 ESP32 固件里的高级功能（比如 WiFi 相关调试、日志等级、额外传感器）暴露出来。