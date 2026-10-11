#pragma once

#include "cocos2d.h"

#include <map>

namespace aw {

class InputController : public cocos2d::Node
{
public:
    struct State
    {
        cocos2d::Vec2 axis = cocos2d::Vec2::ZERO;
        bool pointerActive = false;
        cocos2d::Vec2 pointerPos = cocos2d::Vec2::ZERO;
        bool bombPressed = false;
    };

    CREATE_FUNC(InputController);

    bool init() override;
    State poll();

private:
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void onMouseDown(cocos2d::EventMouse* event);
    void onMouseMove(cocos2d::EventMouse* event);
    void onMouseUp(cocos2d::EventMouse* event);

    bool isDown(cocos2d::EventKeyboard::KeyCode keyCode) const;

private:
    std::map<int, bool> _keys;
    bool _touchActive = false;
    bool _mouseActive = false;
    cocos2d::Vec2 _pointerPos = cocos2d::Vec2::ZERO;
    bool _bombQueued = false;
};

} // namespace aw
