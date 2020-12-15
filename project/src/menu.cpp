#include "menu.h"

Menu::Menu(MenuSelector selector, sf::RenderWindow& window) {
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
        case MenuSelector::MAIN: {
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

        case MenuSelector::GAME: {
            break;
        }

        default:
            throw MenuSelector::ERROR;
    }
    buttons[selectedIndex].setColor(sf::Color::Red);
}

int Menu::show(sf::RenderWindow& window) {
    this->music.play();
    while (window.isOpen()) {
        this->draw(window);
        window.display();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
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

                    case sf::Keyboard::Enter: {
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

