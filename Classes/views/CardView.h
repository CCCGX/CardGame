#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

/**
 * @brief 卡牌花色类型枚举
 * @说明 定义了卡牌的四种花色及无效状态，用于区分不同花色的卡牌
 */
enum CardSuitType
{
    CST_NONE = -1,       // 无效花色
    CST_CLUBS,           // 梅花
    CST_DIAMONDS,        // 方块
    CST_HEARTS,          // 红桃
    CST_SPADES,          // 黑桃
    CST_NUM_CARD_SUIT_TYPES  // 花色类型总数（用于边界判断）
};

/**
 * @brief 卡牌面值类型枚举
 * @说明 定义了卡牌的13种面值（A到K）及无效状态，用于区分不同面值的卡牌
 */
enum CardFaceType
{
    CFT_NONE = -1,       // 无效面值
    CFT_ACE,             // A
    CFT_TWO,             // 2
    CFT_THREE,           // 3
    CFT_FOUR,            // 4
    CFT_FIVE,            // 5
    CFT_SIX,             // 6
    CFT_SEVEN,           // 7
    CFT_EIGHT,           // 8
    CFT_NINE,            // 9
    CFT_TEN,             // 10
    CFT_JACK,            // J
    CFT_QUEEN,           // Q
    CFT_KING,            // K
    CFT_NUM_CARD_FACE_TYPES  // 面值类型总数（用于边界判断）
};

/**
 * @class CardView
 * @brief 卡牌视图类，继承自cocos2d::ui::Widget
 * @职责 负责单个卡牌的视觉展示（包括花色、面值、背景等），处理卡牌的点击事件，
 *       提供卡牌属性（花色、面值、ID）的访问接口
 * @使用场景 用于各类卡牌游戏中，作为可视化的卡牌元素，展示卡牌信息并响应玩家交互
 */
class CardView : public cocos2d::ui::Widget
{
public:
    /**
     * @brief 创建卡牌视图实例
     * @param suitType 卡牌花色类型（如梅花、红桃等）
     * @param faceType 卡牌面值类型（如A、10、K等）
     * @param cardId 卡牌唯一标识符，用于区分不同卡牌
     * @return CardView* 成功创建的卡牌实例，失败返回nullptr
     */
    static CardView* createCard(CardSuitType suitType, CardFaceType faceType, int cardId);

    /**
     * @brief 获取卡牌的花色类型
     * @return CardSuitType 卡牌的花色（CST_NONE表示无效）
     */
    CardSuitType getSuitType();

    /**
     * @brief 获取卡牌的面值类型
     * @return CardFaceType 卡牌的面值（CFT_NONE表示无效）
     */
    CardFaceType getFaceType();

    /**
     * @brief 获取卡牌的唯一ID
     * @return int 卡牌的ID值
     */
    int getCardId();

protected:
    /**
     * @brief 创建基础节点（内部使用的辅助方法）
     * @return Node* 创建的节点实例
     */
    static Node* create();

    /**
     * @brief 初始化卡牌视图（重写父类方法）
     * @return bool 初始化成功返回true，否则返回false
     * @note 用于初始化卡牌的视觉元素（背景、花色、数字等）
     */
    virtual bool init() override;

    /**
     * @brief 构造函数（默认）
     */
    CardView() = default;

private:
    /**
     * @brief 卡牌的花色类型
     * @用途 存储并标识当前卡牌的花色，通过getSuitType()方法访问
     */
    CardSuitType _suitType = CardSuitType::CST_NONE;

    /**
     * @brief 卡牌的面值类型
     * @用途 存储并标识当前卡牌的面值，通过getFaceType()方法访问
     */
    CardFaceType _faceType = CardFaceType::CFT_NONE;

    /**
     * @brief 卡牌的背景精灵
     * @用途 展示卡牌的背景图案（如卡牌背面或通用背景）
     */
    cocos2d::Sprite* _background = nullptr;

    /**
     * @brief 卡牌的花色精灵
     * @用途 展示卡牌对应的花色图案（如梅花图标、红桃图标等）
     */
    cocos2d::Sprite* _suit = nullptr;

    /**
     * @brief 卡牌的小面值精灵
     * @用途 通常在卡牌角落展示的小型面值标识（如角落的"A"、"10"等）
     */
    cocos2d::Sprite* _numberSmall = nullptr;

    /**
     * @brief 卡牌的大面值精灵
     * @用途 通常在卡牌中央展示的大型面值标识（如中央的"K"、"Q"等）
     */
    cocos2d::Sprite* _number = nullptr;

    /**
     * @brief 卡牌的唯一标识符
     * @用途 区分不同的卡牌实例，通过getCardId()方法访问
     */
    int _cardId;
};