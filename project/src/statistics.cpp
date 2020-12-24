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

    text_bar.resize(6);

    text_bar[0].loadFromFile(BAR0);
    text_bar[1].loadFromFile(BAR1);
    text_bar[2].loadFromFile(BAR2);
    text_bar[3].loadFromFile(BAR3);
    text_bar[4].loadFromFile(BAR4);
    text_bar[5].loadFromFile(BAR5);

    // bar[i].setTextureRect(rect_bar);

    text.resize(2);

    text[0].setFont(font);
    text[0].setColor(sf::Color::Red);
    text[0].setString("Time: ");
    text[0].setOutlineThickness(1.5);
    text[0].setCharacterSize(12);

    text[1].setFont(font);
    text[1].setColor(sf::Color::Red);
    text[1].setCharacterSize(12);
    text[1].setOutlineThickness(1.5);
}

void Statistic::update(sf::RenderWindow& window, int player_hp, 
                        DestructibleWalls& walls, double time) {
    std::ostringstream str_time;
    if (time == 0) {
        str_time.precision(2);
        str_time << std::fixed << this->time;
    } else {
        this->time = time;
        str_time.precision(2);
        str_time << std::fixed << time;
    }

    health[0].setPosition(window.getView().getCenter().x - window.getView().getSize().x / 2, window.getView().getCenter().y - window.getView().getSize().y / 2 - 1);
    health[1].setPosition(window.getView().getCenter().x - window.getView().getSize().x / 2 + 14, window.getView().getCenter().y - window.getView().getSize().y / 2 - 1);
    health[2].setPosition(window.getView().getCenter().x - window.getView().getSize().x / 2 + 28, window.getView().getCenter().y - window.getView().getSize().y / 2 - 1);

    text[0].setPosition(window.getView().getCenter().x + window.getView().getSize().x / 2 - 85, window.getView().getCenter().y - window.getView().getSize().y / 2 - 4);
    text[1].setPosition(window.getView().getCenter().x + window.getView().getSize().x / 2 - 50, window.getView().getCenter().y - window.getView().getSize().y / 2 - 4);

    text[1].setString(str_time.str());
    
    if (walls.base_enemy[0]->getHp() > 80) {
        en_base_hp.setTexture(text_bar[5]);
        en_base_hp.setTextureRect(rect_bar);
    }
    if (walls.base_enemy[0]->getHp() <= 80 && walls.base_enemy[0]->getHp() > 60) {
        en_base_hp.setTexture(text_bar[4]);
        en_base_hp.setTextureRect(rect_bar);
    }
    if (walls.base_enemy[0]->getHp() <= 60 && walls.base_enemy[0]->getHp() > 40) {
        en_base_hp.setTexture(text_bar[3]);
        en_base_hp.setTextureRect(rect_bar);
    }
    if (walls.base_enemy[0]->getHp() <= 40 && walls.base_enemy[0]->getHp() > 20) {
        en_base_hp.setTexture(text_bar[2]);
        en_base_hp.setTextureRect(rect_bar);
    }
    if (walls.base_enemy[0]->getHp() <= 20 && walls.base_enemy[0]->getHp() > 0) {
        en_base_hp.setTexture(text_bar[1]);
        en_base_hp.setTextureRect(rect_bar);
    }
    if (walls.base_enemy[0]->getHp() <= 0) {
        en_base_hp.setTexture(text_bar[0]);
        en_base_hp.setTextureRect(rect_bar);
    }

    if (walls.base_player[0]->getHp() > 80) {
        pl_base_hp.setTexture(text_bar[5]);
        pl_base_hp.setTextureRect(rect_bar);
    }
    if (walls.base_player[0]->getHp() <= 80 && walls.base_player[0]->getHp() > 60) {
        pl_base_hp.setTexture(text_bar[4]);
        pl_base_hp.setTextureRect(rect_bar);
    }
    if (walls.base_player[0]->getHp() <= 60 && walls.base_player[0]->getHp() > 40) {
        pl_base_hp.setTexture(text_bar[3]);
        pl_base_hp.setTextureRect(rect_bar);
    }
    if (walls.base_player[0]->getHp() <= 40 && walls.base_player[0]->getHp() > 20) {
        pl_base_hp.setTexture(text_bar[2]);
        pl_base_hp.setTextureRect(rect_bar);
    }
    if (walls.base_player[0]->getHp() <= 20 && walls.base_player[0]->getHp() > 0) {
        pl_base_hp.setTexture(text_bar[1]);
        pl_base_hp.setTextureRect(rect_bar);
    }
    if (walls.base_player[0]->getHp() <= 0) {
        pl_base_hp.setTexture(text_bar[0]);
        pl_base_hp.setTextureRect(rect_bar);
    }

    if ((player_hp < 100) && (player_hp > 83)) {
        health[2].setTextureRect(full);
        health[1].setTextureRect(full);
        health[0].setTextureRect(full);
    }
    if ((player_hp <= 83) && (player_hp >= 68)) {
        health[2].setTextureRect(half);
        health[1].setTextureRect(full);
        health[0].setTextureRect(full);
    }
    if ((player_hp < 68) && (player_hp >= 52)) {
        health[2].setTextureRect(empty);
        health[1].setTextureRect(full);
        health[0].setTextureRect(full);
    }    
    if ((player_hp < 52) && (player_hp >= 36)) {
        health[2].setTextureRect(empty);
        health[1].setTextureRect(half);
        health[0].setTextureRect(full);
    }
    if ((player_hp < 36) && (player_hp >= 20)) {
        health[2].setTextureRect(empty);
        health[1].setTextureRect(empty);
        health[0].setTextureRect(full);
    }
    if ((player_hp < 20) && (player_hp > 0)) {
        health[2].setTextureRect(empty);
        health[1].setTextureRect(empty);
        health[0].setTextureRect(half);
    }
    if (player_hp <= 0) {
        health[2].setTextureRect(empty);
        health[1].setTextureRect(empty);
        health[0].setTextureRect(empty);
    }
    
    en_base_hp.setScale(0.25, 0.25);
    pl_base_hp.setScale(0.25, 0.25);

    pl_base_hp.setPosition(walls.base_player[0]->coords.x - 23, walls.base_player[0]->coords.y + 17);
    en_base_hp.setPosition(walls.base_player[0]->coords.x - 23, 2);
}

void Statistic::draw(sf::RenderWindow& window) {
    window.draw(pl_base_hp);
    window.draw(en_base_hp);
    for (int i = 0; i < text.size(); i++) {
        window.draw(text[i]);
    }
    for (int i = 0; i < health.size(); i++) {
        window.draw(health[i]);
    }
}