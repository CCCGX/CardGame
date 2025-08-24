#include "GameModel.h"

USING_NS_CC;

GameModel* GameModel::s_instance = nullptr;

GameModel* GameModel::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new (std::nothrow) GameModel();
		CCASSERT(s_instance, "SingletonManager 实例创建失败!");
	}
	return s_instance;
}

GameModel::GameModel() : GameModel(Vec2(0.5f, 0.5f), Size(182.0f, 282.0f))
{
}

GameModel::GameModel(const Vec2& cardAnchorPoint, Size cardSize) :_cardAnchorPoint(cardAnchorPoint), _cardSize(cardSize)
{
}

GameModel::~GameModel()
{
}

bool GameModel::loadLevel(int levelId)
{
	_playfieldCards.clear();
	while (!_stackCards.empty()) _stackCards.pop();
	while (!_handCards.empty()) _handCards.pop();
	_cardCovers.clear();

	const auto& levelConfig = std::shared_ptr<LevelConfig>(_levelConfigLoader.loadLevelConfig(levelId));
	if (levelConfig == nullptr) {
		return false;
	}

	const auto& playfieldConfigs = levelConfig->getPlayfieldConfigs();
	const auto& stackConfigs = levelConfig->getStackConfigs();
	if (stackConfigs.empty()) {
		return false;
	}

	for (const auto& config : playfieldConfigs) {
		_playfieldCards.emplace(config.cardId, config);
	}

	for (int i = 0; i < stackConfigs.size() - 1; i++) {
		const auto& config = stackConfigs.at(i);
		_stackCards.emplace(config);
	}
	
	const auto& lastStackConfig = stackConfigs.at(stackConfigs.size() - 1);
	_handCards.emplace(lastStackConfig);

	rebuildPlayfieldCardTopology();

	return true;
}

const std::vector<CardConfig> GameModel::getPlayfieldCards() const
{
	std::vector<CardConfig> result;
	result.reserve(_playfieldCards.size());

	for (const auto& pair : _playfieldCards) {
		result.push_back(pair.second);
	}

	return result;
}

const std::vector<CardConfig> GameModel::getStackCards() const
{
	std::stack<CardConfig> tempStack = _stackCards;

	std::vector<CardConfig> result;
	result.reserve(tempStack.size());

	while (!tempStack.empty()) {
		result.push_back(tempStack.top());
		tempStack.pop();
	}

	std::reverse(result.begin(), result.end());

	return result;
}

const std::vector<CardConfig> GameModel::getHandCards() const
{
	std::stack<CardConfig> tempStack = _handCards;

	std::vector<CardConfig> result;
	result.reserve(tempStack.size());

	while (!tempStack.empty()) {
		result.push_back(tempStack.top());
		tempStack.pop();
	}

	std::reverse(result.begin(), result.end());

	return result;
}

int GameModel::getStackTopCardId() const
{
	if (_stackCards.empty()) return -1;
	return _stackCards.top().cardId;
}

bool GameModel::moveCardToPlayfield(int cardId)
{
	if (_handCards.empty() || cardId != _handCards.top().cardId) {
		return false;
	}
	const auto& topHandCard = _handCards.top();
	_handCards.pop();
	_playfieldCards.emplace(topHandCard.cardId, topHandCard);
	addPlayfieldCardTopology(topHandCard.cardId);
	return true;
}

bool GameModel::moveCardToStack(int cardId)
{
	if (_handCards.empty() || cardId != _handCards.top().cardId) {
		return false;
	}
	const auto& topHandCard = _handCards.top();
	_handCards.pop();
	_stackCards.emplace(topHandCard);
	return true;
}

bool GameModel::moveCardToHand(int cardId)
{
	if (!_stackCards.empty() && _stackCards.top().cardId == cardId) {
		const auto& topStackCards = _stackCards.top();
		_stackCards.pop();
		_handCards.emplace(topStackCards);
		return true;
	}

	const auto& it = _playfieldCards.find(cardId);
	if (it != _playfieldCards.end()) {
		const int id = it->first;
		_handCards.emplace(it->second);
		_playfieldCards.erase(it);
		removePlayfieldCardTopology(id);
		return true;
	}

	return false;
}

Rect GameModel::getCardAABB(const CardConfig& cardConfig) const
{
	const float posX = cardConfig.position.x;
	const float posY = cardConfig.position.y;

	// 计算AABB边界（左=X-锚点X*宽，下=Y-锚点Y*高，右=左+宽，上=下+高）
	const float left = posX - _cardAnchorPoint.x * _cardSize.width;
	const float bottom = posY - _cardAnchorPoint.y * _cardSize.height;
	const float right = left + _cardSize.width;
	const float top = bottom + _cardSize.height;

	return Rect(left, bottom, _cardSize.width, _cardSize.height);
}

bool GameModel::isCardCovered(int cardId) const
{
	const auto& it = _cardCovers.find(cardId);
	if (it == _cardCovers.end())
		return false;
	return it->second.first != 0;
}

bool GameModel::isCardInPlayfield(int cardId) const
{
	const auto& it = _playfieldCards.find(cardId);
	return it != _playfieldCards.end();
}

bool GameModel::checkPlayfieldCardFacesConsecutiveWithHandTopCard(int cardId) const
{
	const auto& it = _playfieldCards.find(cardId);
	if (it == _playfieldCards.end() || _handCards.empty()) {
		return false;
	}

	const auto& card = it->second;
	const auto& handCard = _handCards.top();
	return -1 == (card.cardFace - handCard.cardFace) || (card.cardFace - handCard.cardFace) == 1;
}

void GameModel::rebuildPlayfieldCardTopology()
{
	// 1. 清空原有拓扑数据（避免重复加载导致脏数据）
	_cardCovers.clear();

	// 2. 提取主牌区所有卡牌（转为列表方便遍历）
	std::vector<CardConfig> playfieldCards;
	playfieldCards.reserve(_playfieldCards.size());
	for (const auto& pair : _playfieldCards) {
		const auto& card = pair.second;
		playfieldCards.push_back(card);
		_cardCovers.emplace(card.cardId, std::pair<int, std::vector<int>>(0, std::vector<int>()));
	}

	// 3. 两两对比卡牌，判断覆盖关系
	for (int i = 0; i < playfieldCards.size(); i++) {
		const auto& cardA = playfieldCards.at(i);
		const int cardAId = cardA.cardId;
		const auto& aabbA = getCardAABB(cardA);
		const auto& itA = _cardCovers.find(cardAId);
		if (itA == _cardCovers.end()) continue;

		for (int j = i + 1; j < playfieldCards.size(); j++) {
			const auto& cardB = playfieldCards.at(j);
			const int cardBId = cardB.cardId;
			const auto& aabbB = getCardAABB(cardB);
			const auto& itB = _cardCovers.find(cardBId);
			if (itB == _cardCovers.end()) continue;

			const bool isOverlapped = aabbA.intersectsRect(aabbB);
			if (!isOverlapped) continue;

			itA->second.first++;
			itB->second.second.emplace_back(cardAId);
		}
	}
}

void GameModel::addPlayfieldCardTopology(int cardId)
{
	const auto& it = _playfieldCards.find(cardId);
	if (it == _playfieldCards.end()) return;
	const auto& targetCard = it->second;

	_cardCovers.emplace(cardId, std::pair<int, std::vector<int>>(0, std::vector<int>()));
	const auto& itA = _cardCovers.find(cardId);
	if (itA == _cardCovers.end()) return;

	for (const auto& pair : _playfieldCards) {
		const auto& card = pair.second;
		if (card.cardId == cardId) continue;

		const auto& aabbA = getCardAABB(targetCard);
		const auto& aabbB = getCardAABB(card);
		const bool isOverlapped = aabbA.intersectsRect(aabbB);
		if (!isOverlapped) continue;

		const auto& itB = _cardCovers.find(card.cardId);
		if (itB == _cardCovers.end()) continue;

		if (card.cardId < cardId) {
			itB->second.first++;
			itA->second.second.emplace_back(card.cardId);
		}
		else if (card.cardId > cardId) {
			itA->second.first++;
			itB->second.second.emplace_back(cardId);
		}
	}
}

void GameModel::removePlayfieldCardTopology(int cardId)
{
	const auto& it = _cardCovers.find(cardId);
	if (it == _cardCovers.end()) return;

	for (const auto& id : it->second.second) {
		const auto& itCovers = _cardCovers.find(id);
		if (itCovers == _cardCovers.end()) continue;

		itCovers->second.first--;
	}
	_cardCovers.erase(it);
}
