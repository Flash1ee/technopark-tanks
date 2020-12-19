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
#define HEALTH "../resources/health.png"

const inline sf::IntRect full(50, 23, 121, 121);
const inline sf::IntRect half(195, 23, 121, 121);
const inline sf::IntRect empty(340, 23, 121, 121);

class Statistic {
    private:
        sf::Image health_img;
        sf::Texture health_text;
        sf::Font font;
        std::vector <sf::Text> text;
        sf::Time time;
        std::vector <sf::Sprite> health;
    public:
        Statistic(sf::RenderWindow& window);
        void update(sf::RenderWindow& window, int hp, double time);
        void draw(sf::RenderWindow& window);
};