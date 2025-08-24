#pragma once
#include "cocos2d.h"
#include "CardView.h"
#include <vector>
#include <stack>
#include <map>
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"
#include "configs/LevelConfig.h"
#include <memory>

USING_NS_CC;
/**

@class PlayFieldView
@brief 游戏主区域视图类，继承自 cocos2d::Node
@职责 负责管理游戏中所有卡牌视图的创建、布局、动画展示及交互反馈，
包括主游戏区、堆叠区、手牌区和撤销操作视图的初始化与更新，
并通过控制器回调处理用户操作（如卡牌匹配、移动、撤销等）
@使用场景 用于卡牌类游戏的主界面展示，衔接游戏逻辑控制器与卡牌视图，
实现视图层与控制层的分离，响应并可视化展示游戏核心操作
*/
class PlayFieldView : public cocos2d::Node
{
public:
    /*
    @brief 工厂方法，创建 PlayFieldView 实例并完成初始化
    @return PlayFieldView* 成功创建的实例指针，失败返回 nullptr
    */
    static PlayFieldView* createGameView();
    /*
    @brief 工厂方法，创建 PlayFieldView 实例并完成初始化
    @return PlayFieldView* 成功创建的实例指针，失败返回 nullptr
    */
    virtual bool init();
    /*
    @brief 初始化游戏主区域的卡牌视图
    @param cards 卡牌配置列表，包含所有需在主区域展示的卡牌数据（ID、样式等）
    @param pPlayFieldController 主游戏区域控制器指针，用于注册视图回调（如卡牌点击事件）
    @note 该方法会根据卡牌配置创建对应的 CardView，并绑定控制器的交互逻辑
    */
    void initPlayFieldView(std::vector<CardConfig>& cards,PlayFieldController * pPlayFieldController);
    /**
    @brief 初始化堆叠区的卡牌视图
    @param cards 堆叠区卡牌配置列表
    @param pStackController 堆叠区控制器指针，用于处理堆叠区卡牌的交互逻辑
    */
    void initStackView(std::vector<CardConfig>& cards, StackController* pStackController);

    /**
    @brief 初始化手牌区的卡牌视图
    @param cards 手牌区卡牌配置列表
    @param pStackController 手牌区控制器指针，用于处理手牌的选择、移动等交互
    */
    void initHandView(std::vector<CardConfig>& cards, StackController* pStackController);

    /**
    @brief 初始化撤销操作视图（如撤销按钮或相关提示视图）
    @param pStackController 撤销操作对应的控制器指针，用于绑定撤销操作的回调
    */
    void initUndoView(StackController*pStackController);

    /**
    @brief 播放卡牌匹配成功的动画
    @param cardID 匹配成功的卡牌 ID，用于定位对应的 CardView
    */
    void playMatchAnimation(int cardID);

    /**
    @brief 播放卡牌移动到目标位置的动画
    @param cardID 移动的卡牌 ID，用于定位对应的 CardView
    */
    void playMoveToAnimation(int cardID);

    /**
    @brief 播放卡牌操作失败的动画（如错误匹配、无效移动）
    @param cardID 操作失败的卡牌 ID，用于定位对应的 CardView
    */
    void playFalseAnimation(int cardID);
    /**
    @brief 播放撤销操作的动画（恢复到上一步状态）
    */
    void playUndoAnimation();

private:
    /**

    @brief 卡牌 ID 到 CardView 的映射表
    @用途 通过卡牌唯一 ID 快速查找对应的视图实例，用于动画触发、状态更新等操作
    */
    std::map<int, CardView*> _cardIDtoCardView;

    /**
    @brief 卡牌 ID 到初始位置的映射表
    @用途 存储卡牌的初始坐标，用于动画中位置恢复或移动起点计算
    */
    std::map<int, Vec2> _CardPrePos;

    /**
    @brief 手牌区卡牌 ID 的栈结构
    @用途 记录当前手牌区的卡牌顺序（栈顶为最新加入的卡牌），控制手牌展示层级
    */
    std::stack<int> _handCardID;

    /**
    @brief 所有卡牌的配置数据列表
    @用途 缓存当前场景中所有卡牌的基础配置，供视图初始化和状态更新使用
    */
    std::vector<CardConfig> _cards;

    /**
    @brief 主游戏区域控制器的智能指针
    @用途 持有控制器引用，用于视图与主游戏逻辑的交互（如触发卡牌匹配校验）
    */
    std::shared_ptr<PlayFieldController> _playFieldController;

    /**
    @brief 堆叠区 / 手牌区控制器的智能指针
    @用途 持有控制器引用，用于处理堆叠区和手牌区的卡牌交互逻辑
    */
    std::shared_ptr<StackController> _stackFieldController;

    /**
    @brief 堆叠区的初始位置坐标
    @用途 定义堆叠区卡牌在屏幕中的默认布局位置
    */
    Vec2 _stackPos = Vec2(250, 300);

    /**
    @brief 手牌区的初始位置坐标
    @用途 定义手牌区卡牌在屏幕中的默认布局位置
    */
    Vec2 _handPos = Vec2(800, 300);

    /**
    @brief 手牌区卡牌的 Z 轴层级基准值
    @用途 控制手牌区卡牌的显示层级，避免后加入的卡牌被遮挡（通过调整 siblingIndex 实现）
    */
    int _handZorder = 1;
};