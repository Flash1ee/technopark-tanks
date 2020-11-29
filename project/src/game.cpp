#include <SFML/Graphics.hpp>

#include "game_map.hpp"

#include "objects.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Tanks");
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    // image.loadFromFile("../textures/sprites.png");
    // image.createMaskFromColor(sf::Color(255, 255, 255));

    // texture.loadFromImage(image);

    // sprite.setTexture(texture);
    // sprite.setTextureRect(sf::IntRect(256, 0, 8, 8));
    // sprite.setPosition(5,5);

    PlayerTank playerTank("../textures/pltank.bmp", 100, 100, 2, 2, 29, 29);
    Map map("../maps/1.txt", "../textures/sprites.png");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        playerTank.actions();

        window.clear();
        // window.draw(sprite);
        map.drawMap(window);
        // window.draw(map.getSprite());
        window.draw(playerTank.getSprite());
        window.display();
    }
}
