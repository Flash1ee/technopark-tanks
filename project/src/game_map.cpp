#include "game_map.hpp"

void Map::set_pos(size_t i, size_t j, entities type) {
  if (i >= m_rows || j >= m_cols || type >= COUNT) {
    throw std::invalid_argument("Bad position");
  }
  m_field[i][j] = type;
  return;
}
Map::Map(std::istream& in) {
  if (!in) {
    throw std::invalid_argument("Bad stream");
  }
  in >> m_rows >> m_cols;
  if (!m_rows || !m_cols) {
    throw "Empty play field";
  }
  m_field.resize(m_rows);
  for (auto& row : m_field) {
    row.resize(m_cols);
  }
  std::string line;
  for (size_t i = 0; i < m_rows; i++) {
    for (size_t j = 0; j < m_cols; j++) {
      in >> line;
      if (!map_entity.count(line)) {
        throw std::invalid_argument("Unknown map entity");
      }
      set_pos(i, j, map_entity[line]);
    }
  }
  if (!in.eof()) {
    throw "read error, not found eof";
  }
}