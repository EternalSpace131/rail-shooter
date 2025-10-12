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