#include "statistics.h"

Statistic::Statistic(sf::RenderWindow& window) {
    if (!this->font.loadFromFile(FONT)) {
        throw std::exception();
    };

    health_img.loadFromFile(HEALTH);
    health_text.loadFromImage(health_img);
    health.resize(3);
    for (int i = 0; i < health.size(); i++) {
        health[i].setTexture(health_text);
        health[i].setTextureRect(full);
        health[i].setScale(0.1, 0.1);
    }

    text.resize(2);

    text[0].setFont(font);
    text[0].setColor(sf::Color::Red);
    text[0].setString("Time: ");
    text[0].setCharacterSize(12);

    text[1].setFont(font);
    text[1].setColor(sf::Color::Red);
    text[1].setCharacterSize(12);
}

void Statistic::update(sf::RenderWindow& window, int hp, double time) {
    std::ostringstream str_time;
    str_time.precision(2);
    str_time << std::fixed << time;

    health[0].setPosition(window.getView().getCenter().x - window.getView().getSize().x / 2, window.getView().getCenter().y - window.getView().getSize().y / 2 - 1);
    health[1].setPosition(window.getView().getCenter().x - window.getView().getSize().x / 2 + 14, window.getView().getCenter().y - window.getView().getSize().y / 2 - 1);
    health[2].setPosition(window.getView().getCenter().x - window.getView().getSize().x / 2 + 28, window.getView().getCenter().y - window.getView().getSize().y / 2 - 1);

    text[0].setPosition(window.getView().getCenter().x + window.getView().getSize().x / 2 - 85, window.getView().getCenter().y - window.getView().getSize().y / 2 - 4);
    text[1].setPosition(window.getView().getCenter().x + window.getView().getSize().x / 2 - 50, window.getView().getCenter().y - window.getView().getSize().y / 2 - 4);

    text[1].setString(str_time.str());

    if ((hp < 100) && (hp > 83)) {
        health[2].setTextureRect(full);
        health[1].setTextureRect(full);
        health[0].setTextureRect(full);
    }
    if ((hp <= 83) && (hp >= 68)) {
        health[2].setTextureRect(half);
        health[1].setTextureRect(full);
        health[0].setTextureRect(full);
    }
    if ((hp < 68) && (hp >= 52)) {
        health[2].setTextureRect(empty);
        health[1].setTextureRect(full);
        health[0].setTextureRect(full);
    }    
    if ((hp < 52) && (hp >= 36)) {
        health[2].setTextureRect(empty);
        health[1].setTextureRect(half);
        health[0].setTextureRect(full);
    }
    if ((hp < 36) && (hp >= 20)) {
        health[2].setTextureRect(empty);
        health[1].setTextureRect(empty);
        health[0].setTextureRect(full);
    }
    if ((hp < 20) && (hp > 0)) {
        health[2].setTextureRect(empty);
        health[1].setTextureRect(empty);
        health[0].setTextureRect(half);
    }
    if (hp <= 0) {
        health[2].setTextureRect(empty);
        health[1].setTextureRect(empty);
        health[0].setTextureRect(empty);
    }
    
}

void Statistic::draw(sf::RenderWindow& window) {
    for (int i = 0; i < text.size(); i++) {
        window.draw(text[i]);
    }
    for (int i = 0; i < health.size(); i++) {
        window.draw(health[i]);
    }
}