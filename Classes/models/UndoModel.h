#pragma once

#include "cocos2d.h"

/**
 * @brief 操作类型枚举
 * @说明 定义了游戏中可记录的操作类型，用于标识不同的用户交互行为
 */
enum Action
{
    UNKNOWN_ACTION = -1,    // 未知操作（默认值）
    CLICK_PLAY_FIELD,       // 点击主游戏区卡牌的操作
    CLICK_STACK_CARD        // 点击堆叠区卡牌的操作
};

/**
 * @class ActionRecord
 * @brief 操作记录类
 * @职责 存储单次用户操作的关键信息，包括操作类型和涉及的卡牌ID，
 *       作为撤销功能的基础数据单元，记录操作的核心要素
 * @使用场景 用于撤销管理器记录用户操作历史，当需要撤销时，通过该类实例还原操作前状态
 */
class ActionRecord
{
public:
    /**
     * @brief 操作类型
     * @用途 标识当前记录的操作属于哪种行为（如点击主游戏区或堆叠区卡牌）
     */
    ::Action action = UNKNOWN_ACTION;

    /**
     * @brief 涉及的卡牌ID
     * @用途 记录该操作所关联的卡牌唯一标识符，用于精准还原操作对象
     */
    int cardId = -1;
};

/**
 * @class UndoModel
 * @brief 撤销数据模型类（单例模式）
 * @职责 负责存储和管理所有用户操作的历史记录，提供操作记录的入栈、出栈功能，
 *       作为撤销功能的底层数据存储中心，维护操作历史的有序性
 * @使用场景 被撤销管理器调用，用于持久化存储操作记录，支持游戏状态的回退功能，
 *           确保撤销操作能准确获取最近的历史记录
 */
class UndoModel
{
public:
    /**
     * @brief 获取单例实例
     * @return UndoModel* 全局唯一的UndoModel实例指针
     */
    static UndoModel* getInstance();

    /**
     * @brief 将操作记录入栈（添加到历史记录）
     * @param record 要存储的操作记录对象
     */
    void pushActionRecord(const ActionRecord& record);

    /**
     * @brief 将最近的操作记录出栈（获取并移除最近记录）
     * @return ActionRecord 最近一次操作的记录对象
     */
    ActionRecord popActionRecord();

private:
    /**
     * @brief 私有构造函数（单例模式，禁止外部实例化）
     */
    UndoModel() = default;

    /**
     * @brief 私有析构函数（单例模式，控制实例生命周期）
     */
    ~UndoModel();

private:
    /**
     * @brief 单例实例指针
     * @用途 存储全局唯一的UndoModel实例，确保整个游戏中只有一个操作记录管理器
     */
    static UndoModel* s_instance;

    /**
     * @brief 操作记录栈
     * @用途 以栈结构存储操作记录，保证"后进先出"的顺序，符合撤销操作的逻辑（先撤销最近操作）
     */
    std::stack<ActionRecord> _actionStack;
};