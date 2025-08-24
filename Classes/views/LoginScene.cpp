
#include "LoginScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include <cocos/ui/UIButton.h>
#include "ui/CocosGUI.h"
#include "CardView.h"
#include "controllers/GameController.h"
//using namespace ui;
USING_NS_CC;

Scene* LoginScene::createScene()
{
    return LoginScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in LoginSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //  创建标语
    auto label = Label::createWithTTF("Card Match Game!\nChoose Level!", "fonts/Marker Felt.ttf", 80);
    label->setTextColor(Color4B::ORANGE);
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
    origin.y + visibleSize.height - label->getContentSize().height));
    // add the label as a child to this layer
    this->addChild(label, 3);

    //创建背景
    Sprite * backGround = Sprite::create("background.png");
    backGround->setContentSize(visibleSize);
    backGround->setPosition(Vec2(visibleSize.width/2+origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(backGround,0);
    //创建按钮
    auto levelButton = ui::Button::create("level1new.png", "level1new.png");
    levelButton->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    levelButton->setPressedActionEnabled(true);
    levelButton->addClickEventListener(CC_CALLBACK_1(LoginScene::levelButtonCallBack,this));
    //levelButton->setContentSize(Size(50, 50));
    this->addChild(levelButton, 1);
    return true;
}

void LoginScene::levelButtonCallBack(Ref* pSender)
{
    GameController::startGame(1);
}
