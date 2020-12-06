#include "game_map.hpp"

#include <fstream>
#include <iostream>

constexpr int block_size = 16;
std::map<char, entities> map_entity = {{'.', EMPTY}, {'#', WALL}};

std::map<entities, std::vector<float>> coords = {
    {WALL,
     {256, 0, static_cast<float>(block_size), static_cast<float>(block_size)}},
    {EMPTY,
     {336, 0, static_cast<float>(block_size), static_cast<float>(block_size)}}};

void Map::setPos(size_t i, size_t j, std::shared_ptr<Object> p) {
    if (i >= m_rows || j >= m_cols) {
        throw std::invalid_argument("Bad position");
    }
    mField[i][j] = p;
    return;
}
sf::Sprite Map::getSprite() const { return this->sprite; }

Map::Map(std::string map, std::string sprite) : m_blockSize(block_size) {
    if (map.empty() || sprite.empty()) {
        throw std::invalid_argument("Bad stream");
    }
    std::ifstream in(map);
    if (!in.is_open()) {
        throw std::runtime_error("Can't open map file");
    }

    in >> m_rows >> m_cols;
    if (!m_rows || !m_cols) {
        std::cout << "Empty play field" << std::endl;
        exit(-1);
    }

    mField.resize(m_rows);
    for (auto& row : mField) {
        row.resize(m_cols);
    }

    std::string line;
    for (size_t i = 0; i < m_rows; i++) {
        in >> line;
        for (size_t j = 0; j < m_cols; j++) {
            char cur = line[j];
            if (!map_entity.count(cur)) {
                throw std::invalid_argument("Unknown map entity");
            }
            auto flag = map_entity[cur];
            std::vector<float> cord = coords[flag];

            auto p = std::make_shared<Object>(sprite, sf::IntRect(cord[0], cord[1],
                                              cord[2], cord[3]), sf::Vector2f(i, j), 0, Direction::UP);
            setPos(i, j, p);
        }
    }

    if (!in.eof()) {
        std::cout << "read error, not found eof" << std::endl;
        exit(-1);
    }
    in.close();
}
void Map::drawMap(sf::RenderWindow& window) {
    for (auto row = 0; row < m_rows; row++) {
        for (auto col = 0; col < m_cols; col++) {
            sf::Sprite temp = mField[row][col]->getSprite();
            temp.setPosition(row * m_blockSize, col * m_blockSize);
            window.draw(temp);
        }
    }
}