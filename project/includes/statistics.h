#pragma once 

#include <vector>
#include <iostream>
#include <string>
#include <exception>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "objects.h"
#define FONT "../resources/19888.ttf"

class Statistic {
    private:
        sf::Font font;
        std::vector <sf::Text> text;
        sf::Time time;
    public:
        Statistic(sf::RenderWindow& window);
        void update(sf::RenderWindow& window, int hp, double time);
        void draw(sf::RenderWindow& window);
};