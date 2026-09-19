#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

class AStar {
public:
    AStar(unsigned int width, unsigned int height, unsigned int cellSize);

    void setBlocked(unsigned int x, unsigned int y, bool blocked);
    void setBlockedRectangle(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom);
    std::vector<sf::Vector2i> findPath(sf::Vector2i start, sf::Vector2i goal,
                                       sf::Vector2f halfSize = {}) const;
    sf::Vector2i nearestWalkable(sf::Vector2i cell) const;
    sf::Vector2i worldToCell(sf::Vector2f position) const;
    sf::Vector2f cellToWorld(sf::Vector2i cell) const;
    bool isWalkable(sf::Vector2f position) const;
    bool isWalkable(sf::Vector2f position, sf::Vector2f halfSize) const;
    sf::Vector2f nearestWalkablePosition(sf::Vector2f position, sf::Vector2f halfSize) const;
    bool isBlocked(sf::Vector2i cell) const;
    unsigned int width() const;
    unsigned int height() const;
    unsigned int cellSize() const;

private:
    unsigned int width_;
    unsigned int height_;
    unsigned int cellSize_;
    std::vector<bool> blocked_;

    bool inside(sf::Vector2i cell) const;
    std::size_t index(sf::Vector2i cell) const;
};
