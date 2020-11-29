#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.h"
#include "objects.h"
#include "game_map.hpp"



int main() {
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Tanks");
    Player player(playerTankImage, 100, 100, 1, 2, 13, 13, 100);
    // Map map("../maps/1.txt", "../textures/sprites.png");
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
        // map.drawMap(window);
        window.draw(player.getSprite());
        window.display();
	}
}
>>>>>>> f9713d83fdf1ea75f5ed9fec6cbcc817af22117c
