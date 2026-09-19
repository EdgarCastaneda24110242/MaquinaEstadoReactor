#include "Zombie.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>
#include <cmath>

namespace {
float length(sf::Vector2f vector) { return std::sqrt(vector.x * vector.x + vector.y * vector.y); }
}

Zombie::Zombie(sf::Vector2f position, const sf::Texture* spriteSheet)
    : Entity(position), spriteSheet_(spriteSheet) {}

namespace {
constexpr sf::Vector2f ZombieHalfSize{11.0F, 15.0F};
constexpr float FrameDuration = 0.12F;

void setAnimationDirection(sf::Vector2f direction, unsigned int& row) {
    if (std::abs(direction.x) > std::abs(direction.y)) row = direction.x < 0.0F ? 2 : 3;
    else if (std::abs(direction.y) > 0.01F) row = direction.y < 0.0F ? 0 : 1;
}
}

void Zombie::setDestination(sf::Vector2f destination, const AStar& pathfinder) {
    const auto cells = pathfinder.findPath(pathfinder.worldToCell(position_), pathfinder.worldToCell(destination),
                                           ZombieHalfSize);
    path_.clear();
    for (const auto cell : cells) path_.push_back(pathfinder.cellToWorld(cell));
    nextWaypoint_ = path_.size() > 1 ? 1 : 0;
}

void Zombie::moveManually(sf::Vector2f direction, float deltaTime, const AStar& pathfinder) {
    const float directionLength = length(direction);
    if (directionLength <= 0.01F) return;
    path_.clear();
    nextWaypoint_ = 0;
    const sf::Vector2f candidate = position_ + direction / directionLength * speed_ * deltaTime;
    if (pathfinder.isWalkable(candidate, ZombieHalfSize)) {
        position_ = candidate;
        movementDirection_ = direction / directionLength;
        setAnimationDirection(movementDirection_, animationRow_);
        advanceAnimation(deltaTime);
    }
}

void Zombie::update(float deltaTime) {
    hunger_ = std::max(0.0F, hunger_ - deltaTime * 1.5F);
    boostDuration_ = std::max(0.0F, boostDuration_ - deltaTime);
    if (nextWaypoint_ >= path_.size()) return;
    const sf::Vector2f direction = path_[nextWaypoint_] - position_;
    const float distance = length(direction);
    if (distance < 3.0F) {
        position_ = path_[nextWaypoint_++];
        return;
    }
    movementDirection_ = direction / distance;
    setAnimationDirection(movementDirection_, animationRow_);
    position_ += movementDirection_ * speed() * deltaTime;
    advanceAnimation(deltaTime);
}

void Zombie::update(float deltaTime, const AStar& pathfinder) {
    hunger_ = std::max(0.0F, hunger_ - deltaTime * 1.5F);
    boostDuration_ = std::max(0.0F, boostDuration_ - deltaTime);
    if (nextWaypoint_ < path_.size()) {
        const sf::Vector2f direction = path_[nextWaypoint_] - position_;
        const float distance = length(direction);
        if (distance < 3.0F) {
            position_ = path_[nextWaypoint_++];
        } else {
            movementDirection_ = direction / distance;
            setAnimationDirection(movementDirection_, animationRow_);
            const sf::Vector2f candidate = position_ + movementDirection_ * speed() * deltaTime;
            if (pathfinder.isWalkable(candidate, ZombieHalfSize)) {
                position_ = candidate;
                advanceAnimation(deltaTime);
            }
            else {
                path_.clear();
                nextWaypoint_ = 0;
            }
        }
    }
}

void Zombie::advanceAnimation(float deltaTime) {
    animationTimer_ += deltaTime;
    if (animationTimer_ >= FrameDuration) {
        animationTimer_ -= FrameDuration;
        animationFrame_ = (animationFrame_ + 1) % 5;
    }
}

void Zombie::draw(sf::RenderTarget& target) const {
    if (spriteSheet_ != nullptr) {
        sf::Sprite sprite(*spriteSheet_);
        sprite.setTextureRect({{256 + static_cast<int>(animationFrame_) * 256,
                    32 + static_cast<int>(animationRow_) * 256},
                   {128, 160}});
        sprite.setOrigin({64.0F, 80.0F});
        sprite.setScale({0.26F, 0.26F});
        sprite.setPosition(position_);
        target.draw(sprite);
        return;
    }
    sf::CircleShape body(15.0F);
    body.setOrigin({15.0F, 15.0F});
    body.setPosition(position_);
    body.setFillColor(sf::Color(177, 53, 54));
    body.setOutlineThickness(3.0F);
    body.setOutlineColor(sf::Color(35, 20, 20));
    target.draw(body);
    sf::CircleShape eye(3.0F);
    eye.setOrigin({3.0F, 3.0F});
    eye.setPosition(position_ + sf::Vector2f{5.0F, -4.0F});
    eye.setFillColor(sf::Color(255, 235, 150));
    target.draw(eye);
}

void Zombie::eat(float amount) { hunger_ = std::min(100.0F, hunger_ + amount); }
void Zombie::chargeBoost(float amount) { boost_ = std::min(100.0F, boost_ + amount); }
void Zombie::fillBoost() { boost_ = 100.0F; }
void Zombie::useBoost() {
    if (boost_ < 100.0F) return;
    boost_ = 0.0F;
    boostDuration_ = 6.0F;
}
void Zombie::hurt(float amount) { health_ = std::max(0.0F, health_ - amount); }
void Zombie::regenerate(float amount) { health_ = std::min(100.0F, health_ + amount); }
float Zombie::hunger() const { return hunger_; }
float Zombie::health() const { return health_; }
float Zombie::boost() const { return boost_; }
float Zombie::speed() const { return speed_ * (boostDuration_ > 0.0F ? 2.5F : 1.0F); }
bool Zombie::moving() const { return nextWaypoint_ < path_.size(); }
