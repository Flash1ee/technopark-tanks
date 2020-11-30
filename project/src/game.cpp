#include "game.h"
#include "cam.h"
#include "game_map.hpp"

#include <SFML/Graphics.hpp>
//#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Tanks");
    Player player(playerTankImage, 200, 200, 1, 2, 13, 13, 100, 0.1);
    Bullet bullet(bulletImage, 0, 0, 0, 0, 15, 15, 0.5, 0);
    Map map(map_one, playerTankImage);
    sf::Clock clock;
    Cam cam;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();  //дать прошедшее время в микросекундах
        clock.restart();    //перезагружает время
        time = time / 800;  //скорость игры

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (player.getShot()) {
                player.setShot(false);
                bullet.setX(player.getX());
                bullet.setY(player.getY());
                bullet.setDir(player.getDir());
            }
        }

        player.makeAction(time);
        bullet.move(time);
        cam.changeViewCoords(player.getX(), player.getY());
        cam.changeView();

        window.setView(cam.view);//"оживляем" камеру в окне sfml
        window.clear();

        map.drawMap(window);
        window.draw(bullet.getSprite());
        window.draw(player.getSprite());
        window.display();
    }
}
