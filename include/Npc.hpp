#pragma once

#include "AStar.hpp"
#include "Entity.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <vector>

class Npc final : public Entity {
public:
    enum class Role { Civilian, Soldier };

    Npc(sf::Vector2f position, Role role = Role::Civilian, const sf::Texture* texture = nullptr);

    void update(float deltaTime) override;
    void draw(sf::RenderTarget& target) const override;
    void update(float deltaTime, sf::Vector2f threat, const AStar& pathfinder);
    bool canShoot(sf::Vector2f target, float deltaTime);
    Role role() const;

private:
    Role role_;
    const sf::Texture* texture_ = nullptr;
    float speed_ = 115.0F;
    float visionRange_ = 280.0F;
    float shotCooldown_ = 0.0F;
    float wanderTimer_ = 0.0F;
    unsigned int wanderDirectionIndex_ = 0;
    sf::Vector2f wanderDirection_{1.0F, 0.0F};
    float animationTimer_ = 0.0F;
    unsigned int animationPhase_ = 0;
    unsigned int animationRow_ = 0;
    std::vector<sf::Vector2f> path_;
    std::size_t nextWaypoint_ = 0;
    float pathRefreshTimer_ = 0.0F;
};
