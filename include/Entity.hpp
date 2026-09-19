#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

class Entity {
public:
    explicit Entity(sf::Vector2f position);
    virtual ~Entity() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderTarget& target) const = 0;

    sf::Vector2f position() const;
    void setPosition(sf::Vector2f position);

protected:
    sf::Vector2f position_;
};
