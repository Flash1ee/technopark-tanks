#include "text_event.h"

void TextEvent::SetTimer(sf::Time timer) {
    this->timer = timer;
}


sf::Time& TextEvent::GetTimer() {
    return timer;
}


TextEvent::TextEvent(textEvents event, int count) {
    if (!this->font.loadFromFile(T_FONT)) {
        throw std::exception();
    };
    text.setColor(sf::Color::Red);
    text.setFont(font);
    timer.Zero;
    type = event;
    switch (type)
    {
        case BOT_KILL: {
            string << "KILL " << count << " BOTS!!!";
            text.setString(string.str());
            break;
        }
        case DESTROY: {
            text.setString("DESTROY BASE!!!");
            break;
        }
        case BOSS_APPEAR: {
            text.setString("BOSS APPEARED!!!");
            break;
        }
    }
}


double TextEvent::update(sf::RenderWindow& window, sf::Time main_timer) {
    text.setPosition(window.getView().getCenter().x - 120, window.getView().getCenter().y - 30);
    text.setOutlineThickness(2);
    switch (type)
    {
        case BOT_KILL: {
            if (main_timer.asSeconds() > 4) {
                return ST;
            }
            break;
        }
        case DESTROY: {
            if (main_timer.asSeconds() - timer.asSeconds() > 4) {
                return ST;
            }
            break;
        }
        case BOSS_APPEAR: {
            if ((main_timer.asSeconds() - timer.asSeconds() > 4) || timer == sf::Time::Zero) {
                return ST;
            }
            break;
        }
    }
    window.draw(text);
    return PL;
}