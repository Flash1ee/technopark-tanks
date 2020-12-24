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
#define FIRST 1000
#define SECOND 2000
#define THIRD 3000
#define FORTH 4000
#define MULTI 200
#define CONTINUE 300
#define MENU_ERROR -1000
#define STOP_RUN -245
#define MAIN_COUNT 4
#define GAME_COUNT 3
#define CHARACTER_COUNT 4
#define FONT_PATH "../resources/20085.ttf"
#define BACKGROUND_PATH "../resources/background.jpg"
#define BACKGROUND1_PATH "../resources/background1.jpg"
#define MUSIC_PATH "../resources/menu_music.ogg"
#define PAUSE_MUSIC_PATH "../resources/pause_music.ogg"
#define MORTAL_PATH "../resources/mortal.ogg"
#define CHOOSE_MUSIC_PATH "../resources/character.ogg"

const inline sf::IntRect pl_rects [4] {
    {1, 2, 13, 13},
    {209, 128, 13, 13},
    {193, 129, 13, 13},
    {224, 129, 13, 13},
};

enum class MenuSelector {
    MAIN,
    GAME,
    CHARACTER,
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

const inline Button charMenu[CHARACTER_COUNT] {
    {.text = "Le Tank", .color = sf::Color::White},
    {.text = "Kolobok Ivanich", .color = sf::Color::White},
    {.text = "Green bomba*s", .color = sf::Color::White},
    {.text = "EL PROBLEMA", .color = sf::Color::White}
};

class Menu {
    private:
        int selectedIndex;
        sf::Font font;
        std::vector <sf::Text> buttons;
        std::vector <sf::Sprite> buttons_img;
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
