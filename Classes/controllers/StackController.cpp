#include "GameController.h"
#include "views/PlayFieldView.h"
#include "views/StackView.h"
#include "views/GameScene.h"
#include  "StackController.h"
#include  "StackController.h"
#include "managers/GameManager.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

#include"configs/LevelConfigLoader.h"

USING_NS_CC;
StackController* StackController::init()
{
	auto ret = new StackController();
	return ret;
}
////初始化关卡场景
void StackController::initView(PlayFieldView* playFieldView)
{
	vector<CardConfig>cards = _gameManager->getGameModel()->getStackCards();
	playFieldView->initStackView(cards, this);
	vector<CardConfig>handcard = _gameManager->getGameModel()->getHandCards();
	playFieldView->initHandView(handcard, this);
	_playFieldView = shared_ptr<PlayFieldView>(playFieldView);
	playFieldView->initUndoView(this);
}
//处理点击事件 判断是否可以移动，可以移动执行移动操作 
//调用mannager，执行移动 更新model数据
//播放 view动画
void StackController::handleCardClick(Ref* sender)
{
	auto card = dynamic_cast<CardView*>(sender);
	const int cardID = card->getCardId();
	if (_gameManager->canClick(cardID))
	{
		//若匹配播放动画
		_undoManager->recordStackCardClick(cardID);
		_gameManager->getGameModel()->moveCardToHand(cardID);
		_playFieldView->playMoveToAnimation(cardID);
	}
	else
	{
		//若不匹配播放错误动画
		_playFieldView->playFalseAnimation(cardID);
	}
}
void StackController::handleUndoClick(Ref* sender)
{
	const auto& actionRecord = _undoManager->removeActionRecord();
	if (actionRecord.action == CLICK_PLAY_FIELD) {
		_gameManager->getGameModel()->moveCardToPlayfield(actionRecord.cardId);
	}
	else if (actionRecord.action == CLICK_STACK_CARD) {
		_gameManager->getGameModel()->moveCardToStack(actionRecord.cardId);
	}
	else {
		return;
	}
	_playFieldView->playUndoAnimation();
}
void StackController::setGameManager(shared_ptr<GameManager> gameManager)
{
	_gameManager = gameManager;
}
void StackController::setUndoManager(shared_ptr<UndoManager> undoManager)
{
	_undoManager = undoManager;
}