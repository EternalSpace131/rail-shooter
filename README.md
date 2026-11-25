# Rail Shooter Game (轨道射击游戏)

> 我们的征途是星辰大海... 和及格分数！🚀

这是一个使用 C++17、Qt 6 Widgets 和 Qt Multimedia 制作的竖屏射击课程项目。玩家在底部左右移动并自动射击，迎战不断出现的敌人和 Boss。游戏包含奖励箱、随机强化、能量大招、角色图鉴和本地得分排行榜。

## 构建与运行

需要 CMake 3.20 或更高版本、支持 C++17 的编译器，以及包含 Core、Gui、Widgets、Multimedia 模块的 Qt 6 桌面开发套件。Qt 和编译器需要使用匹配的套件。

在 Qt Creator 中打开根目录的 `CMakeLists.txt`，选择桌面 Kit 并配置项目，然后按 `Ctrl+R` 构建并运行。

使用命令行时，先在 Qt 的 MinGW 构建环境中准备好 CMake 和 Ninja，再从仓库根目录执行以下命令。请将 Qt 路径替换为本机的安装位置。

```bat
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/Qt/6.9.0/mingw_64"
cmake --build build --parallel 4
build\RailShooterGame.exe
```

图片和音频通过 `res.qrc` 打包到程序中。首次使用排行榜时，会在可执行文件旁创建 `ranking.json`，保存得分时需要该目录可写。

## 操作

| 操作 | 方法 |
| --- | --- |
| 进入游戏 | 主菜单点击开始按钮 |
| 左右移动 | `A` / `D` 或方向键 `←` / `→` |
| 射击 | 自动向上发射 |
| 释放大招 | 得分积攒满能量后按 `E` |
| 获得强化 | 击破奖励箱，随机提升射速、伤害或获得限时散射 |
| 查看图鉴或排行榜 | 从主菜单进入相应页面 |
| 保存本局得分 | 点击游戏中的返回按钮，或关闭游戏窗口 |


## 📂 文件结构 (Folder Structure)

我们采用按功能模块组织代码的结构，以保持项目清晰：

```text
RailShooterGame/
│
├── .gitignore         
├── CMakeLists.txt      #修改和添加.cpp/.h文件时不需要修改cmake，保证添加在src里就可以
├── README.md           
│
├── assets/             # 存放所有非代码资源
│   ├── images/
│   └── sounds/
├── data/               # 排行榜初始资源
├── res.qrc             # Qt 资源清单
│
└── src/                # 存放所有源代码，.cpp和.h文件都存放在这里
    ├── main.cpp
    ├── Core/           # 游戏核心引擎代码
    ├── Entities/       # 游戏实体 (玩家、敌人等)
    ├── GameLogic/      # 游戏逻辑
    └── Manager/        # 实体、音频和排行榜管理器

```

---

## 🤝 协作流程 (Contribution Workflow)

为了保证代码质量和项目稳定，请所有成员严格遵守以下流程：

> **黄金规则：** 任何时候都不要在 `main` 分支上直接进行任何开发！开始工作前，请务必从 `main` 创建你自己的分支。

1.  **从 `main` 创建新分支**: 在开始一个新任务前，务必从最新的 `main` 分支创建你自己的特性分支。
    **分支命名建议**: `feature/你的名字/功能描述`，例如 `feature/zhangsan/create-window`。
2.  **在新分支上开发**: 在你自己的分支上进行编码和调试，可以随意 `commit`。
3.  **推送分支并创建合并请求**: 当你完成一个功能后，将你的分支 `push` 到 GitHub，并创建一个指向 `main` 分支的 **Pull Request**。
4.  **代码审查**: 在合并请求中，`@`组长或其他相关同学进行代码审查。审查通过后，将由组长负责将代码合并到 `main` 分支。


---
