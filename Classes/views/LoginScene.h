#pragma once

#include "cocos2d.h"
/**
 * @class LoginScene
 * @brief 登录场景类，继承自cocos2d::Scene
 * @职责 负责游戏登录界面的展示与交互逻辑处理，提供用户进入游戏的入口
 * @使用场景 作为游戏启动后的第一个场景，用于玩家登录或直接进入游戏主界面
 */
class LoginScene : public cocos2d::Scene
{
public:
    /**
     * @brief 创建登录场景实例
     * @return cocos2d::Scene* 成功创建的场景实例，失败返回nullptr
     */
    static cocos2d::Scene* createScene();

    /**
     * @brief 初始化登录场景
     * @return bool 初始化成功返回true，否则返回false
     * @note 重写cocos2d::Scene的init方法，用于初始化场景中的UI元素和事件监听
     */
    virtual bool init();

    /**
     * @brief 关卡按钮点击事件 创建gameController
     * @param pSender 事件发送者（通常是被点击的按钮对象）
     * @note 处理关卡选择按钮的点击逻辑，如切换到对应关卡场景
     */
    void LoginScene::levelButtonCallBack(Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(LoginScene);
};
