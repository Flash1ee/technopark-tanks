#include "statistics.h"

Statistic::Statistic(sf::RenderWindow& window) {
    if (!this->font.loadFromFile(FONT)) {
        throw std::exception();
    };
    // std::cout << "CONSTR" << '\n';
    // std::cout << window.getView().getViewport().top << '\n';
    // std::cout << window.getView().getViewport().left << '\n';
    text.resize(4);
    text[0].setFont(font);
    text[0].setColor(sf::Color::Red);
    text[0].setCharacterSize(12);
    text[0].setString("Health: ");

    text[1].setFont(font);
    text[1].setColor(sf::Color::Red);
    text[1].setCharacterSize(12);

    text[2].setFont(font);
    text[2].setColor(sf::Color::Red);
    text[2].setString("Time: ");
    text[2].setCharacterSize(12);

    text[3].setFont(font);
    text[3].setColor(sf::Color::Red);
    text[3].setCharacterSize(12);
}

void Statistic::update(sf::RenderWindow& window, int hp, double time) {
    std::ostringstream str_hp, str_time;
    str_time.precision(2);
    str_hp << hp;
    str_time << std::fixed << time;

    text[0].setPosition(window.getView().getCenter().x + window.getView().getSize().x / 2 - 100, window.getView().getCenter().y + window.getView().getSize().y / 2 - text[0].getCharacterSize() - 5);
    text[1].setPosition(window.getView().getCenter().x + window.getView().getSize().x / 2 - (text[1].getCharacterSize() / 1.4) * text[1].getString().getSize(), window.getView().getCenter().y + window.getView().getSize().y / 2 - text[1].getCharacterSize() - 5);
    text[2].setPosition(window.getView().getCenter().x + window.getView().getSize().x / 2 - 85, window.getView().getCenter().y - window.getView().getSize().y / 2);
    text[3].setPosition(window.getView().getCenter().x + window.getView().getSize().x / 2 - 50, window.getView().getCenter().y - window.getView().getSize().y / 2);

    text[1].setString(str_hp.str());
    text[3].setString(str_time.str());
}

void Statistic::draw(sf::RenderWindow& window) {
    for (int i = 0; i < text.size(); i++) {
        window.draw(text[i]);
    }
}