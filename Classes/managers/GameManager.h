#pragma once

#include "cocos2d.h"
#include "models/GameModel.h"

/**
 * @class GameManager
 * @brief 游戏管理器类
 * @职责 负责游戏的整体流程控制，包括关卡启动、游戏数据管理及卡牌交互验证，
 *       作为游戏核心逻辑的调度中心，协调模型层与控制层的交互
 * @使用场景 贯穿整个游戏生命周期，供controller调用，配合gamemodel初始化关卡、获取游戏数据、验证玩家操作合法性等，
 *           是连接游戏数据模型与控制器的关键组件
 */
class GameManager
{
public:
    /**
     * @brief 构造函数
     * @note 初始化游戏管理器，准备管理游戏状态和数据
     */
    GameManager();

    /**
     * @brief 启动指定关卡
     * @param levelId 要启动的关卡ID
     * @return bool 关卡启动成功返回true，失败返回false
     */
    bool startLevel(int levelId);

    /**
     * @brief 异步启动指定关卡并设置加载完成回调
     * @param levelId 要启动的关卡ID
     * @param loadDoneCallBack 关卡加载完成后的回调函数，参数为加载成功状态（true为成功）
     */
    void startLevelWithCallBack(int levelId, std::function<void(bool)> loadDoneCallBack);

    /**
     * @brief 获取当前游戏数据模型
     * @return GameModel* 游戏数据模型指针，用于访问和修改游戏状态数据
     */
    GameModel* getGameModel();

    /**
     * @brief 验证指定卡牌是否可被点击（是否符合交互条件）
     * @param cardId 要验证的卡牌ID
     * @return bool 可点击返回true，不可点击返回false
     */
    bool canClick(int cardId);

private:
    /**
     * @brief 游戏数据模型指针
     * @用途 存储和管理当前游戏的所有状态数据（如卡牌位置、关卡进度等），供管理器访问和修改
     */
    GameModel* _gameModel;

    /**
     * @brief 关卡加载完成的回调函数
     * @用途 存储关卡加载完成后需要执行的回调逻辑，用于通知调用者加载结果
     */
    std::function<void(bool)> _loadDoneCallBack;
};