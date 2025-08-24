#include "CardView.h"

USING_NS_CC;

CardView* CardView::createCard(CardSuitType suitType, CardFaceType faceType, int cardId)
{
    CardView* ret = new (std::nothrow) CardView();

    if (ret) {
        ret->_suitType = suitType;
        ret->_faceType = faceType;
        ret->_cardId = cardId;
    }

    if (ret && ret->init())
    {
        ret->autorelease();
        // 启用触摸事件
        ret->setTouchEnabled(true);
        ret->setSwallowTouches(true);
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CardSuitType CardView::getSuitType()
{
    return _suitType;
}

CardFaceType CardView::getFaceType()
{
    return _faceType;
}

int CardView::getCardId()
{
    return _cardId;
}

std::string getSuitImagePath(CardSuitType suitType) {
    switch (suitType)
    {
    case CST_CLUBS:
        return "res/res/suits/club.png";
    case CST_DIAMONDS:
        return "res/res/suits/diamond.png";
    case CST_HEARTS:
        return "res/res/suits/heart.png";
    case CST_SPADES:
        return "res/res/suits/spade.png";
    case CST_NUM_CARD_SUIT_TYPES:
    case CST_NONE:
	default:
        return "";
    }
}

std::string getNumberImagePath(CardSuitType suitType, CardFaceType faceType, bool isSmall) {

    std::string basePath = "res/res/number/";
    if (isSmall) 
    {
        basePath += "small_";
    }
    else 
    {
        basePath += "big_";
    }

    switch (suitType)
    {
    case CST_CLUBS:
    case CST_SPADES:
        basePath += "black_";
        break;
    case CST_DIAMONDS:
    case CST_HEARTS:
        basePath += "red_";
        break;
    case CST_NUM_CARD_SUIT_TYPES:
    case CST_NONE:
    default:
        return "";
    }


    switch (faceType)
    {
    case CFT_ACE:
        return basePath + "A.png";
    case CFT_TWO:
        return basePath + "2.png";
    case CFT_THREE:
        return basePath + "3.png";
    case CFT_FOUR:
        return basePath + "4.png";
    case CFT_FIVE:
        return basePath + "5.png";
    case CFT_SIX:
        return basePath + "6.png";
    case CFT_SEVEN:
        return basePath + "7.png";
    case CFT_EIGHT:
        return basePath + "8.png";
    case CFT_NINE:
        return basePath + "9.png";
    case CFT_TEN:
        return basePath + "10.png";
    case CFT_JACK:
        return basePath + "J.png";
    case CFT_QUEEN:
        return basePath + "Q.png";
    case CFT_KING:
        return basePath + "K.png";
    case CFT_NUM_CARD_FACE_TYPES:
    case CFT_NONE:
    default:
        return "";
    }
}

bool CardView::init()
{
    if (!Widget::init()) {
        return false;
    }
    _background = Sprite::create("res/res/card_general.png");
    if (_background == nullptr) {
        return false;
    }
    const auto& backgroundSize = _background->getContentSize();
    setContentSize(backgroundSize);
    _background->setAnchorPoint(Vec2(0, 0));
    _background->setPosition(Vec2(0, 0));
    addChild(_background);

    const auto& suitImagePath = getSuitImagePath(_suitType);
    if (!suitImagePath.empty()) 
    {
        _suit = Sprite::create(suitImagePath);
        if (_suit == nullptr) {
            return false;
        }
        _suit->setAnchorPoint(Vec2(1, 1));
		_suit->setPosition(Vec2(backgroundSize.width - backgroundSize.width * 0.1, backgroundSize.height - backgroundSize.height * 0.07));
        addChild(_suit);
    }

    const auto& smallNumberImagePath = getNumberImagePath(_suitType, _faceType, true);
    if (!smallNumberImagePath.empty())
    {
        _numberSmall = Sprite::create(smallNumberImagePath);
        if (_numberSmall == nullptr) {
            return false;
        }
        _numberSmall->setAnchorPoint(Vec2(0, 1));
        _numberSmall->setPosition(Vec2(backgroundSize.width * 0.1, backgroundSize.height - backgroundSize.height * 0.07));
        addChild(_numberSmall);
    }

    const auto& bigNumberImagePath = getNumberImagePath(_suitType, _faceType, false);
    if (!bigNumberImagePath.empty())
    {
        _number = Sprite::create(bigNumberImagePath);
        if (_number == nullptr) {
            return false;
        }
        auto& numberSize = _number->getContentSize();
        _number->setAnchorPoint(Vec2(0, 0));
        _number->setPosition(Vec2((backgroundSize.width - numberSize.width) / 2, backgroundSize.height * 0.15));
        addChild(_number);
    }

	return true;
}
