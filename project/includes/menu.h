#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <exception>
#include <SFML/Graphics.hpp>
#include "game_session.hpp"

#define SINGLE 0
#define MULTI 1
#define MENU_ERROR -1000
#define MAIN_COUNT 4
#define GAME_COUNT 3
#define FONT_PATH "../resources/20085.ttf"
#define BACKGROUND_PATH "../resources/background.jpg"

enum class MenuSelector {
    MAIN,
    GAME,
    COUNT,
    ERROR
};


struct Button {
    std::string text;
    sf::Color color;
};

const inline Button mainMenu[MAIN_COUNT] {
    {.text = "Single play", .color = sf::Color::White},
    {.text = "Multiplayer", .color = sf::Color::White},
    {.text = "Top results", .color = sf::Color::White},
    {.text = "Exit", .color = sf::Color::White},
};

const inline Button gameMenu[GAME_COUNT] {
    {.text = "Countinue", .color = sf::Color::White},
    {.text = "Back to main menu", .color = sf::Color::White},
    {.text = "Exit", .color = sf::Color::White},
};

class Menu {
    private:
        int selectedIndex;
        sf::Font font;
        std::vector <sf::Text> buttons;
        sf::Sprite background;
    public:
        Menu(MenuSelector selector, sf::RenderWindow& window);
        void moveUp();
        void moveDown();
        void checkKeyboard();
        int show(sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);
};