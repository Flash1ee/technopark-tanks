#include <SFML/Graphics.hpp>
#include <iostream>

#include "game.h"
#include "cam.h"
#include "game_map.hpp"
#include "TmxLevel.h"


int main() {
    TmxLevel level;
    level.LoadFromFile("/home/dmitry/prep/tanks_2d_udp/project/src/map1.tmx"); //deprecated #TODO
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Tanks"); // TODO

    TmxObject Player_obj = level.GetFirstObject("player"); //TODO: make const name

    Player player(level, objectImage, sf::IntRect(1, 2, 13, 13), Player_obj.rect.left + Player_obj.rect.width / 2, Player_obj.rect.top - Player_obj.rect.width / 2, 0.07, 100);

    std::vector<Bullet*> vectorBullet;
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
        for (auto i: vectorBullet) {
            window.draw(i->getSprite());
        }
        window.draw(player.getSprite());
        window.display();
    }
}
