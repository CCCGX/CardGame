#pragma once

#include "cocos2d.h"
#include "views/CardView.h"


/**
 * @class CardConfig
 * @brief 单个卡牌的配置信息类
 * @职责 存储单张卡牌的核心配置数据，包括唯一标识、点数、花色和初始位置，
 *       作为卡牌初始化的基础数据结构，用于在加载关卡时传递卡牌的初始属性
 * @使用场景 用于关卡配置解析过程中，存储从配置文件中读取的单张卡牌信息，
 *           供controller层控制view层创建CardView实例时使用
 */
class CardConfig
{
public:
    /**
     * @brief 卡牌的唯一标识符
     * @用途 区分不同的卡牌实例，确保在操作和状态更新时能准确识别目标卡牌
     */
    int cardId = -1;

    /**
     * @brief 卡牌的点数类型（A-K）
     * @用途 定义卡牌的面值信息，决定卡牌在游戏中的数值属性和显示样式
     */
    CardFaceType cardFace = CFT_NONE;

    /**
     * @brief 卡牌的花色类型（梅花/方块/红桃/黑桃）
     * @用途 定义卡牌的花色属性，决定卡牌的视觉样式和可能的游戏规则关联
     */
    CardSuitType cardSuit = CST_NONE;

    /**
     * @brief 卡牌在场景中的初始位置坐标
     * @用途 指定卡牌在游戏开始时的初始摆放位置，用于视图层初始化卡牌布局
     */
    NS_CC::Vec2 position = cocos2d::Vec2::ZERO;

    /**
     * @brief 默认构造函数
     */
    CardConfig() = default;
};

/**
 * @class LevelConfig
 * @brief 关卡配置信息类
 * @职责 存储单个关卡的所有卡牌配置数据，包括主牌区和备用牌堆的卡牌列表，
 *       提供配置数据的访问和添加接口，作为关卡初始化的核心数据容器
 * @使用场景 用于加载关卡时，整合该关卡所需的所有卡牌配置信息，
 *           供控制器层初始化游戏场景中的卡牌布局和初始状态
 *           确保卡牌展示符合关卡设计
 *           供gameManager加载关卡使用
 */
class LevelConfig
{
public:
    /**
     * @brief 创建LevelConfig实例（遵循Cocos内存管理规范）
     * @return LevelConfig* 成功创建的实例指针，失败返回nullptr
     */
    static LevelConfig* create();

    /**
     * @brief 获取主牌区卡牌配置列表
     * @return const std::vector<CardConfig>& 主牌区卡牌配置的常量引用，不可修改
     */
    const std::vector<CardConfig>& getPlayfieldConfigs() const;

    /**
     * @brief 向主牌区添加单张卡牌的配置信息
     * @param config 要添加的卡牌配置对象
     */
    void addPlayfieldConfig(const CardConfig& config);

    /**
     * @brief 获取备用牌堆的卡牌配置列表
     * @return const std::vector<CardConfig>& 备用牌堆卡牌配置的常量引用，不可修改
     */
    const std::vector<CardConfig>& getStackConfigs() const;

    /**
     * @brief 向备用牌堆添加单张卡牌的配置信息
     * @param config 要添加的卡牌配置对象
     */
    void addStackConfig(const CardConfig& config);

protected:
    /**
     * @brief 保护的构造函数，确保通过create()方法创建实例（遵循Cocos内存管理规范）
     */
    LevelConfig() = default;

private:
    /**
     * @brief 主牌区卡牌配置列表
     * @用途 存储游戏主区域（如战场、牌桌中央）所有卡牌的初始配置信息
     */
    std::vector<CardConfig> _playfieldConfigs;

    /**
     * @brief 备用牌堆卡牌配置列表
     * @用途 存储备用牌堆（如牌库、手牌堆）所有卡牌的初始配置信息
     */
    std::vector<CardConfig> _stackConfigs;
};