#pragma once
#include "views/GameScene.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "models/GameModel.h"
#include "managers/UndoManager.h"
#include "managers/GameManager.h"
#include <memory>
#include "PlayFieldController.h"
#include "StackController.h"

using namespace std;

/**
 * @class GameController
 * @brief 游戏总控制器类
 * @职责 负责统筹游戏的整体流程，包括初始化游戏数据、创建并协调各子控制器（如PlayFieldController、StackController），
 *       管理游戏的启动与核心逻辑调度，是连接游戏模型、视图与子控制器的中枢
 * @使用场景 作为整个游戏的总控组件，在游戏启动时初始化各模块，协调不同区域的逻辑交互，确保游戏流程正常运行
 */
class GameController
{
public:
    /**
     * @brief 启动游戏
     * @param levelID 关卡ID，用于指定当前启动的游戏关卡
     * @note 负责游戏的整体启动流程，包括加载关卡数据、初始化控制器和视图等
     */
    static void startGame(int levelID);//GameScene* gameScene

    /**
     * @brief 初始化游戏控制器
     * @param levelID 关卡ID，用于初始化对应关卡的游戏数据和场景
     * @return bool 初始化成功返回true，否则返回false
     */
    virtual bool init(int levelID);

private:
    /**
     * @brief 游戏数据模型的智能指针
     * @用途 存储和管理游戏的核心数据（如关卡信息、卡牌状态等），供各控制器访问和修改
     */
    shared_ptr<GameModel> _gameModel;

    /**
     * @brief 游戏主区域控制器的智能指针
     * @用途 管理游戏主区域的逻辑交互，如卡牌放置、区域内操作响应等
     */
    shared_ptr<PlayFieldController> _playFieldController;

    /**
     * @brief 卡牌堆叠控制器的智能指针
     * @用途 管理卡牌堆叠区域（如手牌堆、牌堆）的逻辑交互，如卡牌抽取、堆叠操作等
     */
    shared_ptr<StackController> _stackController;

    /**
     * @brief 游戏管理器的智能指针
     * @用途 处理游戏的核心业务逻辑（如规则校验、状态更新等），为控制器提供逻辑支持
     */
    shared_ptr<GameManager> _gameManager;

    /**
     * @brief 撤销管理器的智能指针
     * @用途 管理游戏操作的历史记录，支持撤销功能，协调各控制器的撤销逻辑
     */
    shared_ptr<UndoManager> _undoManager;
};