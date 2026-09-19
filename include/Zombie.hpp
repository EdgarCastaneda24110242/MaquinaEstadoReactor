#pragma once

#include "AStar.hpp"
#include "Entity.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

class Zombie final : public Entity {
public:
    Zombie(sf::Vector2f position, const sf::Texture* spriteSheet = nullptr);

    void update(float deltaTime) override;
    void update(float deltaTime, const AStar& pathfinder);
    void draw(sf::RenderTarget& target) const override;
    void setDestination(sf::Vector2f destination, const AStar& pathfinder);
    void moveManually(sf::Vector2f direction, float deltaTime, const AStar& pathfinder);
    void eat(float amount);
    void chargeBoost(float amount);
    void fillBoost();
    void useBoost();
    void hurt(float amount);
    void regenerate(float amount);
    float hunger() const;
    float health() const;
    float boost() const;
    float speed() const;
    bool moving() const;

private:
    std::vector<sf::Vector2f> path_;
    std::size_t nextWaypoint_ = 0;
    float hunger_ = 100.0F;
    float health_ = 100.0F;
    float boost_ = 0.0F;
    float boostDuration_ = 0.0F;
    float speed_ = 90.0F;
    const sf::Texture* spriteSheet_ = nullptr;
    sf::Vector2f movementDirection_;
    float animationTimer_ = 0.0F;
    unsigned int animationFrame_ = 0;
    unsigned int animationRow_ = 1;

    void advanceAnimation(float deltaTime);
};
