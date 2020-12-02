#include "game.h"
#include "cam.h"
#include "game_map.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 760), "Tanks");
    Player player(playerTankImage, sf::IntRect(1, 2, 13, 13), 200, 200, 0.1, 100);
    std::vector<Bullet*> vectorBullet;
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
                vectorBullet.push_back(new Bullet(bulletImage, sf::IntRect(0, 0, 15, 15), player.getX(), player.getY(), 0.5, player.getDir()));
            }
        }

        player.makeAction(time);
        for (auto i: vectorBullet) {
            i->move(time);
        }
        cam.changeViewCoords(player.getX(), player.getY());
        cam.changeView();

        window.setView(cam.view);//"оживляем" камеру в окне sfml
        window.clear();

        map.drawMap(window);
        for (auto i: vectorBullet) {
            window.draw(i->getSprite());
        }
        window.draw(player.getSprite());
        window.display();
    }
}
