#pragma once
#include "cocos2d.h"

/**
 * @class GameScene
 * @brief 游戏主场景类，继承自cocos2d::Scene
 * @职责 负责游戏核心玩法界面的展示与管理 主要创建游戏背景。
 */
class GameScene : public cocos2d::Scene
{
public:
    /**
     * @brief 静态工厂方法，创建游戏主场景实例
     * @return cocos2d::Scene* 成功创建的游戏场景实例，失败返回nullptr
     */
    static cocos2d::Scene* createGameScene();

    /**
     * @brief 初始化游戏主场景
     * @return bool 初始化成功返回true，否则返回false
     * @note 重写cocos2d::Scene的init方法，用于初始化场景中的游戏元素、视图组件及事件监听
     */
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};
