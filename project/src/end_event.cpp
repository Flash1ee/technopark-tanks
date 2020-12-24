#include "end_event.h"

EndEvent::EndEvent(endEvents event) {
    switch (event)
    {
        case G_LOSE: {
            if (!texture.loadFromFile(WASTED)) {
                throw std::exception();
            };
            sprite.setTexture(texture);
            sprite.setTextureRect(sf::IntRect( 3, 150, 390, 104));
            sprite.setScale(0.5, 0.5);
            music.openFromFile(WASTED_MUSIC);
            timer.Zero;
            type = G_LOSE;
            break;
        }

        case G_VICTORY: {
            if (!texture.loadFromFile(WIN)) {
                throw std::exception();
            };
            sprite.setTexture(texture);
            sprite.setTextureRect(sf::IntRect(124, 76, 698, 103));
            sprite.setScale(0.3, 0.3);
            music.openFromFile(WIN_MUSIC);
            timer.Zero;
            type = G_VICTORY;
            break;
        }

        default:
            break;
    }
}

double EndEvent::update(sf::RenderWindow& window, sf::View& view, sf::Time main_timer) {
    if (music.getStatus() != sf::SoundSource::Playing){
        music.play();
    }
    if (main_timer.asSeconds() - timer.asSeconds() > 6) {
        return STOP;
    } else {
        switch (type)
        {
            case G_LOSE: {
                sprite.setPosition(window.getView().getCenter().x - 90,
                                   window.getView().getCenter().y - 26);
                view.zoom(1.0001);
                break;
            }
            case G_VICTORY: {
                sprite.setPosition(window.getView().getCenter().x - 100,
                                   window.getView().getCenter().y - 51);
                break;
            }
        }
        window.draw(sprite);
    }
    return PLAY;
}

void EndEvent::SetTimer(sf::Time timer) {
    this->timer = timer;
}

sf::Time& EndEvent::GetTimer() {
    return this->timer;
}
