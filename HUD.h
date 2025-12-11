#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace sf;
using namespace std;

class HUD
{
private:
    int zycia;
    int punkty;

    Clock gameClock;
    Text timerText;

    Texture heartTex;
    vector<Sprite> hearts;

    Font font;
    Text scoreText;

    bool timerStarted;

public:
    HUD()
    {
        zycia = 3;
        punkty = 0;
        timerStarted = false;

        // SERDUSZKA
        heartTex.loadFromFile("textures/zycie.png");

        hearts.resize(3);
        for (int i = 0; i < 3; i++)
        {
            hearts[i].setTexture(heartTex);
            hearts[i].setScale(0.3f, 0.3f);
            hearts[i].setPosition(10 + i * 40, 10);
        }

        font.loadFromFile("textures/pixelfont.ttf");

        // PUNKTY
        scoreText.setFont(font);
        scoreText.setCharacterSize(35);
        scoreText.setFillColor(Color::White);
        scoreText.setString("0");
        scoreText.setPosition(675, -5);

        // GAME TIMER
        timerText.setFont(font);
        timerText.setCharacterSize(35);
        timerText.setFillColor(Color::White);
        timerText.setPosition(17.5, 40); // pod serduszkami
        timerText.setString("0:00");
    }

    void dodajPunkty(int ile, float windowWidth)
    {
        punkty += ile;
        scoreText.setString(std::to_string(punkty));

        // automatyczne wyrównanie do prawej strony
        FloatRect bounds = scoreText.getLocalBounds();
        float rightMargin = 20.f;
        scoreText.setPosition(windowWidth - bounds.width - rightMargin, -5);
    }

    void utrataZycia()
    {
        if (zycia > 0) zycia--;

        for (int i = 0; i < 3; i++)
        {
            if (i < zycia) hearts[i].setColor(Color::White);
            else hearts[i].setColor(Color(255, 255, 255, 50));
        }
    }

    int getZycia() const { return zycia; }
    int getPunkty() const { return punkty; }

    void startTimer()
    {
        if (!timerStarted)
        {
            gameClock.restart();
            timerStarted = true;
        }
    }

    void updateTimer()
    {
        if (!timerStarted) return;

        Time elapsed = gameClock.getElapsedTime();
        int totalSeconds = (int)elapsed.asSeconds();

        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;

        ostringstream ss;
        ss << minutes << ":" << setw(2) << setfill('0') << seconds;
        timerText.setString(ss.str());
    }

    void draw(RenderWindow& window)
    {
        for (auto& h : hearts)
            window.draw(h);

        window.draw(scoreText);
        window.draw(timerText);
    }

    void reset()
    {
        zycia = 3;
        punkty = 0;

        // reset tekstów
        scoreText.setString("0");

        // reset serduszek
        for (int i = 0; i < 3; i++)
            hearts[i].setColor(Color::White);

        // reset timera
        timerStarted = false;
        timerText.setString("0:00");
        gameClock.restart();
    }
};
