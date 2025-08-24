#include "GameController.h"
#include "views/GameScene.h"
#include "views/PlayFieldView.h"
#include "views/StackView.h"
#include  "PlayFieldController.h"
#include  "StackController.h"
#include "models/GameModel.h"
#include "cocos2d.h"

#include "configs/LevelConfigLoader.h"


void GameController::startGame(int levelID)// GameScene* gameScene
{
	GameController* ret = new (std::nothrow) GameController();

	if (ret )
	{
		ret->init(levelID);
	}

}
bool GameController::init(int levelID)
{
	_gameManager = make_shared<GameManager> ();
	_gameManager->startLevel(1);
	_undoManager = make_shared<UndoManager>();

	//GameController初始化各子控制器:
	_playFieldController = shared_ptr<PlayFieldController>(PlayFieldController::init());
	_stackController = shared_ptr<StackController>(StackController::init());

	//设置manager指针
	_playFieldController->setGameManager(_gameManager);
	_playFieldController->setUndoManager(_undoManager);
	_stackController->setGameManager(_gameManager);
	_stackController->setUndoManager(_undoManager);

  //创建GameView并添加到父节点:
	auto gameScene = GameScene::createGameScene();
    Director::getInstance()->replaceScene(gameScene);



	PlayFieldView* playFieldView = PlayFieldView::createGameView();
	gameScene->addChild(playFieldView);

	//初始化各子控制器的视图 :
	_stackController->initView(playFieldView);
	_playFieldController->initView( playFieldView);
	
	//GameView初始化UI : 
		//创建游戏场景
	return true;
}