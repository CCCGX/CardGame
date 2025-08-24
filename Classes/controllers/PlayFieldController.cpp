#include "GameController.h"
#include "views/PlayFieldView.h"
#include "views/StackView.h"
#include "views/GameScene.h"
#include  "PlayFieldController.h"
#include  "StackController.h"
#include "managers/GameManager.h"
#include "managers/UndoManager.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

#include"configs/LevelConfigLoader.h"

USING_NS_CC;
PlayFieldController* PlayFieldController::init()
{
	auto ret = new PlayFieldController();
	return ret;
}
////初始化关卡场景
void PlayFieldController::initView(PlayFieldView* playFieldView)
{
	vector<CardConfig>cards = _gameManager->getGameModel()->getPlayfieldCards();
	playFieldView->initPlayFieldView(cards, this);
	_playFieldView = shared_ptr<PlayFieldView>(playFieldView);
}

//处理点击事件 判断是否可以移动，可以移动执行移动操作 
//调用mannager，执行移动 更新model数据
//播放 view动画
void PlayFieldController::handleCardClick(Ref* sender)
{
	auto card = dynamic_cast<CardView*>(sender);
	if (card)// 取出之前存储的cardID
	{
		const int cardID = card->getCardId();
		//判断是否匹配
		if (_gameManager->canClick(cardID))
		{
			//若匹配播放动画
			_undoManager->recordPlayfieldCardClick(cardID);
			_gameManager->getGameModel()->moveCardToHand(cardID);
			_playFieldView->playMatchAnimation(cardID);
		}
		else
		{
			//若不匹配播放错误动画
			_playFieldView->playFalseAnimation(cardID);
		}
	}
}
void PlayFieldController::setGameManager(shared_ptr<GameManager> gameManager)
{
	_gameManager = gameManager;
}
void PlayFieldController::setUndoManager(shared_ptr<UndoManager> undoManager)
{
	_undoManager = undoManager;
}