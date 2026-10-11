#pragma once

#include "cocos2d.h"

#include <string>

namespace aw {

class TextureFactory
{
public:
    static cocos2d::Texture2D* texture(const std::string& key);
    static cocos2d::Sprite* sprite(const std::string& key);

private:
    static cocos2d::Texture2D* generate(const std::string& key);
};

} // namespace aw
