#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

class Food {
public:
    enum class Type { Brain, Leg, Can };

    Food(sf::Vector2f position, Type type = Type::Can, const sf::Texture* texture = nullptr);

    void draw(sf::RenderTarget& target) const;
    sf::Vector2f position() const;
    void update(float deltaTime);
    float value() const;
    bool consumed() const;
    void consume();

private:
    sf::Vector2f position_;
    Type type_;
    const sf::Texture* texture_ = nullptr;
    bool consumed_ = false;
    float respawnTimer_ = 0.0F;
};
