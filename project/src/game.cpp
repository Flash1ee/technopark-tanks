#include <SFML/Graphics.hpp>
#include "game.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Tanks");
    Player player(playerTankImage, 100, 100);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
                window.close();
            }
		}
        
        makeAction(player);

        window.clear();
        window.draw(player.getSprite());
        window.display();
	}
}

int makeAction(Tank& tank) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        tank.setTexture(34, 2, 29, 29);
        tank.move(-0.1, 0);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        tank.setTexture(99, 1, 29, 29);
        tank.move(0,-0.1);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        tank.setTexture(2, 2, 29, 29);
        tank.move(0.1,0);
        return 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        tank.setTexture(66, 1, 29, 29);
        tank.move(0,0.1);
        return 0;
    }
    return nonAction;
}