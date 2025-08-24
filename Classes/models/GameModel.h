#pragma once

#include "cocos2d.h"
#include "configs/LevelConfigLoader.h"

/**
 * @brief 游戏数据模型类，负责管理游戏中的卡牌数据、关卡配置及卡牌移动逻辑
 *
 * 该类维护了游戏中三种区域的卡牌状态：游戏区(playfield)、堆叠区(stack)和手牌区(hand)，
 * 并提供了卡牌在不同区域间移动的方法及覆盖关系检测功能。
 */
class GameModel {
public:
	/**
	 * @brief 加载指定关卡的配置数据
	 *
	 * 从关卡配置中加载游戏区、堆叠区和手牌区的初始卡牌配置
	 *
	 * @param levelId 要加载的关卡ID
	 * @return 加载成功返回true，否则返回false
	 */
	bool loadLevel(int levelId);

	/**
	 * @brief 获取游戏区所有卡牌的配置数据
	 *
	 * @return 包含游戏区所有卡牌配置
	 */
	const std::vector<CardConfig> getPlayfieldCards() const;

	/**
	 * @brief 获取堆叠区所有卡牌的配置数据
	 *
	 * @return 包含堆叠区所有卡牌配置量（按堆叠顺序排列）
	 */
	const std::vector<CardConfig> getStackCards() const;

	/**
	 * @brief 获取手牌区所有卡牌的配置数据
	 *
	 * @return 包含手牌区所有卡牌配置（按手牌顺序排列）
	 */
	const std::vector<CardConfig> getHandCards() const;

	/**
	 * @brief 获取堆叠区顶部卡牌的 ID
	 *
	 * @return 堆叠区顶部卡牌的 ID
	 */
	int getStackTopCardId() const;

	/**
	 * @brief 将指定卡牌从手牌区移动到游戏区
	 *
	 * 仅允许移动手牌区顶部的卡牌
	 *
	 * @param cardId 要移动的卡牌ID
	 * @return 移动成功返回true，否则返回false
	 */
	bool moveCardToPlayfield(int cardId);

	/**
	 * @brief 将指定卡牌从手牌区移动到堆叠区
	 *
	 * 仅允许移动手牌区顶部的卡牌
	 *
	 * @param cardId 要移动的卡牌ID
	 * @return 移动成功返回true，否则返回false
	 */
	bool moveCardToStack(int cardId);

	/**
	 * @brief 将指定卡牌移动到手牌区
	 *
	 * 支持从堆叠区顶部或游戏区移动卡牌到手牌区
	 *
	 * @param cardId 要移动的卡牌ID
	 * @return 移动成功返回true，否则返回false
	 */
	bool moveCardToHand(int cardId);

	/**
	 * @brief 检查指定卡牌是否被其他卡牌覆盖
	 *
	 * 仅对游戏区的卡牌有效，若卡牌被至少一张其他卡牌覆盖则返回true
	 *
	 * @param cardId 要检查的卡牌ID
	 * @return 被覆盖返回true，否则返回false
	 */
	bool isCardCovered(int cardId) const;

	/**
	 * @brief 检查指定卡牌是否在游戏区
	 *
	 * @param cardId 要检查的卡牌ID
	 * @return 如果在则返回true，否则返回false
	 */
	bool isCardInPlayfield(int cardId) const;

	/**
	 * @brief 检查指定的游戏区的卡牌的牌面是否与最顶部的手牌牌面连续
	 *
	 * @param cardId 要检查的卡牌ID
	 * @return 如果在则返回true，否则返回false
	 */
	bool checkPlayfieldCardFacesConsecutiveWithHandTopCard(int cardId) const;

	static GameModel* getInstance();

private:
		/**
	 * @brief 默认构造函数
	 *
	 * 使用默认的卡牌锚点(0.5f, 0.5f)和尺寸(182.0f, 282.0f)初始化游戏模型
	 */
	GameModel();

	/**
	 * @brief 带参数的构造函数
	 *
	 * @param cardAnchorPoint 卡牌的锚点位置
	 * @param cardSize 卡牌的尺寸大小
	 */
	GameModel(const NS_CC::Vec2& cardAnchorPoint, NS_CC::Size cardSize);

	~GameModel();

	/**
	 * @brief 计算指定卡牌的轴对齐 bounding box (AABB)
	 *
	 * 根据卡牌的位置、锚点和尺寸计算其碰撞边界
	 *
	 * @param cardConfig 卡牌配置数据
	 * @return 计算得到的AABB矩形
	 */
	NS_CC::Rect getCardAABB(const CardConfig& cardConfig) const;

	/**
	 * @brief 重建游戏区所有卡牌的覆盖拓扑关系
	 *
	 * 重新计算所有卡牌之间的覆盖关系，用于更新卡牌的覆盖状态
	 */
	void rebuildPlayfieldCardTopology();

	/**
	 * @brief 为新添加到游戏区的卡牌更新覆盖拓扑关系
	 *
	 * 仅计算新卡牌与现有游戏区卡牌之间的覆盖关系
	 *
	 * @param cardId 新添加的卡牌ID
	 */
	void addPlayfieldCardTopology(int cardId);

	/**
	 * @brief 移除从游戏区移走的卡牌的覆盖拓扑关系
	 *
	 * 更新受影响的其他卡牌的覆盖状态
	 *
	 * @param cardId 被移除的卡牌ID
	 */
	void removePlayfieldCardTopology(int cardId);

private:
	// 关卡配置加载器
	LevelConfigLoader _levelConfigLoader;

	// 游戏区卡牌集合（ID->配置）
	std::map<int, CardConfig> _playfieldCards;
	// 堆叠区卡牌栈
	std::stack<CardConfig> _stackCards;
	// 手牌区卡牌栈
	std::stack<CardConfig> _handCards;

	// 卡牌锚点位置
	NS_CC::Vec2 _cardAnchorPoint;
	// 卡牌尺寸
	NS_CC::Size _cardSize;

	/**
	 * @brief 卡牌覆盖关系映射表
	 *
	 * key: 卡牌ID
	 * value: pair(被多少张卡牌覆盖, 覆盖当前卡牌的卡牌ID列表)
	 */
	std::unordered_map<int, std::pair<int, std::vector<int>>> _cardCovers;

	// 静态实例指针
	static GameModel* s_instance;
};