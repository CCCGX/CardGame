

#include "StackView.h"
#include"StackView.h"
#include "SimpleAudioEngine.h"
#include <cocos/ui/UIButton.h>
#include "ui/CocosGUI.h"


//using namespace ui;
USING_NS_CC;

StackView* StackView::createStackView()
{
    StackView* ret = new (std::nothrow) StackView();

    if (ret && ret->init())
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
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in StackViewScene.cpp\n");
}

// on "init" you need to initialize your instance
bool StackView::init()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
  //  ��ȡ����
//������Ƭ��Ϣ 
//��ӽ��븸�ڵ�
 //Ϊÿһ�ſ�Ƭ��ע��ص����� 

    return true;
}

void StackView::playMoveToAnimation(int cardID)
{
    CardView* pCardView = _stackCard[cardID];
    _handCardPrePos[cardID] = pCardView->getPosition();//��ȡ��ǰλ�ã�
    _handCardID.push(cardID);  //���뵽����ջ����
    auto moveTo = MoveTo::create(1, stackPos);//
    pCardView->runAction(moveTo);
}
void StackView::playUndoAnimation()
{
    if (_handCardID.empty()) return;
    int cardID =_handCardID.top();
    _handCardID.pop();
    auto moveTo = MoveTo::create(1, _handCardPrePos[cardID]);
    _stackCard[cardID]->runAction(moveTo);//�ص�ԭ����λ��
}