#pragma once

#include <vector>
#include <iostream>
#include <sstream> 
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#define T_FONT "../resources/19888.ttf"
constexpr int ST = -1;
constexpr int PL = 0;

enum textEvents {
    BOT_KILL,
    DESTROY,
    T_COUNT,
    T_END
};

class TextEvent {
    private:
        textEvents type;
        sf::Time timer;
        sf::Font font;
        sf::Music music;
        sf::Text text;
        std::ostringstream string;
        TextEvent();
    public:
        void SetTimer(sf::Time timer);
        sf::Time& GetTimer();
        TextEvent(textEvents event, int count);
        double update(sf::RenderWindow& window, sf::Time main_timer);
};