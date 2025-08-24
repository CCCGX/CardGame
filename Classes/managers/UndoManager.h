#pragma once

#include "cocos2d.h"
#include "models/UndoModel.h"

/**
 * @class UndoManager
 * @brief 撤销管理器类
 * @职责 负责记录游戏中的操作行为（如卡牌点击），提供撤销操作所需的历史记录管理，
 *       通过与UndoModel交互存储和提取操作记录，支持游戏状态的回退功能
 * @使用场景 用于需要支持撤销操作的卡牌游戏中，在玩家执行操作（如点击主牌区或堆叠区卡牌）时记录行为，
 *           当触发撤销指令时提取最近的操作记录，供控制器恢复上一步游戏状态
 */
class UndoManager
{
public:
    /**
     * @brief 构造函数
     * @note 初始化撤销管理器，创建或关联用于存储操作记录的UndoModel实例
     */
    UndoManager();

    /**
     * @brief 记录主牌区卡牌的点击操作
     * @param cardId 被点击的主牌区卡牌ID
     * @note 将主牌区卡牌的点击行为作为操作记录存储到UndoModel中，用于后续撤销操作
     */
    void recordPlayfieldCardClick(int cardId);

    /**
     * @brief 记录堆叠区卡牌的点击操作
     * @param cardId 被点击的堆叠区卡牌ID
     * @note 将堆叠区卡牌的点击行为作为操作记录存储到UndoModel中，用于后续撤销操作
     */
    void recordStackCardClick(int cardId);

    /**
     * @brief 移除并获取最近的一条操作记录
     * @return ActionRecord 最近的操作记录对象，包含操作类型及相关卡牌ID等信息
     * @note 用于撤销操作时提取最近的行为记录，以便控制器根据记录恢复游戏状态
     */
    ActionRecord removeActionRecord();

private:
    /**
     * @brief 撤销数据模型指针
     * @用途 用于存储和管理所有操作记录的底层数据结构，提供操作记录的增删存取功能
     */
    UndoModel* _undoModel;
};