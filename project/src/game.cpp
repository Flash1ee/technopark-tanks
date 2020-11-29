#include <iostream>
#include <SFML/Graphics.hpp>
#include "game.h"
#include "game_map.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Tanks");
    Player player(playerTankImage, 100, 100, 1, 2, 13, 13, 100);
    Map map(map_one, playerTankImage);
    sf::Clock clock;

	while (window.isOpen()) {
        double time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
        clock.restart(); //перезагружает время
        time = time/800; //скорость игры
        
        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
                window.close();
            }
		}

        player.makeAction(time);

        window.clear();
        map.drawMap(window);
        // window.draw(player.getSprite());
        window.display();
	}
}
