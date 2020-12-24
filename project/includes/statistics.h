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
#define BAR0 "../resources/hpbar0.png"
#define BAR1 "../resources/hpbar1.png"
#define BAR2 "../resources/hpbar2.png"
#define BAR3 "../resources/hpbar3.png"
#define BAR4 "../resources/hpbar4.png"
#define BAR5 "../resources/hpbar5.png"

const inline sf::IntRect full(50, 23, 121, 121);
const inline sf::IntRect half(195, 23, 121, 121);
const inline sf::IntRect empty(340, 23, 121, 121);

const inline sf::IntRect rect_bar(47, 69, 245, 49);

class Statistic {
    private:
        sf::Image health_img;
        sf::Texture health_text;
        sf::Font font;
        std::vector <sf::Text> text;
        std::vector <sf::Texture> text_bar;
        double time;
        std::vector <sf::Sprite> health;
        sf::Sprite pl_base_hp;
        sf::Sprite en_base_hp;
    public:
        Statistic(sf::RenderWindow& window);
        void update(sf::RenderWindow& window, int player_hp, DestructibleWalls& walls, double time);
        void draw(sf::RenderWindow& window);
};