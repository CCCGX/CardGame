

#include "GameScene.h"
#include "PlayFieldView.h"
#include "SimpleAudioEngine.h"
#include <cocos/ui/UIButton.h>
#include "ui/CocosGUI.h"
#include "CardView.h"


//using namespace ui;
USING_NS_CC;

Scene* GameScene::createGameScene()
{
    return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
     //创建背景
     Sprite* backGround = Sprite::create("background.png");
     if (!backGround)
     {
         problemLoading("background.png");
     }
     else
     {
         backGround->setContentSize(visibleSize);
         backGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
         this->addChild(backGround, 0);
     }
     return true;
}
