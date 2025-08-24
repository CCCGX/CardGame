/****************************************************************************

 ****************************************************************************/
#pragma once
#include "StackView.h"
#include "cocos2d.h"
#include "CardView.h"
#include <map>
class StackView : public cocos2d::Node
{
public:
    static StackView* createStackView();
    virtual bool init();
    //�������� controllerע��ĵĻص����� չʾview
    //���ƶ������� undo match replace
     //����ÿһ�� ��Ƭ�������¼����� 
    void playMoveToAnimation(int cardID);
    void playUndoAnimation();

private:
    //��������  ���棬���ƣ��ѻ�õ��ơ�����view��˵ ����Ҫ��ע��Щ����ʲô��ͬ��ֻ��Ҫ��עid���ɣ�
    std::map<int, CardView*>_stackCard; //��ID���CardView ʵ�ֶ���Ч��
    std::map<int, Vec2>_handCardPrePos;//
    std:: stack<int>_handCardID;//����������Ŀ��� 
    Vec2 stackPos;
    //Sprite * upPlayCard
    // implement the "static create()" method manually
};

