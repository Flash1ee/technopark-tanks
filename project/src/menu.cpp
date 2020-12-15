#include "menu.h"

// Menu::Menu(MenuSelector selector, sf::RenderWindow& window) {
Menu::Menu(int selector, sf::RenderWindow& window) {
    if (!this->font.loadFromFile(FONT_PATH)) {
        throw std::exception();
    };
    if (!bg.loadFromFile(BACKGROUND_PATH)) {
        throw std::exception();
    };
    if (!music.openFromFile(MUSIC_PATH)) {
        throw std::exception();
    };
    this->background.setTexture(bg);
    background.setPosition(0, 0);
    selectedIndex = 0;
    switch (selector) {
        case 0: {
            this->buttons.resize(MAIN_COUNT);
            auto height = window.getSize().y / this->buttons.size();
            for (int i = 0; i < this->buttons.size(); i++) {
                buttons[i].setColor(mainMenu[i].color);
                buttons[i].setFont(font);
                buttons[i].setString(mainMenu[i].text);
                buttons[i].setPosition(100, 50 + height * (i));
            }
            break;
        }

        case 1: {
            this->buttons.resize(GAME_COUNT);
            auto height = window.getSize().y / this->buttons.size();
            for (int i = 0; i < this->buttons.size(); i++) {
                buttons[i].setColor(gameMenu[i].color);
                buttons[i].setFont(font);
                buttons[i].setString(gameMenu[i].text);
                buttons[i].setPosition(100, 50 + height * (i));
            }
            break;
        }

        default:
            throw MenuSelector::ERROR;
    }
    buttons[selectedIndex].setColor(sf::Color::Red);
}

int Menu::menuSwitcher(sf::RenderWindow& window, std::string map_skin, std::string player_skin, std::string server_ip) {
    switch (this->show(window))
        {
            case SINGLE: {
                GameSession game_session("Tanks", map_skin, player_skin, false);
                if (game_session.Run() == STOP_RUN) {
                    // std::cout <<"Returned stop run" <<'\n';
                    return STOP_RUN;
                };
                break;
            }

            case MULTI: {
                GameSession game_session("Tanks", map_skin, player_skin, true,
                   server_ip, PORT);
                if (game_session.Run() == STOP_RUN) {
                    return STOP_RUN;
                };
                break;
            }

            default: {
                throw std::exception();
                break;
            }
        }
}


int Menu::show(sf::RenderWindow& window) {
    // this->music.play();
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
                    case sf::Keyboard::Down: {
                        this->moveDown();
                        break;
                    }

                    case sf::Keyboard::Up: {
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
                            window.close();
                            this->music.stop();
                            return SINGLE;
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
                        break;
                    }

                }
            }
        }
    }
    return MENU_ERROR;
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(background);
    for (int i = 0; i < buttons.size(); i++) {
        window.draw(buttons[i]);
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

