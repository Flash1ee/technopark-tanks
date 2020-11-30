#include "cam.h"

void Cam::changeViewCoords(float x, float y) { //функция для считывания координат игрока
	float tempX = x; 
	float tempY = y;
 
	if (x < 110) tempX = 110;//убираем из вида левую сторону
	if (x > 350) tempX = 350;//убираем из вида правую сторону
	if (y < 150) tempY = 150;//верхнюю сторону
	if (y > 310) tempY = 310;//нижнюю сторону	
 
	view.setCenter(tempX, tempY); //следим за игроком, передавая его координаты. 
}

Cam::Cam() {
    view.reset(sf::FloatRect(200, 200, 400, 400));
	// view.setSize(200, 200);
}
 
// void Cam::moveMap(float time, float speed) { //функция для перемещения камеры по карте. принимает время sfml
// 	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
// 		view.move(time*speed, 0);//скроллим карту вправо (см урок, когда мы двигали героя - всё тоже самое)
// 	}
// 	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
// 		view.move(0,time*speed);//скроллим карту вниз (см урок, когда мы двигали героя - всё тоже самое)
// 	}
// 	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
// 		view.move(time*speed, 0);//скроллим карту влево (см урок, когда мы двигали героя - всё тоже самое)
// 	}
// 	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
// 		view.move(0, time*speed);//скроллим карту вправо (см урок, когда мы двигали героя - всё тоже самое)
// 	}
// }
 
void Cam::changeView(){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
		view.zoom(0.999); //масштабируем, увеличение
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
		view.setSize(640, 480);//устанавливает размер камеры (наш исходный)
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		view.setSize(540, 380);//например другой размер
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
		view.setSize(400, 200);//например другой размер
	}
}