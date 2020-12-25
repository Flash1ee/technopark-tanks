#include "menu.h"

Menu::Menu(int selector, sf::RenderWindow& window) {
    if (!this->font.loadFromFile(FONT_PATH)) {
        throw std::exception();
    };
    selectedIndex = 0;
    switch (selector) {
        case 0: {
            if (!bg.loadFromFile(BACKGROUND_PATH)) {
                throw std::exception();
            };
            this->background.setTexture(bg);
            background.setPosition(0, 0);
            if (!music.openFromFile(MUSIC_PATH)) {
                throw std::exception();
            };
            this->buttons.resize(MAIN_COUNT);
            auto height = window.getSize().y / this->buttons.size();
            for (int i = 0; i < this->buttons.size(); i++) {
                buttons[i].setColor(mainMenu[i].color);
                buttons[i].setFont(font);
                buttons[i].setString(mainMenu[i].text);
                buttons[i].setPosition(100, 130 + height * (i));
            }
            break;
        }

        case 1: {
            if (!bg.loadFromFile(BACKGROUND1_PATH)) {
                throw std::exception();
            };
            this->background.setTexture(bg);
            background.setPosition(0, 0);
            if (!music.openFromFile(PAUSE_MUSIC_PATH)) {
                throw std::exception();
            };
            this->buttons.resize(GAME_COUNT);
            auto height = window.getSize().y / this->buttons.size();
            for (int i = 0; i < this->buttons.size(); i++) {
                buttons[i].setColor(gameMenu[i].color);
                buttons[i].setFont(font);
                buttons[i].setString(gameMenu[i].text);
                buttons[i].setPosition(100, 200 + height * (i));
            }
            break;
        }

        case 2: {
            if (!bg.loadFromFile(DEVYATKA_PATH)) {
                throw std::exception();
            };
            this->background.setTexture(bg);
            background.setPosition(0, 0);
            if (!music.openFromFile(CHOOSE_MUSIC_PATH)) {
                throw std::exception();
            };
            this->buttons.resize(CHARACTER_COUNT);
            auto height = window.getSize().y / this->buttons.size();
            for (int i = 0; i < this->buttons.size(); i++) {
                buttons[i].setColor(charMenu[i].color);
                buttons[i].setFont(font);
                buttons[i].setString(charMenu[i].text);
                buttons[i].setPosition(window.getSize().x - 400, 80 + height * (i));
            }
            this->buttons_texture.resize(1);
            this->buttons_sprite.resize(CHARACTER_COUNT);
            buttons_texture[0].loadFromFile(OBJECT_IMAGE);
            for (int i = 0; i < buttons_sprite.size(); i++) {
                buttons_sprite[i].setTexture(buttons_texture[0]);
                buttons_sprite[i].setTextureRect(pl_rects[i]);
                buttons_sprite[i].setPosition(buttons[i].getPosition().x - 200, buttons[i].getPosition().y - 40);
                buttons_sprite[i].setScale(10, 10);
            }
            break;
        }

        case 3: {
            if (!bg.loadFromFile(DEVYATKA_PATH)) {
                throw std::exception();
            };
            this->background.setTexture(bg);
            background.setPosition(0, 0);
            // if (!music.openFromFile(CHOOSE_MUSIC_PATH)) {
            //     throw std::exception();
            // };
            this->buttons.resize(MAP_COUNT);
            auto height = window.getSize().y / this->buttons.size();
            for (int i = 0; i < this->buttons.size(); i++) {
                buttons[i].setColor(mapMenu[i].color);
                buttons[i].setFont(font);
                buttons[i].setString(mapMenu[i].text);
                // buttons[i].setPosition(window.getSize().x - win, 300);
            }
            buttons[0].setPosition(400, 300);
            buttons[1].setPosition(1400, 300);
            this->buttons_texture.resize(MAP_COUNT);
            buttons_texture[0].loadFromFile(TEXT_MAP_ONE);
            buttons_texture[1].loadFromFile(TEXT_MAP_TWO);
            this->buttons_sprite.resize(MAP_COUNT);
            buttons_sprite[0].setTexture(buttons_texture[0]);
            buttons_sprite[1].setTexture(buttons_texture[1]);
            buttons_sprite[0].setTextureRect(map_rects[0]);
            buttons_sprite[0].setPosition(300, 400);
            buttons_sprite[1].setPosition(1250, 400);
            buttons_sprite[1].setTextureRect(map_rects[1]);
            music.openFromFile(CHOOSE_MAP_MUSIC_PATH);
            break;
        }

        default:
            throw MenuSelector::ERROR;
    }
    buttons[selectedIndex].setColor(sf::Color::Red);
}

int Menu::menuSwitcher(sf::RenderWindow& window, std::string player_skin, std::string server_ip) {
    switch (this->show(window))
        {
            case T_D: {
                std::string map = MAP_ONE;
                GameSession game_session("Tanks", map, player_skin, false);
                music.pause();
                if (game_session.Run(pl_rects[0]) == STOP_RUN) {
                    return STOP_RUN;
                };
                break;
            }

            case T_G: {
                std::string map = MAP_TWO;
                GameSession game_session("Tanks", map, player_skin, false);
                music.pause();
                if (game_session.Run(pl_rects[0]) == STOP_RUN) {
                    return STOP_RUN;
                };
                break;
            }

            case I_D: {
                std::string map = MAP_ONE;
                GameSession game_session("Tanks", map, player_skin, false);
                music.pause();
                if (game_session.Run(pl_rects[1]) == STOP_RUN) {
                    return STOP_RUN;
                };
                break;
            }

            case I_G: {
                std::string map = MAP_TWO;
                GameSession game_session("Tanks", map, player_skin, false);
                music.pause();
                if (game_session.Run(pl_rects[1]) == STOP_RUN) {
                    return STOP_RUN;
                };
                break;
            }

            case K_D: {
                std::string map = MAP_ONE;
                GameSession game_session("Tanks", map, player_skin, false);
                music.pause();
                if (game_session.Run(pl_rects[2]) == STOP_RUN) {
                    return STOP_RUN;
                };
                break;
            }

            case K_G: {
                std::string map = MAP_TWO;
                GameSession game_session("Tanks", map, player_skin, false);
                music.pause();
                if (game_session.Run(pl_rects[2]) == STOP_RUN) {
                    return STOP_RUN;
                };
                break;
            }

            case C_D: {
                std::string map = MAP_ONE;
                GameSession game_session("Tanks", map, player_skin, false);
                music.pause();
                if (game_session.Run(pl_rects[3]) == STOP_RUN) {
                    return STOP_RUN;
                };
                break;
            }

            case C_G: {
                std::string map = MAP_TWO;
                GameSession game_session("Tanks", map, player_skin, false);
                music.pause();
                if (game_session.Run(pl_rects[3]) == STOP_RUN) {
                    return STOP_RUN;
                };
                break;
            }

            case MULTI: {
                std::string map = MAP_ONE;
                GameSession game_session("Tanks", map, player_skin, true,
                   server_ip, PORT);
                if (game_session.Run(pl_rects[1]) == STOP_RUN) {
                    return STOP_RUN;
                };
                break;
            }

            default: {
                // throw std::exception();
                break;
            }
        }
}


int Menu::show(sf::RenderWindow& window) {
    if (this->buttons[0].getString() != "Le Tank") {
        music.setVolume(30);
    }
    this->music.play();
    while (window.isOpen()) {
        this->draw(window);
        window.display();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(-10);
            }
            if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code)
                {
                    case sf::Keyboard::Down:
                    case sf::Keyboard::Right: {
                        this->moveDown();
                        break;
                    }

                    case sf::Keyboard::Up:
                    case sf::Keyboard::Left: {
                        this->moveUp();
                        break;
                    }

                    case sf::Keyboard::Return: {
                        std::string currbutton = this->buttons[this->selectedIndex].getString();
                        if (currbutton == "Exit") {
                            window.close();
                            exit(0);
                        }
                        if (currbutton == "Single play") {
                            Menu tanks(2, window);
                            tanks.music.setVolume(200);
                            return tanks.show(window);
                        }
                        if (currbutton == "Multiplayer") {
                            window.close();
                            return MULTI;
                        }
                        if (currbutton == "Countinue") {
                            // window.close();
                            return CONTINUE;
                        }
                        if (currbutton == "Back to main menu") {
                            window.close();
                            return STOP_RUN;
                        }
                        if (currbutton == "Show current stats") {
                            window.close();
                            return 0;
                        }
                        if (currbutton == "Le Tank") {
                            Menu maps(3, window);
                            maps.music.setVolume(200);
                            return maps.show(window) + TANK;
                        }
                        if (currbutton == "Kolobok Ivanich") {
                            Menu maps(3, window);
                            maps.music.setVolume(200);
                            return maps.show(window) + IVAN;
                        }
                        if (currbutton == "Green bombass") {
                            Menu maps(3, window);
                            maps.music.setVolume(200);
                            return maps.show(window) + KRIP;
                        }
                        if (currbutton == "EL PROBLEMA") {
                            Menu maps(3, window);
                            maps.music.setVolume(200);
                            return maps.show(window) + CAR;
                        }
                        if (currbutton == "Standart") {
                            window.close();
                            return DEFAULT;
                        }
                        if (currbutton == "Grass") {
                            window.close();
                            return GRAS;
                        }
                        break;
                    }

                }
            }
        }
    }
    return MENU_ERROR;
}

void Menu::draw(sf::RenderWindow& window) {
    window.clear();
    window.draw(background);
    for (int i = 0; i < buttons.size(); i++) {
        window.draw(buttons[i]);
        if (this->buttons_sprite.size() != 0) {
            window.draw(buttons_sprite[i]);
        }
    }
}

void Menu::moveUp() {
    if (this->selectedIndex != 0) {
        this->buttons[selectedIndex].setColor(sf::Color::White);
        this->selectedIndex--;
        this->buttons[selectedIndex].setColor(sf::Color::Red);
    }
}

void Menu::moveDown() {
    if (this->selectedIndex != this->buttons.size() - 1) {
        this->buttons[selectedIndex].setColor(sf::Color::White);
        this->selectedIndex++;
        this->buttons[selectedIndex].setColor(sf::Color::Red);
    }
}

