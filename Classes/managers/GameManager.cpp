#include "GameManager.h"

USING_NS_CC;

GameManager::GameManager()
{
	_gameModel = GameModel::getInstance();
}

bool GameManager::startLevel(int levelId)
{
	return _gameModel->loadLevel(levelId);
}

void GameManager::startLevelWithCallBack(int levelId, std::function<void(bool)> loadDoneCallBack)
{
	const auto& scheduler = Director::getInstance()->getScheduler();
	_loadDoneCallBack = loadDoneCallBack;
	scheduler->schedule([this, levelId](float dt) {
		const bool res = _gameModel->loadLevel(levelId);
		_loadDoneCallBack(res);
		}, this, 0,0,0, false, "startLevel");
}

GameModel* GameManager::getGameModel()
{
	return _gameModel;
}

bool GameManager::canClick(int cardId)
{
	if (cardId == _gameModel->getStackTopCardId()) {
		return true;
	}
	if (!_gameModel->isCardInPlayfield(cardId) || _gameModel->isCardCovered(cardId)) {
		return false;
	}
	return _gameModel->checkPlayfieldCardFacesConsecutiveWithHandTopCard(cardId);
}
