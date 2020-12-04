#include "game.h"
#include "cam.h"
#include "game_map.hpp"
#include "TmxLevel.h"

#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    TmxLevel level;
    level.LoadFromFile("/home/dmitry/prep/tanks_2d_udp/project/src/map1.tmx");
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Tanks");
    TmxObject obj = level.GetFirstObject("player");
    std::cout << obj.name << std::endl;

    Player player(objectImage, sf::IntRect(1, 2, 13, 13), 200, 200, 0.1, 100);
    std::vector<Bullet*> vectorBullet;
    Map map(mapOne, objectImage);
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
                vectorBullet.push_back(new Bullet(objectImage, sf::IntRect(321, 100, 8, 8), player.getX(), player.getY(), 0.5, player.getDir()));
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

        level.Draw(window);
        // map.drawMap(window);
        for (auto i: vectorBullet) {
            window.draw(i->getSprite());
        }
        window.draw(player.getSprite());
        window.display();
    }
}
