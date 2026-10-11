#include "TextureFactory.h"

#include <cmath>
#include <unordered_map>
#include <vector>

USING_NS_CC;

namespace aw {
namespace {

struct RGBA
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

std::unordered_map<std::string, Texture2D*> s_cache;

void put(std::vector<unsigned char>& data, int w, int h, int x, int y, const RGBA& c)
{
    if (x < 0 || y < 0 || x >= w || y >= h)
    {
        return;
    }
    const size_t idx = static_cast<size_t>(y * w + x) * 4;
    data[idx + 0] = c.r;
    data[idx + 1] = c.g;
    data[idx + 2] = c.b;
    data[idx + 3] = c.a;
}

void fillCircle(std::vector<unsigned char>& data, int w, int h, float cx, float cy, float radius, const RGBA& c)
{
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            const float dx = x - cx;
            const float dy = y - cy;
            const float dist = std::sqrt(dx * dx + dy * dy);
            if (dist <= radius)
            {
                RGBA out = c;
                const float edge = radius - dist;
                if (edge < 2.0f)
                {
                    out.a = static_cast<unsigned char>(c.a * cocos2d::clampf(edge / 2.0f, 0.0f, 1.0f));
                }
                put(data, w, h, x, y, out);
            }
        }
    }
}

void fillTriangle(std::vector<unsigned char>& data, int w, int h, const RGBA& c)
{
    const float cx = w * 0.5f;
    for (int y = 0; y < h; ++y)
    {
        const float t = static_cast<float>(y) / static_cast<float>(h - 1);
        const float half = (1.0f - t) * w * 0.48f;
        for (int x = 0; x < w; ++x)
        {
            if (std::fabs(x - cx) <= half)
            {
                put(data, w, h, x, y, c);
            }
        }
    }
    fillCircle(data, w, h, cx, h * 0.42f, w * 0.13f, { 255, 255, 255, 220 });
}

void fillDiamond(std::vector<unsigned char>& data, int w, int h, const RGBA& c)
{
    const float cx = (w - 1) * 0.5f;
    const float cy = (h - 1) * 0.5f;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            const float v = std::fabs(x - cx) / (w * 0.5f) + std::fabs(y - cy) / (h * 0.5f);
            if (v <= 1.0f)
            {
                put(data, w, h, x, y, c);
            }
        }
    }
}

void fillBullet(std::vector<unsigned char>& data, int w, int h, const RGBA& c)
{
    const float cx = (w - 1) * 0.5f;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            const bool body = std::fabs(x - cx) <= w * 0.28f;
            const bool head = y < h * 0.35f && std::fabs(x - cx) <= (1.0f - y / (h * 0.35f)) * w * 0.5f;
            if (body || head)
            {
                put(data, w, h, x, y, c);
            }
        }
    }
}

Texture2D* makeTexture(const std::vector<unsigned char>& data, int w, int h)
{
    Texture2D* tex = new (std::nothrow) Texture2D();
    CCASSERT(tex, "TextureFactory: allocation failed");
    tex->initWithData(data.data(), static_cast<ssize_t>(data.size()), Texture2D::PixelFormat::RGBA8888, w, h, Size(static_cast<float>(w), static_cast<float>(h)));
    tex->autorelease();
    tex->retain();
    tex->setAliasTexParameters();
    return tex;
}

} // namespace

Texture2D* TextureFactory::generate(const std::string& key)
{
    int w = 16;
    int h = 16;
    RGBA base{ 255, 255, 255, 255 };

    if (key == "player") { w = 56; h = 72; }
    else if (key == "enemy_grunt") { w = 50; h = 50; }
    else if (key == "enemy_weaver") { w = 58; h = 42; }
    else if (key == "enemy_drifter") { w = 54; h = 54; }
    else if (key == "bullet_player") { w = 10; h = 24; }
    else if (key == "bullet_enemy") { w = 12; h = 12; }
    else if (key == "pickup_power" || key == "pickup_shield" || key == "pickup_bomb") { w = 34; h = 34; }
    else if (key == "explosion") { w = 72; h = 72; }
    else if (key == "star") { w = 4; h = 4; }

    std::vector<unsigned char> data(static_cast<size_t>(w * h * 4), 0);

    if (key == "player")
    {
        fillTriangle(data, w, h, { 76, 224, 255, 255 });
    }
    else if (key == "enemy_grunt")
    {
        fillCircle(data, w, h, w * 0.5f, h * 0.5f, w * 0.48f, { 232, 76, 84, 255 });
        fillCircle(data, w, h, w * 0.5f, h * 0.42f, w * 0.16f, { 80, 18, 26, 255 });
    }
    else if (key == "enemy_weaver")
    {
        fillDiamond(data, w, h, { 255, 164, 64, 255 });
        fillCircle(data, w, h, w * 0.5f, h * 0.5f, w * 0.12f, { 90, 38, 10, 255 });
    }
    else if (key == "enemy_drifter")
    {
        fillCircle(data, w, h, w * 0.5f, h * 0.5f, w * 0.48f, { 184, 96, 255, 255 });
        fillCircle(data, w, h, w * 0.5f, h * 0.5f, w * 0.2f, { 48, 18, 78, 255 });
    }
    else if (key == "bullet_player")
    {
        fillBullet(data, w, h, { 86, 240, 255, 255 });
    }
    else if (key == "bullet_enemy")
    {
        fillCircle(data, w, h, w * 0.5f, h * 0.5f, w * 0.48f, { 255, 82, 214, 255 });
    }
    else if (key == "pickup_power")
    {
        fillDiamond(data, w, h, { 86, 255, 128, 255 });
    }
    else if (key == "pickup_shield")
    {
        fillCircle(data, w, h, w * 0.5f, h * 0.5f, w * 0.48f, { 90, 160, 255, 255 });
        fillCircle(data, w, h, w * 0.5f, h * 0.5f, w * 0.28f, { 20, 42, 90, 255 });
    }
    else if (key == "pickup_bomb")
    {
        fillCircle(data, w, h, w * 0.5f, h * 0.5f, w * 0.48f, { 255, 232, 82, 255 });
        fillCircle(data, w, h, w * 0.38f, h * 0.36f, w * 0.12f, { 120, 80, 10, 255 });
    }
    else if (key == "explosion")
    {
        fillCircle(data, w, h, w * 0.5f, h * 0.5f, w * 0.48f, { 255, 112, 48, 230 });
        fillCircle(data, w, h, w * 0.5f, h * 0.5f, w * 0.28f, { 255, 232, 120, 255 });
    }
    else if (key == "star")
    {
        base = { 220, 238, 255, 255 };
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                put(data, w, h, x, y, base);
            }
        }
    }
    else
    {
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                put(data, w, h, x, y, base);
            }
        }
    }

    return makeTexture(data, w, h);
}

Texture2D* TextureFactory::texture(const std::string& key)
{
    auto it = s_cache.find(key);
    if (it != s_cache.end())
    {
        return it->second;
    }
    Texture2D* tex = generate(key);
    s_cache.emplace(key, tex);
    return tex;
}

Sprite* TextureFactory::sprite(const std::string& key)
{
    return Sprite::createWithTexture(texture(key));
}

} // namespace aw
