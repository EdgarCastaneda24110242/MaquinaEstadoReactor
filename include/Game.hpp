#pragma once

#include "AStar.hpp"
#include "Food.hpp"
#include "Npc.hpp"
#include "Zombie.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

class Game {
public:
    Game();
    int run();

private:
    enum class Screen { Title, Playing };

    sf::RenderWindow window_;
    sf::Texture mapTexture_;
    sf::Sprite mapSprite_;
    sf::Texture zombieTexture_;
    sf::Texture soldierTexture_;
    sf::Texture foodTexture_;
    sf::Texture projectileTexture_;
    sf::Font font_;
    Screen screen_ = Screen::Title;
    bool showCollisionMap_ = true;
    AStar pathfinder_;
    Zombie zombie_;
    std::vector<Npc> npcs_;
    std::vector<Food> foods_;
    struct Projectile {
        sf::Vector2f position;
        sf::Vector2f velocity;
        float rotation = 0.0F;
        float animationTimer = 0.0F;
        unsigned int animationFrame = 0;
    };
    std::vector<Projectile> projectiles_;
    int score_ = 0;
    int nextSoldierScore_ = 1000;
    sf::View camera_;
    std::optional<sf::Vector2f> destination_;

    void processEvents();
    void update(float deltaTime);
    void draw();
    void drawTitle();
    void drawHud();
    void drawCollisionMap();
    void keepCameraInMap();
    void configureCollisionMap();
};
