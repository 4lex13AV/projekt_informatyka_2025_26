#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;

class Menu
{
private:
    Font font;

    std::vector<Text> opcje;
    int wybrane;

    Text title;

    // ekran sterowania
    Text sterowanieTytul;
    Text sterowanieOpis;
    bool trybSterowanie = false;

public:

    Menu(float windowWidth, float windowHeight)
    {
        wybrane = 0;

        font.loadFromFile("textures/pixelfont.ttf");

        title.setFont(font);
        title.setString("ARKANOID");
        title.setCharacterSize(150);
        title.setFillColor(Color::Cyan);

        float titleX = windowWidth / 2.f - title.getLocalBounds().width / 2.f;
        title.setPosition(titleX, 0);

        // 4 opcje menu
        opcje.resize(4);

        std::string nazwy[4] = { "GRA", "WYNIKI", "STEROWANIE", "WYJSCIE" };

        for (int i = 0; i < 4; i++)
        {
            opcje[i].setFont(font);
            opcje[i].setString(nazwy[i]);
            opcje[i].setCharacterSize(60);
            opcje[i].setFillColor(Color::White);

            float x = windowWidth / 2.f - opcje[i].getLocalBounds().width / 2.f;
            float y = 225 + i * 80;

            opcje[i].setPosition(x, y);
        }

        // Teksty do ekranu STEROWANIE
        sterowanieTytul.setFont(font);
        sterowanieTytul.setString("STEROWANIE");
        sterowanieTytul.setCharacterSize(50);
        sterowanieTytul.setFillColor(Color::Yellow);
        sterowanieTytul.setPosition(100, 40);

        sterowanieOpis.setFont(font);
        sterowanieOpis.setCharacterSize(26);
        sterowanieOpis.setFillColor(Color::White);

        sterowanieOpis.setString(
            "Menu:\n"
            "   STRZALKI GORA/DOL - nawigacja\n"
            "   ENTER - zatwierdz\n\n"
            "Gra:\n"
            "   A / STRZALKA LEWO  - ruch w lewo\n"
            "   D / STRZALKA PRAWO - ruch w prawo\n"
            "   SPACJA - start pilki\n\n"
            "ESC - powrot"
        );

        sterowanieOpis.setPosition(50, 140);

        zaznaczOpcje(0);
    }

    // RYSOWANIE MENU
    void draw(RenderWindow& window)
    {
        if (trybSterowanie)
        {
            window.draw(sterowanieTytul);
            window.draw(sterowanieOpis);
            return;
        }

        window.draw(title);

        for (auto& t : opcje)
            window.draw(t);
    }

    // WEJŒCIE DO TRYBU STEROWANIA
    void wlaczSterowanie()
    {
        trybSterowanie = true;
    }

    void wylaczSterowanie()
    {
        trybSterowanie = false;
    }

    bool czySterowanie() const
    {
        return trybSterowanie;
    }

    // ZMIANA OPCJI
    void moveUp()
    {
        wybrane--;
        if (wybrane < 0)
            wybrane = (int)opcje.size() - 1;

        aktualizujKolory();
    }

    void moveDown()
    {
        wybrane++;
        if (wybrane >= opcje.size())
            wybrane = 0;

        aktualizujKolory();
    }

    int getWybrane() const
    {
        return wybrane;
    }

    int handleInput(const sf::Event& event)
    {
        // EKRAN STEROWANIA
        if (trybSterowanie)
        {
            if (event.type == Event::KeyPressed &&
                event.key.code == Keyboard::Escape)
            {
                trybSterowanie = false;
            }
            return -1;
        }

        // MENU G£ÓWNE
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Up)      moveUp();
            else if (event.key.code == sf::Keyboard::Down) moveDown();
            else if (event.key.code == sf::Keyboard::Enter)
            {
                if (wybrane == 2) // opcja "STEROWANIE"
                {
                    wlaczSterowanie();
                    return -1;
                }

                return wybrane;
            }
        }

        return -1;
    }

private:

    void zaznaczOpcje(int index)
    {
        wybrane = index;
        aktualizujKolory();
    }

    void aktualizujKolory()
    {
        for (int i = 0; i < opcje.size(); i++)
        {
            opcje[i].setFillColor(i == wybrane ? Color::Yellow : Color::White);
        }
    }
};
