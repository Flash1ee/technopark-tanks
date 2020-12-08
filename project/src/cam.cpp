#include "cam.h"

void Cam::changeViewCoords(
    const sf::Vector2f& coords) {  //функция для считывания координат игрока
    float tempX = coords.x;
    float tempY = coords.y;

    if (coords.x < 190) tempX = 190;  //убираем из вида левую сторону
    if (coords.x > 192) tempX = 192;  //убираем из вида правую сторону
    if (coords.y < 92) tempY = 92;    //верхнюю сторону
    if (coords.y > 292) tempY = 292;  //нижнюю сторону

    view.setCenter(tempX,
                   tempY);  //следим за игроком, передавая его координаты.
}

Cam::Cam() {
    view.reset(sf::FloatRect(0, 0, 1024, 760));
    view.setSize(1024, 512);
    view.zoom(0.39);
}

// void Cam::moveMap(float time, float speed) { //функция для перемещения камеры
// по карте. принимает время sfml 	if
// (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { 		view.move(time*speed,
// 0);//скроллим карту вправо (см урок, когда мы двигали героя - всё тоже самое)
// 	}
// 	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
// 		view.move(0,time*speed);//скроллим карту вниз (см урок, когда мы
// двигали героя - всё тоже самое)
// 	}
// 	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
// 		view.move(time*speed, 0);//скроллим карту влево (см урок, когда мы
// двигали героя - всё тоже самое)
// 	}
// 	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
// 		view.move(0, time*speed);//скроллим карту вправо (см урок, когда мы
// двигали героя - всё тоже самое)
// 	}
// }

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