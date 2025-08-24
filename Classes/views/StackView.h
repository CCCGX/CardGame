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
    //创建卡牌 controller注册的的回调函数 展示view
    //卡牌动画类型 undo match replace
     //对于每一个 卡片，都有事件监听 
    void playMoveToAnimation(int cardID);
    void playUndoAnimation();

private:
    //三个区域  上面，手牌，已获得的牌。对于view来说 不需要关注这些牌有什么不同，只需要关注id即可，
    std::map<int, CardView*>_stackCard; //用ID获得CardView 实现动画效果
    std::map<int, Vec2>_handCardPrePos;//
    std:: stack<int>_handCardID;//现在在手里的卡牌 
    Vec2 stackPos;
    //Sprite * upPlayCard
    // implement the "static create()" method manually
};

