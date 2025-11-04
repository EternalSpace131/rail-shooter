//
// Created by Morgan on 25-10-23.
//
#include"EnemySpec.h"

std::vector<EnemySpec> getEnemyBook(){
    static const std::vector<EnemySpec> kEnemyBook = {
        {EnemyKind::Mowan,      "魔丸",        90,  3, 1, {28*4,24*4},  50, false,false, false,0,1.0,0,
             {":/assets/images/character/naughty/naughty1.png",":/assets/images/character/naughty/naughty2.png",
            ":/assets/images/character/naughty/naughty3.png",":/assets/images/character/naughty/naughty4.png",
            ":/assets/images/character/naughty/naughty5.png",":/assets/images/character/naughty/naughty6.png",
            ":/assets/images/character/naughty/naughty7.png",":/assets/images/character/naughty/naughty8.png",
            },
         50,20
        },
        {EnemyKind::Kobe,      "牢大(冰红茶)", 80,  7, 0, {30*4,26*4},  20, true,false,  false,0,1.0,0,
         {":/assets/images/character/man/man.png",
        },
         25,30
        }, // 精英，死亡可做烟雾特效
        {EnemyKind::Wujing,     "吴京(彩蛋)",  85,  5, 1, {28*4,24*4},  10, false,false, false,0,1.0,0,
         {":/assets/images/character/wujing/wujing1.png",":/assets/images/character/wujing/wujing2.png",
            ":/assets/images/character/wujing/wujing3.png",":/assets/images/character/wujing/wujing4.png"
            },
            20,50
        }, // 彩蛋，无实际效果
        {EnemyKind::Penguin,    "马达加斯加企鹅", 70, 6, 1, {30*4,26*4}, 12, false,false, true,120,1.0, +1,
         {":/assets/images/character/penguin/penguin1.png",":/assets/images/character/penguin/penguin2.png"
        },
            25,70
        } ,// 光环：+防御

        {EnemyKind::Xiaoyan, "刘晓燕",      70, 6, 1, {30*4,26*4}, 12, false,false, true,120,1.2,  0,
         {":/assets/images/character/xiaoyan/xiaoyan1.png",":/assets/images/character/xiaoyan/xiaoyan2.png",
        ":/assets/images/character/xiaoyan/xiaoyan3.png",":/assets/images/character/xiaoyan/xiaoyan4.png",
        ":/assets/images/character/xiaoyan/xiaoyan5.png",
        },
         25,70
        },  // 光环：+速度
        {EnemyKind::Songhao,    "宋浩(彩蛋)",  80,  6, 1, {30*4,26*4},  10, false,false, false,0,1.0,0,
         {":/assets/images/character/songhao/songhao1.png",":/assets/images/character/songhao/songhao2.png"
        },
            20,50
        },
        {EnemyKind::NJUboss,    "nju(BOSS)",   55, 40, 3, {44*4,40*4},   2, false,true,  false,0,1.0,0,
         {":/assets/images/character/nju/nju.png",
        },
            5,100
        },
    };
    return kEnemyBook;
}
