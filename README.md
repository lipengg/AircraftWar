# AircraftWar（飞机大战）

基于 cocos2d-x **3.17.2** 的 C++ 竖屏飞行射击游戏，设计分辨率 720x1280。

引擎源码**不随本仓库分发**，通过环境变量 `COCOS2DX_ROOT` 指向本机独立安装的引擎目录。

## 一、引擎安装（所有成员必做）

1. 获取 cocos2d-x 3.17.2 引擎源码，解压到任意目录，例如：

   ```
   D:\DevelopTools\cocos2d-x-3.17.2
   ```

2. 下载引擎第三方预编译依赖（约 141 MB，解压为 `external/` 目录）：

   ```powershell
   cd D:\DevelopTools\cocos2d-x-3.17.2
   python download-deps.py
   ```

   依赖版本为 `v3-deps-158`（见 `external/config.json`），团队必须一致。

3. 运行引擎自带环境配置脚本（自动设置 `COCOS_CONSOLE_ROOT`、`COCOS_X_ROOT` 等变量）：

   ```powershell
   python setup.py
   ```

## 二、环境变量配置

| 变量 | 值示例 | 用途 | 设置方式 |
|------|--------|------|---------|
| `COCOS2DX_ROOT` | `D:\DevelopTools\cocos2d-x-3.17.2` | **本工程构建专用**，必须指向引擎根目录 | 手动设置（见下） |
| `COCOS_X_ROOT` | `D:\DevelopTools` | cocos 命令行查找引擎，值为引擎的**父目录** | `setup.py` 自动设置 |
| `COCOS_CONSOLE_ROOT` | `...\cocos2d-x-3.17.2\tools\cocos2d-console\bin` | cocos 命令行工具 | `setup.py` 自动设置 |
| `NDK_ROOT` | `D:\DevelopTools\ndk\android-ndk-r16b` | Android 原生编译 | `setup.py` 或手动 |
| `ANDROID_SDK_ROOT` | `D:\DevelopTools\android\sdk` | Android 打包 | `setup.py` 或手动 |

设置 `COCOS2DX_ROOT`（用户级，永久生效）：

```powershell
setx COCOS2DX_ROOT "D:\DevelopTools\cocos2d-x-3.17.2"
```

> 注意：`setx` 对**已打开**的终端 / Visual Studio / Gradle Daemon 不生效，设置后需重新打开。
> `COCOS_X_ROOT` 与 `COCOS2DX_ROOT` 语义不同：前者是引擎父目录（官方 cocos 工具用），后者是引擎根目录（本工程构建脚本用），不可混用。

## 三、平台编译要求与步骤

### Windows（Win32）

- **Visual Studio 2017**（平台工具集 v141）。引擎工程文件的工具集条件仅覆盖 v120/v140/v141，用 VS2022 打开会报 `MSB8020: 找不到 v100 生成工具`
- 编译步骤：

  1. 打开 `proj.win32/AircraftWar.sln`
  2. 直接 **生成解决方案（F7）**——引擎（librecast → libSpine → libcocos2d）由项目引用自动按序构建，无需手动先编引擎
  3. 产物输出到 `proj.win32/Debug.win32/`（Release 同理）；F5 调试工作目录已配置为 `Resources/`

### Android

- **NDK r16b**（必须此版本，其他版本与引擎 C++11 STL 配置不兼容）
- SDK API 28（compile/target），minSdk 16
- Python（cocos 命令行依赖）
- 本机路径写入 `proj.android/local.properties`（该文件不入库）：

  ```properties
  sdk.dir=D\:\\DevelopTools\\android\\sdk
  ndk.dir=D\:\\DevelopTools\\ndk\\android-ndk-r16b
  ```

- 编译命令（在仓库根目录执行）：

  ```powershell
  cocos compile -p android --ap android-28 --app-abi arm64-v8a
  ```

  - APK 输出到 `bin/debug/android/`
  - `--app-abi` 是完整参数名，cocos console 不识别 `--abi` 简写
  - 默认 ABI 见 `proj.android/gradle.properties` 的 `PROP_APP_ABI`，命令行参数会覆盖它
  - 设置/修改环境变量后如构建异常，先执行 `gradlew --stop` 清理旧 Gradle Daemon 再编译

### CMake（可选，Linux / 命令行构建）

```powershell
mkdir build; cd build
cmake .. -G "Visual Studio 15 2017"   # 或其他生成器
```

## 四、构建配置说明

工程内所有构建入口遵循统一规则：**优先读取 `COCOS2DX_ROOT`，未设置或路径无效时回退到工程根目录下的 `cocos2d/`**（如存在）。

| 入口 | 位置 |
|------|------|
| Win32 | `proj.win32/AircraftWar.vcxproj` 的 `EngineDir` 属性 |
| CMake | `CMakeLists.txt` 的 `COCOS2DX_ROOT_PATH` |
| Android (gradle/cmake) | `proj.android/settings.gradle`、`proj.android/app/build.gradle` |

引擎与游戏通过 `ProjectReference` 关联：`librecast`、`libSpine`（无依赖）→ `libcocos2d` → `AircraftWar`，输出目录统一为 `$(SolutionDir)$(Configuration).win32\`。

## 五、常见问题

| 现象 | 原因与解决 |
|------|-----------|
| `LNK1104: 无法打开文件 "libcocos2d.lib"` | 未设 `COCOS2DX_ROOT` 或设错（应指向引擎根目录，不是其父目录）；重开 VS 后再编译 |
| `LNK1181: 无法打开输入文件 "libSpine.lib"` | 引擎子项目未参与构建；用"生成解决方案"而不是只生成游戏项目 |
| `MSB8020: 找不到 v100 生成工具` | 使用了 VS2022/更高版本 MSBuild，请改用 VS2017 |
| Android CMake 仍引用旧路径 | Gradle Daemon 缓存了旧环境：`gradlew --stop` 后重新编译 |
| cocos 命令找不到引擎 | 检查 `COCOS_X_ROOT` 是否为引擎的**父目录**，且父目录下存在 `cocos2d-x-3.17.2` 版本目录 |
