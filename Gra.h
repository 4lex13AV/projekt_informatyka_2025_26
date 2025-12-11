#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Paletka.h"
#include "Pilka.h"
#include "Level.h"
#include "Menu.h"
#include "HUD.h"
#include "EndScreen.h"
#include "ScoreManager.h"

using namespace sf;
using namespace std;

Texture Blok::texFull;
Texture Blok::texHalf;

class Gra
{
private:

    const float SZEROKOSC = 720;
    const float WYSOKOSC = 640;

    RenderWindow window;

    enum class GameState {
        MENU,
        GRA,
        WYNIKI,
        KONIEC_ROZGRYWKI
    };

    GameState state = GameState::MENU;

    Menu menu;
    EndScreen endscreen;
    ScoreManager scoreManager;

    Font font;
    Text wynikiTitle;

    Paletka pal;
    Pilka pilka;
    HUD hud;
    Level level;

public:

    Gra()
        : window(VideoMode((unsigned)SZEROKOSC, (unsigned)WYSOKOSC),
            "PROJEKT ARKANOID - ALEKSANDER KRZEMINSKI"),
        menu(SZEROKOSC, WYSOKOSC),
        pal(360, 600, 140, 40, 8),
        pilka(320.f, 200.f, 5.f, 5.f, 8.f)
    {
        window.setFramerateLimit(60);

        Blok::loadTextures();
        level.generate(SZEROKOSC, WYSOKOSC);

        font.loadFromFile("textures/pixelfont.ttf");

        wynikiTitle.setFont(font);
        wynikiTitle.setString("TOP 10:");
        wynikiTitle.setCharacterSize(70);
        wynikiTitle.setFillColor(Color::White);
        wynikiTitle.setPosition(235, 0);
    }

    // G£ÓWNA PÊTLA GRY
    void run()
    {
        Event event;

        while (window.isOpen())
        {
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();

                obslugaMenu(event);
                obslugaWynikow(event);
                obslugaKoniec(event);
                obslugaStartuPilki(event);
            }

            rysujMenu();
            rysujWyniki();
            rysujKoniec();

            if (state == GameState::GRA)
                updateGra();

            if (state == GameState::GRA)
                rysujGra();
        }
    }

private:

    // OBS£UGA MENU
    void obslugaMenu(Event& event)
    {
        if (state != GameState::MENU) return;

        int wybor = menu.handleInput(event);

        if (wybor == 0)        // GRA
        {
            state = GameState::GRA;

            pilka.reset(pal);
            pilka.setStarted(false);

            level.generate(SZEROKOSC, WYSOKOSC);
            hud.reset();
        }
        else if (wybor == 1)   // WYNIKI
        {
            state = GameState::WYNIKI;
        }
        else if (wybor == 2)   // STEROWANIE - ju¿ obs³uguje Menu
        {
        }
        else if (wybor == 3)   // WYJŒCIE
        {
            window.close();
            exit(0);
        }
    }

    // EKRAN WYNIKÓW
    void obslugaWynikow(Event& event)
    {
        if (state != GameState::WYNIKI) return;

        if (event.type == Event::KeyPressed &&
            event.key.code == Keyboard::Escape)
        {
            state = GameState::MENU;
        }
    }

    // GAME OVER / WYGRANA
    void obslugaKoniec(Event& event)
    {
        if (state != GameState::KONIEC_ROZGRYWKI) return;

        if (endscreen.handleEvent(event))
        {
            state = GameState::MENU;
        }
    }

    // START PI£KI
    void obslugaStartuPilki(Event& event)
    {
        if (state != GameState::GRA) return;

        if (event.type == Event::KeyPressed &&
            event.key.code == Keyboard::Space)
        {
            if (!pilka.isStarted())
            {
                pilka.setStarted(true);
                hud.startTimer();
            }
        }
    }

    // RYSOWANIE MENU
    void rysujMenu()
    {
        if (state != GameState::MENU) return;

        window.clear(Color(30, 30, 40));
        menu.draw(window);
        window.display();
    }

    // RYSOWANIE WYNIKÓW
    void rysujWyniki()
    {
        if (state != GameState::WYNIKI) return;

        window.clear(Color(30, 30, 40));
        window.draw(wynikiTitle);

        auto wyniki = scoreManager.loadScores();

        float y = 90;
        for (int i = 0; i < wyniki.size() && i < 10; i++)
        {
            Text t(wyniki[i].name + " - " + to_string(wyniki[i].score), font, 35);
            t.setFillColor(Color::White);
            t.setPosition(230, y);
            y += 35;
            window.draw(t);
        }

        window.display();
    }

    // RYSOWANIE KOÑCA GRY
    void rysujKoniec()
    {
        if (state != GameState::KONIEC_ROZGRYWKI) return;

        window.clear(Color(30, 30, 40));
        endscreen.draw(window);
        window.display();
    }

    // UPDATE GRY
    void updateGra()
    {
        // sterowanie paletk¹
        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
            pal.moveLeft();

        if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
            pal.moveRight();

        pal.clampToBounds(SZEROKOSC);

        // PI£KA
        pilka.move(pal);
        pilka.collideWalls(SZEROKOSC, WYSOKOSC);

        for (auto& b : level.getBloki())
            if (pilka.collideBlock(b))
                if (b.hit())
                    hud.dodajPunkty(50, SZEROKOSC);

        pilka.collidePaddle(pal);

        // STRATA ¯YCIA
        if (pilka.getY() - pilka.getRadius() > WYSOKOSC)
        {
            hud.utrataZycia();

            if (hud.getZycia() <= 0)
            {
                endscreen.start(false, hud.getPunkty());
                state = GameState::KONIEC_ROZGRYWKI;
            }

            pilka.reset(pal);
        }

        // WYGRANA
        bool saBloki = false;
        for (auto& b : level.getBloki())
            if (b.isAlive())
                saBloki = true;

        if (!saBloki)
        {
            endscreen.start(true, hud.getPunkty());
            state = GameState::KONIEC_ROZGRYWKI;
        }

        hud.updateTimer();
    }

    // RYSOWANIE GRY
    void rysujGra()
    {
        window.clear(Color(30, 30, 40));

        pal.draw(window);
        level.draw(window);
        pilka.draw(window);
        hud.draw(window);

        window.display();
    }
};
