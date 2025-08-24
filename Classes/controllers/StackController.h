#pragma once

class PlayFieldView;

/**
 * @class StackController
 * @brief 卡牌堆叠控制器类
 * @职责 负责管理卡牌堆叠区域的逻辑控制，包括初始化视图、处理卡牌点击与撤销操作，
 *       协调游戏数据模型与视图的交互，执行卡牌移动验证及状态更新
 * @使用场景 用于卡牌游戏中堆叠区域（如手牌堆、牌堆）的交互逻辑处理，衔接视图层与数据层
 */
class StackController {
public:
    /**
     * @brief 初始化控制器并获取所需数据
     * @return StackController* 成功初始化的控制器实例，失败返回nullptr
     */
    static StackController* init();

    /**
     * @brief 初始化关卡场景中的堆叠区域视图
     * @param playFieldView 游戏主视图指针，用于关联控制器与视图
     */
    void initView(PlayFieldView* playFieldView);

    /**
     * @brief 处理卡牌点击事件
     * @param sender 事件发送者（通常为被点击的卡牌视图）
     * @note 验证卡牌是否可移动，若可移动则调用游戏管理器执行移动操作，
     *       更新数据模型并通知视图播放相应动画
     */
    void handleCardClick(Ref* sender);

    /**
     * @brief 处理撤销操作点击事件
     * @param sender 事件发送者（通常为撤销按钮）
     * @note 触发撤销管理器执行撤销逻辑，恢复上一步操作状态并更新视图
     */
    void handleUndoClick(Ref* sender);

    /**
     * @brief 设置游戏管理器
     * @param gameManager 游戏管理器的智能指针，用于访问游戏核心数据与逻辑
     */
    void setGameManager(std::shared_ptr<GameManager> gameManager);

    /**
     * @brief 设置撤销管理器
     * @param undoManager 撤销管理器的智能指针，用于处理撤销相关操作
     */
    void setUndoManager(std::shared_ptr<UndoManager> undoManager);

private:
    /**
     * @brief 游戏主视图的智能指针
     * @用途 用于与视图层交互，如更新卡牌显示、触发动画等
     */
    std::shared_ptr<PlayFieldView> _playFieldView;

    /**
     * @brief 游戏管理器的智能指针
     * @用途 用于访问和修改游戏数据模型，执行核心游戏逻辑
     */
    std::shared_ptr<GameManager>_gameManager;

    /**
     * @brief 撤销管理器的智能指针
     * @用途 用于处理撤销操作，管理游戏状态的历史记录
     */
    std::shared_ptr<UndoManager> _undoManager;
};