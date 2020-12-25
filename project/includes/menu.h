#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <exception>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "game_session.hpp"
#include "client_server_config.h"
#include "game.h"

#define TANK 100
#define IVAN 2023
#define KRIP 3056
#define CAR 40
#define DEFAULT 55
#define GRAS 777
#define T_D 155
#define T_G 877
#define I_D 2078
#define I_G 2800
#define K_D 3111
#define K_G 3833
#define C_D 95
#define C_G 817
#define MULTI 200
#define CONTINUE 300
#define MENU_ERROR -1000
#define STOP_RUN -245
#define MAIN_COUNT 4
#define GAME_COUNT 3
#define MAP_COUNT 2
#define CHARACTER_COUNT 4
#define TEXT_MAP_ONE "../resources/map_one_text.png"
#define TEXT_MAP_TWO "../resources/map_two_text.png"
#define FONT_PATH "../resources/20085.ttf"
#define BACKGROUND_PATH "../resources/background.jpg"
#define BACKGROUND1_PATH "../resources/background1.jpg"
#define DEVYATKA_PATH "../resources/devyat.jpg"
#define MUSIC_PATH "../resources/menu_music.ogg"
#define PAUSE_MUSIC_PATH "../resources/pause_music.ogg"
#define MORTAL_PATH "../resources/mortal.ogg"
#define CHOOSE_MUSIC_PATH "../resources/character.ogg"

const inline sf::IntRect pl_rects [CHARACTER_COUNT] {
    {1, 2, 13, 13},
    {209, 128, 13, 13},
    {193, 129, 13, 13},
    {224, 129, 13, 13},
};

const inline sf::IntRect map_rects [MAP_COUNT] {
    {0, 0, 429, 369},
    {0, 0, 429, 369},
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
    {.text = "Green bombass", .color = sf::Color::White},
    {.text = "EL PROBLEMA", .color = sf::Color::White}
};

const inline Button mapMenu[MAP_COUNT] {
    {.text = "Standart", .color = sf::Color::White},
    {.text = "Grass", .color = sf::Color::White},
};

class Menu {
    private:
        int selectedIndex;
        sf::Font font;
        std::vector <sf::Text> buttons;
        std::vector <sf::Sprite> buttons_sprite;
        std::vector <sf::Texture> buttons_texture;
        sf::Sprite background;
        sf::Texture bg;
        sf::Music music;
    public:
        ~Menu() {};
        Menu(MenuSelector selector, sf::RenderWindow& window);
        Menu(int selector, sf::RenderWindow& window);
        void moveUp();
        void moveDown();
        int menuSwitcher(sf::RenderWindow& window, std::string player_skin, std::string server_ip);
        int show(sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);
};
