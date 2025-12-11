#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "Paletka.h"
#include "Blok.h"

using namespace sf;
using namespace std;

class Pilka {
private:
	float x, y;
	float vx, vy; // predkosc pilki w plaszczyznie x i y
	float radius;

	Texture pilka;
	Sprite sprite;

	bool start = false;

public:
	Pilka(float X, float Y, float VX, float VY, float R)
		: x(X), y(Y), vx(VX), vy(-abs(VY)), radius(R)
	{
		pilka.loadFromFile("textures/Pilka.png");
		sprite.setTexture(pilka);

		sprite.setOrigin(pilka.getSize().x / 2, pilka.getSize().y / 2);
		sprite.setPosition(x, y);

		// DOPASOWANIE SPRITE'A DO PROMIENIA

		float scale = (radius * 2) / pilka.getSize().x;
		sprite.setScale(scale, scale);
	}

	void move(const Paletka& pal)
	{
		if (!start)
		{
			// Piłka siedzi na paletce i podąża za nią
			float palX = pal.getX();
			float palY = pal.getY();
			float palW = pal.getSzerokosc();

			x = palX; // środek paletki
			y = palY - pal.getWysokosc() / 2 - radius;

			sprite.setPosition(x, y);
			return;
		}

		// normalny ruch
		x += vx;
		y += vy;
		sprite.setPosition(x, y);
	}

	void bounceX() { vx = -vx; }
	void bounceY() { vy = -vy; }


	void collideWalls(float width, float height)
	{
		// LEWA ŚCIANA
		if (x - radius <= 0)
		{
			x = radius;
			bounceX();
		}

		// PRAWA ŚCIANA
		if (x + radius >= width)
		{
			x = width - radius;
			bounceX();
		}

		// SUFIT
		if (y - radius <= 0.f)
		{
			y = radius;
			bounceY();
		}

		sprite.setPosition(x, y);
	}

	bool collidePaddle(const Paletka& pal)
	{
		float palX = pal.getX();
		float palY = pal.getY();
		float palW = pal.getSzerokosc();
		float palH = pal.getWysokosc();
		float paddleTop = palY - palH / 2;

		bool poziomo = x >= palX - palW / 2 && x <= palX + palW / 2;
		bool pionowo = (y + radius) >= paddleTop && (y - radius) < paddleTop;

		if (poziomo && pionowo)
		{
			vy = -abs(vy);                // odbicie pionowe w górę
			y = paddleTop - radius;
			sprite.setPosition(x, y);
			return true;
		}

		return false;
	}

	void draw(RenderTarget& target)
	{
		target.draw(sprite);
	}

	bool collideBlock(Blok& blok)
	{
		if (blok.getBounds().intersects(sprite.getGlobalBounds()))
		{
			FloatRect b = blok.getBounds();
			FloatRect p = sprite.getGlobalBounds();

			float overlapLeft = (p.left + p.width) - b.left;
			float overlapRight = (b.left + b.width) - p.left;
			float overlapTop = (p.top + p.height) - b.top;
			float overlapBottom = (b.top + b.height) - p.top;

			float minOverlapX = std::min(overlapLeft, overlapRight);
			float minOverlapY = std::min(overlapTop, overlapBottom);

			// zderzenie POZIOME
			if (minOverlapX < minOverlapY)
				vx = -vx;
			// zderzenie PIONOWE
			else
				vy = -vy;

			return true;
		}
		return false;
	}

	void reset(const Paletka& pal)
	{
		start = false;

		x = pal.getX();
		y = pal.getY() - pal.getWysokosc() / 2 - radius;

		sprite.setPosition(x, y);
	}

	// GETTERY
		// GETTERY
	float getX() const { return x; }
	float getY() const { return y; }
	float getVx() const { return vx; }
	float getVy() const { return vy; }
	float getRadius() const { return radius; }

	void setStarted(bool s) { start = s; }
	bool isStarted() const { return start; }   // ⬅ DODANE
};
