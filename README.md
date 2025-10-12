# Rail Shooter Game (轨道射击游戏)

> 我们的征途是星辰大海... 和及格分数！🚀


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
│   ├── sounds/
│   └── fonts/
│
└── src/                # 存放所有源代码，.cpp和.h文件都存放在这里
    ├── main.cpp
    ├── Core/           # 游戏核心引擎代码
    ├── Entities/       # 游戏实体 (玩家、敌人等)
    ├── GameLogic/      # 游戏逻辑
    └── Managers/       # 管理器 (资源、碰撞等)

```

---

## 🤝 协作流程 (Contribution Workflow)

为了保证代码质量和项目稳定，请所有成员严格遵守以下流程：

> **黄金规则：** 任何时候都不要在 `main` 分支上直接进行任何开发！开始工作前，请务必从 `main` 创建你自己的分支。

1.  **从 `main` 创建新分支**: 在开始一个新任务前，务必从最新的 `main` 分支创建你自己的特性分支。
    **分支命名建议**: `feature/你的名字/功能描述`，例如 `feature/zhangsan/create-window`。
2.  **在新分支上开发**: 在你自己的分支上进行编码和调试，可以随意 `commit`。
3.  **推送分支并创建合并请求**: 当你完成一个功能后，将你的分支 `push` 到 GitLab，并在 GitLab 页面上创建一个指向 `main` 分支的 **合并请求 (Merge Request)**。
4.  **代码审查**: 在合并请求中，`@`组长或其他相关同学进行代码审查。审查通过后，将由组长负责将代码合并到 `main` 分支。


---