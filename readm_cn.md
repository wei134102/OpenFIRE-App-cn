#### 喜欢我们的作品？[记得支持开发者！](https://github.com/TeamOpenFIRE/.github/blob/main/profile/README.md)

![header](OFA_header.png)
# OpenFIRE 桌面配置应用
### [OpenFIRE 光枪系统](https://github.com/TeamOpenFIRE/OpenFIRE-Firmware)的参考配置工具，使用 Qt 和 C++ 编写。

## 项目简介
OpenFIRE-App-cn 是一款专为 OpenFIRE 光枪系统设计的跨平台配置工具，允许用户轻松设置、校准和测试光枪设备。该应用程序使用 Qt 和 C++ 开发，支持 Linux、Windows 桌面系统以及 Raspberry Pi。

当前版本：3.0.1

代码名称：Tokinomiya

许可证：GNU 通用公共许可证 v3.0

## 主要功能

### 1. 设备配置与管理
- 自动检测并列出可用的 OpenFIRE 设备
- 查看和修改设备的引脚布局
- 启用/禁用自定义映射
- 管理力反馈（FFB）和 I2C 外设等高级设置
- 实时更新设备配置，无需重启

### 2. 校准系统
- 直观的全屏校准界面，支持简单准确的校准
- 支持多个配置文件的独立校准
- 校准过程包括：顶部、底部、左侧、右侧、中心校准点和验证步骤
- 提供红外发射器对齐助手，帮助优化发射器位置

### 3. 开发板预览
- 预览所有支持的开发板布局，包括：
  - Raspberry Pi Pico/Pico W/Pico 2/Pico 2 W
  - Arduino Nano RP2040
  - Adafruit ItsyBitsy RP2040
  - Adafruit KB2040
  - ESP32-S3 DevKitC-1
  - Waveshare ESP32-S3 Pico
  - Waveshare Zero
  - 通用开发板
- 显示每个引脚的默认功能和能力（I2C 类型和通道等）
- 交互式界面，可高亮显示特定引脚

### 4. 测试工具
- 测试数字和模拟输入
- 测试力反馈设备
- 测试摄像头功能
- 提供调试模式（OFAPP_DEBUG）以获取更详细的日志输出

### 5. 多语言支持
- 基于 Qt 的翻译系统
- 支持英语和中文等多种语言

## 技术架构

### 核心组件

#### 1. 主窗口模块 (appmainwindow.h/cpp)
- 实现主用户界面和交互逻辑
- 管理设备连接和断开
- 处理用户配置变更
- 协调各个功能模块的工作

#### 2. 串口通信模块 (appserial.h/cpp)
- 搜索和连接串口设备
- 与 OpenFIRE 设备进行数据交换
- 实现设置的读取和保存
- 处理通信错误和超时

#### 3. 校准模块 (appcali.h/cpp)
- 实现全屏校准界面
- 处理校准过程中的用户输入
- 提供红外发射器对齐功能
- 支持校准数据的保存和应用

#### 4. 开发板预览模块 (apppreviewer.h/cpp)
- 显示开发板 SVG 图形
- 实现交互式引脚高亮
- 显示引脚功能和能力信息

#### 5. 公共定义模块 (appcommon.h)
- 定义共享的常量和数据结构
- 实现全局配置管理
- 提供工具函数

#### 6. 共享板卡定义 (boards/OpenFIREshared.h)
- 定义支持的开发板类型
- 提供引脚和功能映射
- 定义通信协议和命令

### 数据流
1. 用户通过主界面选择设备和配置选项
2. 配置变更通过串口通信模块发送到设备
3. 设备返回确认和状态信息
4. 应用程序更新界面以反映当前配置
5. 校准过程中，用户输入通过校准模块捕获并转换为校准数据

### 技术栈
- **编程语言**: C++17
- **框架**: Qt 5.15.X 或 Qt 6.x
- **构建系统**: CMake 3.5+
- **版本控制**: Git
- **UI 组件**: Qt Widgets、Qt SVG
- **通信**: Qt SerialPort
- **并发处理**: Qt Concurrent

## 项目结构

```
├── .github/            # GitHub 配置和工作流
├── boards/             # 板卡定义和图片
│   ├── OpenFIREshared.h # 共享的板卡定义
│   └── boardPics/      # 板卡 SVG 图片
├── img/                # 应用程序图片和资源
│   ├── cali/           # 校准相关资源
│   ├── ico/            # 图标和资源文件
│   └── testFont/       # 测试字体
├── src/                # 源代码
│   ├── appmainwindow.* # 主窗口实现
│   ├── appserial.*     # 串口通信实现
│   ├── appcali.*       # 校准窗口实现
│   ├── apppreviewer.*  # 板卡预览器实现
│   ├── appcommon.h     # 公共定义
│   └── main.cpp        # 程序入口
├── translation/        # 翻译文件
├── CMakeLists.txt      # CMake 构建脚本
├── README.md           # 英文 README
└── readme_cn.md        # 中文 README
```

## 构建指南

### Linux 系统

#### Arch Linux
需要安装以下依赖：
- qtX-base
- qtX-serialport
- qtX-svg

可以通过 AUR 安装：
- [`openfireapp`](https://aur.archlinux.org/packages/openfireapp)：稳定版本
- [`openfireapp-git`](https://aur.archlinux.org/packages/openfireapp-git)：从源代码构建的开发版本

#### Debian/Ubuntu
需要安装以下依赖：
```bash
sudo apt-get install build-essential cmake qttools5-dev libqt5serialport5-dev libqt5svg5-dev
```

从源代码构建：
```bash
git clone https://github.com/TeamOpenFIRE/OpenFIRE-App
cd OpenFIRE-App
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./OpenFIREapp
```

如果需要使用特定的 Qt 版本，可以添加 `-DOFAPP_QT_VERSION=Qt5` 或 `-DOFAPP_QT_VERSION=Qt6` 到 CMake 命令。

### Windows 系统

需要从 Qt 安装向导/维护工具安装 Qt 5.15.2 或更高版本，并确保安装了 SerialPort 扩展。

可以通过以下方式构建：
1. 使用 Qt Creator 打开项目
2. 选择适当的构建套件
3. 构建并运行

或者，也可以在 mingw-w64 环境中通过 CMake 构建。

## 运行指南

### Linux 系统
确保用户属于 `dialout` 组以访问串口设备：
```bash
sudo usermod -a -G dialout 用户名
```
注销并重新登录后，运行 `OpenFIREapp` 启动应用程序。

### Windows 系统
1. 下载最新的发布版本 zip 文件
2. 解压到任意位置
3. 运行 `OpenFIREapp.exe`

注意：Windows 7 系统可能存在兼容性问题，建议使用 Windows 10 或更高版本。

## 使用说明

### 1. 连接设备
启动应用程序后，可用设备会定期刷新并显示在 COM 端口下拉框中。选择对应的端口连接设备。

### 2. 配置设备
连接设备后，可以：
- 在引脚布局选项卡中查看和修改引脚映射
- 在按钮映射选项卡中设置按钮功能
- 在枪设置选项卡中调整力反馈、自动开火等设置

### 3. 校准设备
点击校准按钮启动全屏校准界面，按照提示完成校准过程。

### 4. 查看开发板
从帮助菜单中选择"查看兼容板"，可以预览所有支持的开发板及其引脚布局。

### 5. 保存设置
完成配置后，点击确认按钮保存设置到设备。

## 常见问题

### 设备无法连接
- 确保设备已正确连接到电脑
- 检查设备驱动程序是否已安装
- 确认用户有权限访问串口设备（Linux 系统）

### 校准不准确
- 确保校准过程中光线条件稳定
- 按照提示准确点击校准点
- 尝试使用红外发射器对齐助手优化发射器位置

### 力反馈不工作
- 检查力反馈设置是否已启用
- 确认设备硬件支持力反馈功能
- 检查连接线是否正确

## 贡献指南
我们欢迎社区贡献！如果你有任何建议或改进，请提交 Issue 或 Pull Request 到 [GitHub 仓库](https://github.com/TeamOpenFIRE/OpenFIRE-App)。

## 特别鸣谢
* Samuel Ballentyne、Prow7 及其团队，他们在 SAMCO 系统及其衍生产品方面的工作，以及对 OpenFIRE 的支持和构想。
* Odwalla-J、mrkylegp、RG2020 和 lemmingDev 提供的预发布咨询、错误测试和反馈。
* 所有早期的 IR-GUN4ALL 测试人员和 ArcadeForums 用户，他们提供了早期测试和反馈。

## 开源许可证
本程序是自由软件：您可以根据 GNU 通用公共许可证（版本 3 或更高版本）的条款重新分发和/或修改它。

本程序以希望它有用的方式分发，但不提供任何保证；甚至没有默示的适销性或特定用途适用性的保证。有关更多详细信息，请参见 GNU 通用公共许可证。

您应该已经收到了 GNU 通用公共许可证的副本。如果没有，请参见 <https://www.gnu.org/licenses/>。