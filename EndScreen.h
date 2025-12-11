#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ScoreManager.h"

using namespace sf;

class EndScreen
{
private:
    Font font;

    Text titleText;
    Text scoreText;

    Text inputLabel;
    Text nickText;
    Text instruction;

    std::string nickInput;

    ScoreManager scoreManager;

    bool victory;
    int finalScore;

public:
    EndScreen()
    {
        font.loadFromFile("textures/pixelfont.ttf");

        titleText.setFont(font);
        titleText.setCharacterSize(140);
        titleText.setFillColor(Color::White);
        titleText.setPosition(85, 25);

        scoreText.setFont(font);
        scoreText.setCharacterSize(60);
        scoreText.setFillColor(Color::Yellow);
        scoreText.setPosition(165, 170);

        inputLabel.setFont(font);
        inputLabel.setCharacterSize(45);
        inputLabel.setFillColor(Color::White);
        inputLabel.setString("WPISZ NICK:");
        inputLabel.setPosition(225, 285);

        nickText.setFont(font);
        nickText.setCharacterSize(45);
        nickText.setFillColor(Color::Yellow);
        nickText.setPosition(225, 345);

        instruction.setFont(font);
        instruction.setCharacterSize(30);
        instruction.setFillColor(Color(200, 200, 200));
        instruction.setPosition(25, 585);

        victory = false;
        finalScore = 0;
    }

    void start(bool isVictory, int score)
    {
        victory = isVictory;
        finalScore = score;
        nickInput.clear();

        if (victory)
        {
            titleText.setString("WYGRANA!");
            instruction.setString("ENTER - zatwierdz wynik");
        }
        else
        {
            titleText.setString("GAME OVER");
            instruction.setString("ENTER / SPACJA - powrot do menu");
        }
        
        centerX(titleText, 720);

        scoreText.setString("TWÓJ WYNIK: " + std::to_string(score));
    }

    // zwraca true → powrót do menu
    bool handleEvent(Event& event)
    {
        // GAME OVER → szybki powrót
        if (!victory)
        {
            if (event.type == Event::KeyPressed &&
                (event.key.code == Keyboard::Enter || event.key.code == Keyboard::Space))
            {
                return true;
            }
            return false;
        }

        // WYGRANA → wpisywanie nicku
        if (event.type == Event::TextEntered)
        {
            if (event.text.unicode == 8)
            {
                if (!nickInput.empty())
                    nickInput.pop_back();
            }
            else if (nickInput.size() < 12 && event.text.unicode < 128)
            {
                nickInput.push_back(static_cast<char>(event.text.unicode));
            }
        }

        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
        {
            if (!nickInput.empty())
            {
                scoreManager.saveScore(nickInput, finalScore);
                return true;
            }
        }

        return false;
    }

    void draw(RenderWindow& window)
    {
        nickText.setString(nickInput);

        window.draw(titleText);
        window.draw(scoreText);

        if (victory)
        {
            window.draw(inputLabel);
            window.draw(nickText);
        }

        window.draw(instruction);
    }

    void centerX(Text& t, float windowWidth)
    {
        FloatRect box = t.getLocalBounds();
        t.setOrigin(box.left + box.width / 2.f, t.getOrigin().y);
        t.setPosition(windowWidth / 2.f, t.getPosition().y);
    }
};
