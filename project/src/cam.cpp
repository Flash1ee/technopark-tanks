#include "cam.h"

void Cam::changeViewCoords(
    const sf::Vector2f& coords, double map_width, double map_height) {  //функция для считывания координат игрока
    float tempX = coords.x;
    float tempY = coords.y;

    double viewX = view.getSize().x / 2;
    double viewY = view.getSize().y / 2;

    if (coords.x < viewX) tempX = viewX;  //убираем из вида левую сторону
    if (coords.x > map_width - viewX) tempX = map_width - viewX;  //убираем из вида правую сторону
    if (coords.y < viewY) tempY = viewY;    //верхнюю сторону
    if (coords.y > map_height - viewY) tempY = map_height - viewY; 

    view.setCenter(tempX,
                   tempY);  //следим за игроком, передавая его координаты.
}

Cam::Cam() {
    view.reset(sf::FloatRect(0, 0, 1920, 1080));
    view.setSize(1920, 1080);
    view.zoom(0.15); //change ZOOM example 0.15 - 15% from curr view
}

void Cam::changeView() {  // compare function
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
        view.zoom(0.96);  //масштабируем, увеличение
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        view.zoom(0.97);
        //устанавливает размер камеры (наш исходный)
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
        view.zoom(0.98);
        //например другой размер
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
        view.zoom(0.99);
        //например другой размер
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
        view.setSize(760, 380);
        view.zoom(1);
        //например другой размер
    }
}