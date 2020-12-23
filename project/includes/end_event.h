#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#define WIN "../resources/victory.png"
#define WASTED "../resources/wasted1.png"
#define WASTED_MUSIC "../sounds/wasted.ogg"
#define WIN_MUSIC "../sounds/win.ogg"

constexpr int STOP = -1;
constexpr int PLAY = 0;

enum endEvents{
    G_LOSE,
    G_VICTORY,
    G_COUNT,
    G_END
};

class EndEvent {
    private:
        endEvents type;
        sf::Time timer;
        sf::Music music;
        sf::Texture texture;
        sf::Sprite sprite;
        EndEvent();
    public:
        void SetTimer(sf::Time timer);
        sf::Time& GetTimer();
        EndEvent(endEvents event);
        double update(sf::RenderWindow& window, sf::View& view, sf::Time main_timer);
        void draw(sf::RenderWindow& window);
};



