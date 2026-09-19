#include "AStar.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>

namespace {
struct Node {
    sf::Vector2i cell;
    float cost;
    bool operator>(const Node& other) const { return cost > other.cost; }
};

float distance(sf::Vector2i a, sf::Vector2i b) {
    const float x = static_cast<float>(a.x - b.x);
    const float y = static_cast<float>(a.y - b.y);
    return std::sqrt(x * x + y * y);
}
}

AStar::AStar(unsigned int width, unsigned int height, unsigned int cellSize)
    : width_(width), height_(height), cellSize_(cellSize), blocked_(width * height, false) {}

void AStar::setBlocked(unsigned int x, unsigned int y, bool blocked) {
    if (x < width_ && y < height_) blocked_[y * width_ + x] = blocked;
}

void AStar::setBlockedRectangle(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom) {
    for (unsigned int y = top; y <= bottom && y < height_; ++y) {
        for (unsigned int x = left; x <= right && x < width_; ++x) setBlocked(x, y, true);
    }
}

bool AStar::inside(sf::Vector2i cell) const {
    return cell.x >= 0 && cell.y >= 0 && static_cast<unsigned int>(cell.x) < width_ &&
           static_cast<unsigned int>(cell.y) < height_;
}

std::size_t AStar::index(sf::Vector2i cell) const {
    return static_cast<std::size_t>(cell.y) * width_ + static_cast<std::size_t>(cell.x);
}

std::vector<sf::Vector2i> AStar::findPath(sf::Vector2i start, sf::Vector2i goal, sf::Vector2f halfSize) const {
    if (!inside(start) || !inside(goal)) return {};
    const auto walkableCell = [this, halfSize](sf::Vector2i cell) {
        return inside(cell) && isWalkable(cellToWorld(cell), halfSize);
    };
    const auto nearestWalkableCell = [this, &walkableCell](sf::Vector2i cell) {
        if (walkableCell(cell)) return cell;
        for (int radius = 1; radius < static_cast<int>(std::max(width_, height_)); ++radius) {
            for (int y = cell.y - radius; y <= cell.y + radius; ++y) {
                for (int x = cell.x - radius; x <= cell.x + radius; ++x) {
                    const sf::Vector2i candidate{x, y};
                    if (walkableCell(candidate)) return candidate;
                }
            }
        }
        return sf::Vector2i{-1, -1};
    };
    start = nearestWalkableCell(start);
    goal = nearestWalkableCell(goal);
    if (!inside(start) || !inside(goal)) return {};

    const sf::Vector2i directions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    const float infinity = std::numeric_limits<float>::infinity();
    std::vector<float> cost(width_ * height_, infinity);
    std::vector<sf::Vector2i> previous(width_ * height_, {-1, -1});
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;
    cost[index(start)] = 0.0F;
    open.push({start, distance(start, goal)});

    while (!open.empty()) {
        const sf::Vector2i current = open.top().cell;
        open.pop();
        if (current == goal) break;

        for (const sf::Vector2i direction : directions) {
            const sf::Vector2i next = current + direction;
            if (!walkableCell(next)) continue;
            const float nextCost = cost[index(current)] + 1.0F;
            if (nextCost >= cost[index(next)]) continue;
            cost[index(next)] = nextCost;
            previous[index(next)] = current;
            open.push({next, nextCost + distance(next, goal)});
        }
    }

    if (previous[index(goal)] == sf::Vector2i{-1, -1} && start != goal) return {};
    std::vector<sf::Vector2i> path;
    for (sf::Vector2i current = goal; current != start; current = previous[index(current)]) path.push_back(current);
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

sf::Vector2i AStar::nearestWalkable(sf::Vector2i cell) const {
    if (inside(cell) && !blocked_[index(cell)]) return cell;
    for (int radius = 1; radius < static_cast<int>(std::max(width_, height_)); ++radius) {
        for (int y = cell.y - radius; y <= cell.y + radius; ++y) {
            for (int x = cell.x - radius; x <= cell.x + radius; ++x) {
                const sf::Vector2i candidate{x, y};
                if (inside(candidate) && !blocked_[index(candidate)]) return candidate;
            }
        }
    }
    return {-1, -1};
}

sf::Vector2i AStar::worldToCell(sf::Vector2f position) const {
    return {static_cast<int>(position.x / static_cast<float>(cellSize_)),
            static_cast<int>(position.y / static_cast<float>(cellSize_))};
}

sf::Vector2f AStar::cellToWorld(sf::Vector2i cell) const {
    const float half = static_cast<float>(cellSize_) / 2.0F;
    return {static_cast<float>(cell.x) * cellSize_ + half, static_cast<float>(cell.y) * cellSize_ + half};
}

bool AStar::isWalkable(sf::Vector2f position) const {
    const sf::Vector2i cell = worldToCell(position);
    return inside(cell) && !blocked_[index(cell)];
}

bool AStar::isWalkable(sf::Vector2f position, sf::Vector2f halfSize) const {
    const sf::Vector2i minimum = worldToCell(position - halfSize);
    const sf::Vector2i maximum = worldToCell(position + halfSize);
    for (int y = minimum.y; y <= maximum.y; ++y) {
        for (int x = minimum.x; x <= maximum.x; ++x) {
            if (!inside({x, y}) || blocked_[index({x, y})]) return false;
        }
    }
    return true;
}

sf::Vector2f AStar::nearestWalkablePosition(sf::Vector2f position, sf::Vector2f halfSize) const {
    const sf::Vector2i origin = worldToCell(position);
    for (int radius = 0; radius < static_cast<int>(std::max(width_, height_)); ++radius) {
        for (int y = origin.y - radius; y <= origin.y + radius; ++y) {
            for (int x = origin.x - radius; x <= origin.x + radius; ++x) {
                const sf::Vector2i cell{x, y};
                if (inside(cell) && isWalkable(cellToWorld(cell), halfSize)) return cellToWorld(cell);
            }
        }
    }
    return position;
}

bool AStar::isBlocked(sf::Vector2i cell) const { return !inside(cell) || blocked_[index(cell)]; }
unsigned int AStar::width() const { return width_; }
unsigned int AStar::height() const { return height_; }
unsigned int AStar::cellSize() const { return cellSize_; }
