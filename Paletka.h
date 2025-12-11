#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Paletka {
private:
    float x;
    float y;
    float szerokosc;
    float wysokosc;
    float predkosc;

    Texture paletka;
    Sprite sprite;

public:
    Paletka(float X, float Y, float SZER, float WYS, float PRED)
        : x(X), y(Y), szerokosc(SZER), wysokosc(WYS), predkosc(PRED)
    {
        paletka.loadFromFile("textures/Paletka.png");

        sprite.setTexture(paletka);
        sprite.setOrigin(paletka.getSize().x / 2, paletka.getSize().y / 2);
        sprite.setPosition(x, y);

        sprite.setScale(szerokosc / paletka.getSize().x,
            wysokosc / paletka.getSize().y);
    }

    void reset(float X, float Y)
    {
        x = X;
        y = Y;
        sprite.setPosition(x, y);
    }

    void moveLeft() {
        x -= predkosc;
        sprite.setPosition(x, y);
    }

    void moveRight() {
        x += predkosc;
        sprite.setPosition(x, y);
    }

    void clampToBounds(float width) {
        if (x - szerokosc / 2 <= 0)
            x = szerokosc / 2;
        if (x + szerokosc / 2 >= width)
            x = width - szerokosc / 2;

        sprite.setPosition(x, y);
    }

    void draw(sf::RenderTarget& target) {
        target.draw(sprite);
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getSzerokosc() const { return szerokosc; }
    float getWysokosc() const { return wysokosc; }
    const Sprite& getSprite() const { return sprite; }
};
