#pragma once
#include "views/GameScene.h"
#include "cocos2d.h"
#include <memory>
#include "managers/GameManager.h"
#include "managers/UndoManager.h"

USING_NS_CC;

class PlayFieldView;

/**
 * @class PlayFieldController
 * @brief 游戏主区域控制器类
 * @职责 负责游戏主区域的逻辑控制，包括初始化视图、处理卡牌点击事件，
 *       协调游戏数据模型与视图的交互，验证卡牌移动合法性并执行相应操作
 * @使用场景 作为游戏主区域（如战场、牌桌中央区域）的核心控制器，衔接视图展示与业务逻辑
 */
class PlayFieldController {
public:
    /**
     * @brief 初始化控制器实例
     * @return PlayFieldController* 成功初始化的控制器实例，失败返回nullptr
     */
    static PlayFieldController* init();

    /**
     * @brief 初始化游戏主区域视图
     * @param playFieldView 游戏主区域视图指针，用于关联控制器与视图层
     */
    void initView(PlayFieldView* playFieldView);

    /**
     * @brief 处理卡牌点击事件
     * @param sender 事件发送者（通常为被点击的卡牌视图）
     * @note 验证卡牌是否可移动，若合法则通过游戏管理器执行移动操作，
     *       更新数据模型并通知视图播放对应动画
     */
    void handleCardClick(Ref* sender);

    /**
     * @brief 设置游戏管理器
     * @param gameManager 游戏管理器的智能指针，用于访问和修改游戏核心数据
     */
    void setGameManager(std::shared_ptr<GameManager> gameManager);

    /**
     * @brief 设置撤销管理器
     * @param undoManager 撤销管理器的智能指针，用于支持游戏操作的撤销功能
     */
    void setUndoManager(std::shared_ptr<UndoManager> undoManager);

private:
    /**
     * @brief 游戏主区域视图的智能指针
     * @用途 用于控制视图层的展示逻辑，如更新卡牌位置、触发动画等
     */
    std::shared_ptr<PlayFieldView> _playFieldView;

    /**
     * @brief 游戏管理器的智能指针
     * @用途 用于调用游戏核心逻辑，如卡牌移动、状态更新等数据操作
     */
    std::shared_ptr<GameManager>_gameManager;

    /**
     * @brief 撤销管理器的智能指针
     * @用途 用于处理游戏操作的撤销功能，管理操作历史记录
     */
    std::shared_ptr<UndoManager> _undoManager;
};