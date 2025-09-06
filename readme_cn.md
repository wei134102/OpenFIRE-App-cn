#### 喜欢我们的作品？[记得支持开发者！](https://github.com/TeamOpenFIRE/.github/blob/main/profile/README.md)

![header](OFA_header.png)
# OpenFIRE 桌面配置应用
### [OpenFIRE 光枪系统](https://github.com/TeamOpenFIRE/OpenFIRE-Firmware)的参考配置工具，使用 Qt 和 C++ 编写。

## 功能特点
- **跨平台 Qt 应用**，可在 Linux、Windows 桌面和 Raspberry Pi 系统上运行。
- **简单易用**：插入设备并从下拉菜单中选择枪，即可开始配置！
- 查看和管理当前引脚布局，启用/禁用自定义映射，管理其他可调设置（如 FFB 和 I2C 外设等），所有操作均可实时进行。
- 使用直观的交互式全屏界面校准任何已加载的配置文件，实现简单而准确的校准。
- 使用红外发射器对齐助手查看发射器的理想对齐方式。
- 预览所有支持的开发板布局，包括它们的功能（I2C 类型和通道等），使构建 OpenFIRE 光枪更加容易。
- 还可用作数字和模拟输入、力反馈设备、摄像头等的测试工具。

## 运行方法
运行 `OpenFIREapp`。在主窗口中，可用设备将定期刷新并显示在 *COM 端口* 下拉框中。即使当前没有插入任何设备，您仍然可以通过从 *帮助* 应用菜单按钮中选择 *查看兼容板* 来访问 *红外发射器对齐助手* 或预览支持的开发板及其默认布局。

当连接开发板时，选择对应于您的微控制器的端口将其连接到应用程序（如果有当前连接的开发板，将先断开连接）。无法与应用程序正确通信的设备可以重启到其引导加载程序，以便它们可以更新到最新的可用固件版本。

![inline](OFA_screens.png)

### Linux 系统
##### 要求：支持 Qt 5.15.X 或更高版本。
- Arch Linux：以下 AUR PKGBUILD 之一：
  - [`openfireapp`](https://aur.archlinux.org/packages/openfireapp)：当前稳定版本
  - [`openfireapp-git`](https://aur.archlinux.org/packages/openfireapp-git)：不稳定版本，直接从源代码构建
- 其他发行版：使用 [发布页面](https://github.com/TeamOpenFIRE/OpenFIRE-App/releases/latest) 中的 `OpenFIRE_App-[x86_64/aarch64].AppImage`
- 确保您的用户属于 `dialout` 组（`# usermod -a -G dialout 用户名`）；启动时如果需要，系统会通知您。更改生效需要注销并重新登录。
  - 如果运行上述命令时出现错误消息 `usermod: group 'dialout' does not exist`，您需要创建该组（`# groupadd dialout`）并重启系统，然后再试一次。

### Windows 系统
##### 要求：Windows 10 及以上版本（仅 64 位）。
- 下载 [最新发布的 zip 文件](https://github.com/TeamOpenFIRE/OpenFIRE-App/releases/latest)。
- 从存档中提取 `OpenFIREapp` 文件夹到系统上最方便的位置 - `OpenFIREapp.exe` 应与 `Qt6Core.dll` 等文件以及 `platforms` 和 `styles` 文件夹位于同一目录。
- 启动 `OpenFIREapp.exe`
##### 虽然 Qt 5 版本的应用程序可以在 Windows 7 上运行，但 RP2040 驱动程序至少需要 Windows 10。尝试强制 Windows 7 兼容会遇到无法预见的问题，这些问题可以通过升级到受支持的操作系统来解决。

## 构建方法
### Linux 系统
#### Arch：需要 `qtX-base` `qtX-serialport` `qtX-svg`（对应您所需的 Qt 版本）。
#### Debian：需要 `build-essential` `cmake` `qttools5-dev` `libqt5serialport5-dev` `libqt5svg5-dev`（Qt5 或 Qt6 库均可）
- 克隆仓库：
  ```
  git clone https://github.com/TeamOpenFIRE/OpenFIRE-App
  ```
- 设置构建目录：
  ```
  cd OpenFIRE-App
  mkdir build && cd build
  cmake .. -DCMAKE_BUILD_TYPE=Release
  ```
  * CMake 构建脚本将使用最新可用的 Qt 版本的开发头文件进行构建。如果系统已安装 Qt6，但您想使用 Qt5 构建，可在构建选项中添加 `-DOFAPP_QT_VERSION=Qt5`。
- 编译：
  ```
  make
  ```
- 运行：
  ```
  ./OpenFIREapp
  ```
### Windows 系统
#### 需要从 Qt 安装向导/维护工具的存档部分安装 Qt 5.15.2，它包含所有需要的附加组件
#### Qt 6.x 需要为该版本安装相应的 SerialPort 扩展
- 项目可以在 Qt Creator IDE 中打开，前提是系统已安装适用于其所需版本/环境的适当 Qt 库。或者，它应该可以通过 CMake 在 mingw-w64 环境中构建，但这尚未经过充分测试以确认。

## 特别鸣谢
* Samuel Ballentyne、Prow7 及其团队，他们在 SAMCO 系统及其衍生产品方面的工作，以及对 OpenFIRE 的支持和构想。
* Odwalla-J、mrkylegp、RG2020 和 lemmingDev 提供的预发布咨询、错误测试和反馈。
* 所有早期的 IR-GUN4ALL 测试人员和 ArcadeForums 用户，他们提供了早期测试和反馈。

## 开源许可证
本程序是自由软件：您可以根据 GNU 通用公共许可证（版本 3 或更高版本）的条款重新分发和/或修改它。

本程序以希望它有用的方式分发，但不提供任何保证；甚至没有默示的适销性或特定用途适用性的保证。有关更多详细信息，请参见 GNU 通用公共许可证。

您应该已经收到了 GNU 通用公共许可证的副本。如果没有，请参见 <https://www.gnu.org/licenses/>。