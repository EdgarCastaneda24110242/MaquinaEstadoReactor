#include "Food.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

Food::Food(sf::Vector2f position, Type type, const sf::Texture* texture)
    : position_(position), type_(type), texture_(texture) {}

void Food::draw(sf::RenderTarget& target) const {
    if (consumed_) return;
    if (texture_ != nullptr) {
        const int panelWidth = static_cast<int>(texture_->getSize().x / 3);
        sf::Sprite sprite(*texture_);
        sprite.setTextureRect({{type_ == Type::Brain ? 0 : type_ == Type::Leg ? panelWidth : panelWidth * 2, 400},
                       {panelWidth, 600}});
        sprite.setOrigin({panelWidth / 2.0F, 300.0F});
        sprite.setPosition(position_);
        sprite.setScale({0.05F, 0.05F});
        target.draw(sprite);
        return;
    }
    sf::CircleShape shape(8.0F);
    shape.setOrigin({8.0F, 8.0F});
    shape.setPosition(position_);
    shape.setFillColor(sf::Color(244, 190, 55));
    shape.setOutlineThickness(2.0F);
    shape.setOutlineColor(sf::Color(120, 70, 20));
    target.draw(shape);
}

sf::Vector2f Food::position() const { return position_; }
void Food::update(float deltaTime) {
    if (!consumed_ || type_ != Type::Brain) return;
    respawnTimer_ -= deltaTime;
    if (respawnTimer_ <= 0.0F) consumed_ = false;
}
float Food::value() const {
    if (type_ == Type::Brain) return 80.0F;
    if (type_ == Type::Leg) return 40.0F;
    return 15.0F;
}
bool Food::consumed() const { return consumed_; }
void Food::consume() {
    consumed_ = true;
    if (type_ == Type::Brain) respawnTimer_ = 30.0F;
}
