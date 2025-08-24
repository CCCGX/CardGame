#include "PlayFieldView.h"
#include "SimpleAudioEngine.h"
#include "controllers/PlayFieldController.h"
#include <cocos/ui/UIButton.h>
#include "ui/CocosGUI.h"
#include "PlayFieldView.h"
#include "configs/LevelConfig.h"


//using namespace ui;
USING_NS_CC;
PlayFieldView * PlayFieldView::createGameView()
{
    PlayFieldView* ret = new (std::nothrow) PlayFieldView();
    if (ret&&ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in PlayFieldViewScene.cpp\n");
}

// on "init" you need to initialize your instance
bool PlayFieldView::init()
{
    return true;
}
void PlayFieldView::playMatchAnimation(int cardID)
{
    CardView* pCardView = _cardIDtoCardView[cardID];
    _CardPrePos[cardID] = pCardView->getPosition();//把移动前的位置保存起来，
    _handCardID.push(cardID);  //加入到手牌栈里面
    auto moveTo = MoveTo::create(0.5, _handPos);//
    pCardView->setZOrder(_handZorder++);
    pCardView->runAction(moveTo);
}
void PlayFieldView::playMoveToAnimation(int cardID)
{
    CardView* pCardView = _cardIDtoCardView[cardID];
    //_CardPrePos[cardID] = pCardView->getPosition();//把移动前的位置保存起来，
    _handCardID.push(cardID);  //加入到手牌栈里面
    pCardView->setZOrder(_handZorder++);
    auto moveTo = MoveTo::create(0.5, _handPos);//
    pCardView->runAction(moveTo);
}

void PlayFieldView::playFalseAnimation(int cardID)
{
    CardView* pCardView = _cardIDtoCardView[cardID];
    auto scaleTo1 = ScaleTo::create(0.2, 1.2);
    auto scaleTo2 = ScaleTo::create(0.2, 1);
    auto sq = Sequence::create(scaleTo1,scaleTo2,nullptr);
    //pCardView->runAction(scaleTo1);
    //pCardView->runAction(scaleTo2);
    pCardView->runAction(sq);
}
void PlayFieldView::playUndoAnimation()
{
    if (_handCardID.empty()) return;
    int cardID = _handCardID.top();
    _handCardID.pop();
    auto moveTo = MoveTo::create(0.5, _CardPrePos[cardID]);

    auto& cardView = _cardIDtoCardView[cardID];

    cardView->runAction(moveTo);//栈顶元素 回到原来的位置
    cardView->scheduleOnce([cardView](float dt) {
        cardView->setZOrder(0);
        }, 0.3, std::string("playUndoAnimation_") + std::to_string(cardID));
}
//要回调函数，只有一个controller对象，要知道那儿个对象
void PlayFieldView::initPlayFieldView(std::vector<CardConfig>& cards,PlayFieldController* pPlayFieldController)
{
    for (CardConfig it :cards)
    {
        auto card = CardView::createCard(it.cardSuit, it.cardFace, it.cardId);
        card->setPosition(Vec2(it.position.x, it.position.y + 580));
        _CardPrePos[it.cardId] = card->getPosition();//把移动前的位置保存起来，
        card->setTag(it.cardId);
        card->addClickEventListener(CC_CALLBACK_1(PlayFieldController::handleCardClick, pPlayFieldController));
        _cardIDtoCardView[it.cardId] = card;
        this->addChild(card);
        //log("Zorder: %d\n", card->getZOrder());
        //log("sib :%d\n", card->getSiblingIndex());
    }
}

void PlayFieldView::initStackView(std::vector<CardConfig>& cards, StackController* pStackController)
{
    //int size = cards.size();
    int intervalDis = 100;
    int cnt = 0;
    for (CardConfig it : cards)
    {
        auto card = CardView::createCard(it.cardSuit, it.cardFace, it.cardId);
        card->setPosition(Vec2(_stackPos.x +cnt*intervalDis, _stackPos.y));
        _CardPrePos[it.cardId] = card->getPosition();//把移动前的位置保存起来，
        card->setTag(it.cardId);
        card->addClickEventListener(CC_CALLBACK_1(StackController::handleCardClick, pStackController));
        _cardIDtoCardView[it.cardId] = card;
        this->addChild(card);
        cnt++;
    }
}
void PlayFieldView::initHandView(std::vector<CardConfig>& cards, StackController* pStackController)
{
    for (CardConfig it : cards)
    {
        auto card = CardView::createCard(it.cardSuit, it.cardFace, it.cardId);
        card->setPosition(_handPos);
        card->setTag(it.cardId);
        card->addClickEventListener(CC_CALLBACK_1(StackController::handleCardClick, pStackController));
        _cardIDtoCardView[it.cardId] = card;
        this->addChild(card);
    }
}
void PlayFieldView::initUndoView(StackController* pStackController)
{
    //创建rollback按钮
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto rollBackButton = ui::Button::create("rollback.png");
    rollBackButton->setPosition(Vec2(visibleSize.width - rollBackButton->getContentSize().width,  3*rollBackButton->getContentSize().height));
    rollBackButton->setPressedActionEnabled(true);
    rollBackButton->addClickEventListener(CC_CALLBACK_1(StackController::handleUndoClick, pStackController));
    this->addChild(rollBackButton, 2);
}