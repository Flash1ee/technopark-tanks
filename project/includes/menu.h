#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <exception>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "game_session.hpp"
#include "client_server_config.h"

#define SINGLE 100
#define MULTI 200
#define CONTINUE 300
#define MENU_ERROR -1000
#define STOP_RUN -245
#define MAIN_COUNT 4
#define GAME_COUNT 3
#define FONT_PATH "../resources/20085.ttf"
#define BACKGROUND_PATH "../resources/background.jpg"
#define MUSIC_PATH "../resources/menu_music.ogg"


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
    {.text = "Exit", .color = sf::Color::White}
};

const inline Button gameMenu[GAME_COUNT] {
    {.text = "Countinue", .color = sf::Color::White},
    {.text = "Show current stats", .color = sf::Color::White},
    {.text = "Back to main menu", .color = sf::Color::White}
};

class Menu {
    private:
        int selectedIndex;
        sf::Font font;
        std::vector <sf::Text> buttons;
        sf::Sprite background;
        sf::Texture bg;
        sf::Music music;
    public:
        ~Menu() {};
        Menu(MenuSelector selector, sf::RenderWindow& window);
        Menu(int selector, sf::RenderWindow& window);
        void moveUp();
        void moveDown();
        int menuSwitcher(sf::RenderWindow& window, std::string map_skin, std::string player_skin, std::string server_ip);
        int show(sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);
};
