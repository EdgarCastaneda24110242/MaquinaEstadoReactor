#include "Npc.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>
#include <cmath>

namespace {
float length(sf::Vector2f vector) { return std::sqrt(vector.x * vector.x + vector.y * vector.y); }
}

Npc::Npc(sf::Vector2f position, Role role, const sf::Texture* texture)
    : Entity(position), role_(role), texture_(texture) {
    speed_ = role_ == Role::Soldier ? 70.0F : 120.0F;
    visionRange_ = role_ == Role::Soldier ? 520.0F : 300.0F;
}

void Npc::update(float deltaTime) { (void)deltaTime; }

void Npc::update(float deltaTime, sf::Vector2f threat, const AStar& pathfinder) {
    const sf::Vector2f away = position_ - threat;
    const float distance = length(away);
    const bool hasThreat = distance <= visionRange_ && distance > 0.01F;
    sf::Vector2f goal = position_;
    if (role_ == Role::Soldier && hasThreat) {
        if (distance < 150.0F) {
            goal = position_ + away / distance * 220.0F;
        } else {
            goal = threat;
        }
    } else if (role_ == Role::Civilian && hasThreat) {
        goal = position_ + away / distance * 260.0F;
    } else {
        wanderTimer_ -= deltaTime;
        if (wanderTimer_ <= 0.0F) {
            wanderTimer_ = 2.4F;
            const sf::Vector2f directions[] = {{1.0F, 0.0F}, {0.0F, 1.0F}, {-1.0F, 0.0F}, {0.0F, -1.0F}};
            wanderDirectionIndex_ = (wanderDirectionIndex_ + 1) % 4;
            wanderDirection_ = directions[wanderDirectionIndex_];
            path_.clear();
        }
        goal = position_ + wanderDirection_ * 180.0F;
    }

    pathRefreshTimer_ -= deltaTime;
    if (path_.empty() || pathRefreshTimer_ <= 0.0F) {
        const auto cells = pathfinder.findPath(pathfinder.worldToCell(position_), pathfinder.worldToCell(goal), {10.0F, 10.0F});
        path_.clear();
        for (const auto cell : cells) path_.push_back(pathfinder.cellToWorld(cell));
        nextWaypoint_ = path_.size() > 1 ? 1 : 0;
        pathRefreshTimer_ = role_ == Role::Soldier ? 1.0F : (hasThreat ? 0.35F : 1.0F);
    }
    if (nextWaypoint_ < path_.size()) {
        const sf::Vector2f delta = path_[nextWaypoint_] - position_;
        const float waypointDistance = length(delta);
        bool moved = false;
        if (waypointDistance < 3.0F) {
            position_ = path_[nextWaypoint_++];
            moved = true;
        } else {
            const sf::Vector2f direction = delta / waypointDistance;
            const sf::Vector2f candidate = position_ + direction * speed_ * deltaTime;
            if (pathfinder.isWalkable(candidate, {10.0F, 10.0F})) {
                position_ = candidate;
                moved = true;
            } else {
                path_.clear();
            }
            wanderDirection_ = direction;
        }
        if (std::abs(wanderDirection_.x) > std::abs(wanderDirection_.y)) {
            animationRow_ = wanderDirection_.x < 0.0F ? 2 : 3;
        } else {
            animationRow_ = wanderDirection_.y < 0.0F ? 0 : 1;
        }
        if (moved) {
            animationTimer_ += deltaTime;
            while (animationTimer_ >= 0.06F) {
                animationTimer_ -= 0.06F;
                animationPhase_ = (animationPhase_ + 1) % 8;
            }
        }
    }
}

bool Npc::canShoot(sf::Vector2f target, float deltaTime) {
    if (role_ != Role::Soldier) return false;
    shotCooldown_ = std::max(0.0F, shotCooldown_ - deltaTime);
    const sf::Vector2f delta = target - position_;
    if (shotCooldown_ > 0.0F || length(delta) > visionRange_) return false;
    shotCooldown_ = 2.8F;
    return true;
}

void Npc::draw(sf::RenderTarget& target) const {
    if (role_ == Role::Soldier && texture_ != nullptr) {
        const int frameWidth = static_cast<int>(texture_->getSize().x / 5);
        const int frameHeight = static_cast<int>(texture_->getSize().y / 4);
        const unsigned int frameSequence[] = {0, 1, 2, 3, 4, 3, 2, 1};
        sf::Sprite sprite(*texture_);
        sprite.setTextureRect({{static_cast<int>(frameSequence[animationPhase_]) * frameWidth,
                    static_cast<int>(animationRow_) * frameHeight}, {frameWidth, frameHeight}});
        sprite.setOrigin({frameWidth / 2.0F, frameHeight * 0.75F});
        sprite.setScale({0.2F, 0.2F});
        sprite.setPosition(position_);
        target.draw(sprite);
        return;
    }
    sf::CircleShape body(11.0F);
    body.setOrigin({11.0F, 11.0F});
    body.setPosition(position_);
    body.setFillColor(sf::Color(75, 220, 105));
    body.setOutlineThickness(2.0F);
    body.setOutlineColor(sf::Color(18, 48, 58));
    target.draw(body);
}

Npc::Role Npc::role() const { return role_; }
