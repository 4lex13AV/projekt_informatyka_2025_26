#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Blok {
private:
    int zycia;          // 2 → 1 → 0
    Sprite sprite;

    static Texture texFull;
    static Texture texHalf;

public:

    static void loadTextures()
    {
        texFull.loadFromFile("textures/Blok2_2.png");
        texHalf.loadFromFile("textures/Blok2_1.png");
    }

    Blok(float x, float y) : zycia(2)
    {
        sprite.setTexture(texFull);
        sprite.setPosition(x, y);
    }

    void draw(RenderTarget& target)
    {
        if (zycia > 0)
            target.draw(sprite);
    }

    FloatRect getBounds() const
    {
        return sprite.getGlobalBounds();
    }

    bool hit()
    {
        if (zycia <= 0) return false;

        zycia--;

        switch (zycia)
        {
        case 1:
            sprite.setTexture(texHalf);
            break;

        case 0:
            sprite.setPosition(-200.f, -200.f);
            break;
        }

        return zycia == 0;
    }

    bool isAlive() const {
    return zycia > 0;
}
};
