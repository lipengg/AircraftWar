#include "Systems/InputController.h"

USING_NS_CC;

namespace aw {

bool InputController::init()
{
    if (!Node::init())
    {
        return false;
    }

    auto keyboard = EventListenerKeyboard::create();
    keyboard->onKeyPressed = CC_CALLBACK_2(InputController::onKeyPressed, this);
    keyboard->onKeyReleased = CC_CALLBACK_2(InputController::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);

    auto touch = EventListenerTouchOneByOne::create();
    touch->setSwallowTouches(true);
    touch->onTouchBegan = CC_CALLBACK_2(InputController::onTouchBegan, this);
    touch->onTouchMoved = CC_CALLBACK_2(InputController::onTouchMoved, this);
    touch->onTouchEnded = CC_CALLBACK_2(InputController::onTouchEnded, this);
    touch->onTouchCancelled = CC_CALLBACK_2(InputController::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch, this);

    auto mouse = EventListenerMouse::create();
    mouse->onMouseDown = CC_CALLBACK_1(InputController::onMouseDown, this);
    mouse->onMouseMove = CC_CALLBACK_1(InputController::onMouseMove, this);
    mouse->onMouseUp = CC_CALLBACK_1(InputController::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouse, this);

    return true;
}

InputController::State InputController::poll()
{
    State state;

    if (isDown(EventKeyboard::KeyCode::KEY_A) || isDown(EventKeyboard::KeyCode::KEY_LEFT_ARROW))
    {
        state.axis.x -= 1.0f;
    }
    if (isDown(EventKeyboard::KeyCode::KEY_D) || isDown(EventKeyboard::KeyCode::KEY_RIGHT_ARROW))
    {
        state.axis.x += 1.0f;
    }
    if (isDown(EventKeyboard::KeyCode::KEY_S) || isDown(EventKeyboard::KeyCode::KEY_DOWN_ARROW))
    {
        state.axis.y -= 1.0f;
    }
    if (isDown(EventKeyboard::KeyCode::KEY_W) || isDown(EventKeyboard::KeyCode::KEY_UP_ARROW))
    {
        state.axis.y += 1.0f;
    }
    if (state.axis.lengthSquared() > 1.0f)
    {
        state.axis.normalize();
    }

    state.pointerActive = _touchActive || _mouseActive;
    state.pointerPos = _pointerPos;
    state.bombPressed = _bombQueued;
    _bombQueued = false;
    return state;
}

bool InputController::onTouchBegan(Touch* touch, Event* /*event*/)
{
    _touchActive = true;
    _pointerPos = touch->getLocation();
    return true;
}

void InputController::onTouchMoved(Touch* touch, Event* /*event*/)
{
    _touchActive = true;
    _pointerPos = touch->getLocation();
}

void InputController::onTouchEnded(Touch* touch, Event* /*event*/)
{
    _touchActive = false;
    _pointerPos = touch->getLocation();
}

void InputController::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* /*event*/)
{
    _keys[static_cast<int>(keyCode)] = true;
    if (keyCode == EventKeyboard::KeyCode::KEY_SPACE || keyCode == EventKeyboard::KeyCode::KEY_B)
    {
        _bombQueued = true;
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_P)
    {
        // GameScene polls ESC/P through a custom event to keep pause symmetric with HUD button.
        EventCustom pauseEvent("aw_toggle_pause");
        _eventDispatcher->dispatchEvent(&pauseEvent);
    }
}

void InputController::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* /*event*/)
{
    _keys[static_cast<int>(keyCode)] = false;
}

void InputController::onMouseDown(EventMouse* event)
{
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        _mouseActive = true;
        _pointerPos = Vec2(event->getCursorX(), event->getCursorY());
    }
}

void InputController::onMouseMove(EventMouse* event)
{
    _pointerPos = Vec2(event->getCursorX(), event->getCursorY());
}

void InputController::onMouseUp(EventMouse* event)
{
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        _mouseActive = false;
    }
}

bool InputController::isDown(EventKeyboard::KeyCode keyCode) const
{
    auto it = _keys.find(static_cast<int>(keyCode));
    return it != _keys.end() && it->second;
}

} // namespace aw
