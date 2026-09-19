#include "Game.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <random>

namespace {
constexpr sf::Vector2u WindowSize{1280, 720};
constexpr unsigned int CellSize = 16;
constexpr unsigned int MapCellsWide = 96;
constexpr unsigned int MapCellsHigh = 52;
constexpr float ViewWidth = 800.0F;
constexpr float ViewHeight = 450.0F;

float distance(sf::Vector2f a, sf::Vector2f b) {
    const sf::Vector2f delta = a - b;
    return std::sqrt(delta.x * delta.x + delta.y * delta.y);
}
}

Game::Game()
    : window_(sf::VideoMode(WindowSize), "Zombie A*"),
      mapSprite_(mapTexture_),
            pathfinder_(MapCellsWide, MapCellsHigh, CellSize),
        zombie_({704.0F, 400.0F}, &zombieTexture_),
            camera_({704.0F, 400.0F}, {ViewWidth, ViewHeight}) {
    window_.setFramerateLimit(60);
        if (!mapTexture_.loadFromFile("docs/images/Mapa.png")) {
            std::cerr << "No se pudo cargar docs/images/Mapa.png\n";
        } else {
            std::cerr << "Mapa cargado: " << mapTexture_.getSize().x << "x" << mapTexture_.getSize().y << "\n";
        }
        if (!zombieTexture_.loadFromFile("docs/images/Zommbie.png")) (void)zombieTexture_.resize({1, 1});
        if (!soldierTexture_.loadFromFile("docs/images/Soldado.png")) (void)soldierTexture_.resize({1, 1});
        if (!foodTexture_.loadFromFile("docs/images/Comidas.png")) (void)foodTexture_.resize({1, 1});
        (void)projectileTexture_.loadFromFile("docs/images/Disparo.png");
    mapSprite_.setTexture(mapTexture_, true);
    mapSprite_.setScale({0.5F, 0.5F});

    configureCollisionMap();
    const sf::Vector2f zombieHalfSize{11.0F, 15.0F};
    const sf::Vector2f initialSoldierSpawn = pathfinder_.nearestWalkablePosition({940.0F, 520.0F}, {10.0F, 10.0F});
    npcs_.emplace_back(pathfinder_.nearestWalkablePosition({560.0F, 350.0F}, {10.0F, 10.0F}), Npc::Role::Civilian);
    npcs_.emplace_back(pathfinder_.nearestWalkablePosition({850.0F, 360.0F}, {10.0F, 10.0F}), Npc::Role::Civilian);
    npcs_.emplace_back(pathfinder_.nearestWalkablePosition({350.0F, 550.0F}, {10.0F, 10.0F}), Npc::Role::Civilian);
    npcs_.emplace_back(pathfinder_.nearestWalkablePosition({1100.0F, 300.0F}, {10.0F, 10.0F}), Npc::Role::Civilian);
    npcs_.emplace_back(initialSoldierSpawn, Npc::Role::Soldier, &soldierTexture_);
    const auto addFood = [this, zombieHalfSize](sf::Vector2f position, Food::Type type) {
        foods_.emplace_back(pathfinder_.nearestWalkablePosition(position, zombieHalfSize), type, &foodTexture_);
    };
    addFood({400.0F, 150.0F}, Food::Type::Brain);
    addFood({1100.0F, 650.0F}, Food::Type::Brain);
    addFood({300.0F, 200.0F}, Food::Type::Can);
    addFood({450.0F, 300.0F}, Food::Type::Can);
    addFood({600.0F, 200.0F}, Food::Type::Can);
    addFood({750.0F, 300.0F}, Food::Type::Can);
    addFood({900.0F, 200.0F}, Food::Type::Can);
    addFood({1050.0F, 300.0F}, Food::Type::Can);
    addFood({1200.0F, 400.0F}, Food::Type::Can);
    addFood({500.0F, 600.0F}, Food::Type::Can);
    addFood({350.0F, 500.0F}, Food::Type::Leg);
    addFood({550.0F, 500.0F}, Food::Type::Leg);
    addFood({750.0F, 500.0F}, Food::Type::Leg);
    addFood({950.0F, 500.0F}, Food::Type::Leg);
    addFood({1150.0F, 500.0F}, Food::Type::Leg);
    (void)font_.openFromFile("C:/Windows/Fonts/consola.ttf");
}

void Game::configureCollisionMap() {
    for (unsigned int y = 0; y < MapCellsHigh; ++y) {
        pathfinder_.setBlocked(0, y, true);
        pathfinder_.setBlocked(MapCellsWide - 1, y, true);
    }
    for (unsigned int x = 0; x < MapCellsWide; ++x) {
        pathfinder_.setBlocked(x, 0, true);
        pathfinder_.setBlocked(x, MapCellsHigh - 1, true);
    }

    const auto block = [this](unsigned int left, unsigned int top, unsigned int width,
                              unsigned int height) {
        pathfinder_.setBlockedRectangle(left, top, left + width - 1, top + height - 1);
    };
    const auto wall = [this](unsigned int left, unsigned int top, unsigned int length,
                             bool horizontal) {
        if (horizontal) pathfinder_.setBlockedRectangle(left, top, left + length - 1, top);
        else pathfinder_.setBlockedRectangle(left, top, left, top + length - 1);
    };

    // Small obstacles leave three-cell-wide passages for the zombie.
    block(6, 5, 3, 3);
    block(15, 5, 3, 2);
    block(25, 5, 3, 3);
    block(36, 5, 3, 2);
    block(48, 5, 3, 3);
    block(61, 5, 3, 2);
    block(73, 5, 3, 3);
    block(10, 12, 3, 3);
    block(21, 11, 2, 3);
    block(31, 13, 3, 2);
    block(42, 11, 3, 3);
    block(55, 13, 3, 2);
    block(67, 11, 2, 3);
    block(78, 13, 3, 3);
    block(5, 22, 3, 2);
    block(16, 20, 3, 3);
    block(28, 23, 2, 3);
    block(39, 20, 3, 2);
    block(51, 22, 3, 3);
    block(64, 20, 3, 2);
    block(75, 22, 3, 3);
    block(11, 31, 3, 3);
    block(23, 29, 3, 2);
    block(35, 32, 2, 3);
    block(47, 29, 3, 3);
    block(59, 31, 3, 2);
    block(71, 29, 3, 3);
    block(7, 39, 3, 2);
    block(19, 37, 3, 3);
    block(31, 40, 3, 2);
    block(43, 37, 2, 3);
    block(55, 40, 3, 2);
    block(67, 37, 3, 3);
    block(79, 40, 3, 2);

    // Thin alternating walls turn the open field into connected maze corridors.
    wall(3, 9, 15, true);
    wall(22, 9, 17, true);
    wall(42, 9, 14, true);
    wall(59, 9, 12, true);
    wall(76, 9, 9, true);
    wall(12, 18, 18, true);
    wall(35, 18, 14, true);
    wall(54, 18, 17, true);
    wall(76, 18, 9, true);
    wall(10, 36, 15, true);
    wall(30, 36, 14, true);
    wall(50, 36, 17, true);
    wall(72, 36, 13, true);
    wall(17, 10, 8, false);
    wall(37, 10, 9, false);
    wall(59, 10, 8, false);
    wall(72, 19, 9, false);
    wall(30, 19, 8, false);
    wall(49, 19, 7, false);
    wall(24, 37, 7, false);
    wall(67, 37, 8, false);

    // Continue the maze into the enlarged eastern area.
    block(85, 5, 3, 3);
    block(91, 12, 3, 3);
    block(84, 23, 3, 2);
    block(90, 39, 3, 3);
    wall(84, 9, 10, true);
    wall(84, 18, 10, true);
    wall(84, 36, 10, true);
    wall(88, 10, 8, false);
    wall(92, 19, 8, false);
    wall(87, 27, 7, false);

    const auto widenNarrowRuns = [this](bool horizontal) {
        const unsigned int outer = horizontal ? pathfinder_.height() : pathfinder_.width();
        const unsigned int inner = horizontal ? pathfinder_.width() : pathfinder_.height();
        for (unsigned int fixed = 1; fixed + 1 < outer; ++fixed) {
            unsigned int start = 1;
            while (start + 1 < inner) {
                auto blocked = [this, horizontal, fixed](unsigned int variable) {
                    const sf::Vector2i cell = horizontal
                        ? sf::Vector2i{static_cast<int>(variable), static_cast<int>(fixed)}
                        : sf::Vector2i{static_cast<int>(fixed), static_cast<int>(variable)};
                    return pathfinder_.isBlocked(cell);
                };
                if (blocked(start)) {
                    ++start;
                    continue;
                }
                unsigned int end = start;
                while (end + 1 < inner && !blocked(end + 1)) ++end;
                if (end - start + 1 == 2) {
                    const unsigned int cellToOpen = end + 1 < inner - 1 ? end + 1 : start - 1;
                    const sf::Vector2i cell = horizontal
                        ? sf::Vector2i{static_cast<int>(cellToOpen), static_cast<int>(fixed)}
                        : sf::Vector2i{static_cast<int>(fixed), static_cast<int>(cellToOpen)};
                    pathfinder_.setBlocked(static_cast<unsigned int>(cell.x),
                                           static_cast<unsigned int>(cell.y), false);
                }
                start = end + 1;
            }
        }
    };
    widenNarrowRuns(true);
    widenNarrowRuns(false);
}

int Game::run() {
    sf::Clock clock;
    while (window_.isOpen()) {
        processEvents();
        const float deltaTime = std::min(clock.restart().asSeconds(), 0.05F);
        update(deltaTime);
        draw();
    }
    return 0;
}

void Game::processEvents() {
    while (const std::optional event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window_.close();
        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Escape) window_.close();
            if (key->code == sf::Keyboard::Key::F3) showCollisionMap_ = !showCollisionMap_;
            if (key->code == sf::Keyboard::Key::Space && screen_ == Screen::Playing) zombie_.useBoost();
            if (screen_ == Screen::Title) screen_ = Screen::Playing;
        }
        if (screen_ == Screen::Title && event->is<sf::Event::MouseButtonPressed>()) screen_ = Screen::Playing;
        if (screen_ == Screen::Playing) {
            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (click->button == sf::Mouse::Button::Left) {
                    const sf::Vector2f destination = window_.mapPixelToCoords(click->position, camera_);
                    zombie_.setDestination(destination, pathfinder_);
                    destination_ = destination;
                }
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (screen_ == Screen::Title) return;
    sf::Vector2f keyboardDirection;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) keyboardDirection.y -= 1.0F;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) keyboardDirection.y += 1.0F;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) keyboardDirection.x -= 1.0F;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) keyboardDirection.x += 1.0F;
    zombie_.chargeBoost(deltaTime * 5.0F);
    zombie_.regenerate(deltaTime * (100.0F / 60.0F));
    zombie_.moveManually(keyboardDirection, deltaTime, pathfinder_);
    zombie_.update(deltaTime, pathfinder_);
    for (Food& food : foods_) food.update(deltaTime);
    for (Npc& npc : npcs_) {
        npc.update(deltaTime, zombie_.position(), pathfinder_);
        if (npc.canShoot(zombie_.position(), deltaTime)) {
            const sf::Vector2f delta = zombie_.position() - npc.position();
            const float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
            if (length > 0.01F) {
                const sf::Vector2f velocity = delta / length * 145.0F;
                const sf::Vector2f direction = delta / length;
                const sf::Vector2f muzzlePosition = npc.position() + sf::Vector2f{0.0F, -28.0F} + direction * 14.0F;
                projectiles_.push_back({muzzlePosition, velocity,
                                        std::atan2(velocity.y, velocity.x) * 180.0F / 3.14159265F});
            }
        }
    }
    for (auto projectile = projectiles_.begin(); projectile != projectiles_.end();) {
        projectile->position += projectile->velocity * deltaTime;
        projectile->animationTimer += deltaTime;
        if (projectile->animationTimer >= 0.08F) {
            projectile->animationTimer = 0.0F;
            projectile->animationFrame = (projectile->animationFrame + 1) % 3;
        }
        const bool blocked = !pathfinder_.isWalkable(projectile->position);
        const bool hit = distance(projectile->position, zombie_.position()) < 14.0F;
        if (hit) zombie_.hurt(8.0F);
        if (blocked || hit) projectile = projectiles_.erase(projectile);
        else ++projectile;
    }
    for (Food& food : foods_) {
        if (!food.consumed() && distance(food.position(), zombie_.position()) < 25.0F) {
            food.consume();
            zombie_.eat(food.value());
        }
    }
    for (auto npc = npcs_.begin(); npc != npcs_.end();) {
        if (distance(npc->position(), zombie_.position()) < 24.0F) {
            const bool wasSoldier = npc->role() == Npc::Role::Soldier;
            foods_.emplace_back(npc->position(), Food::Type::Brain, &foodTexture_);
            zombie_.fillBoost();
            zombie_.regenerate(10.0F);
            score_ += 100;
            npc = npcs_.erase(npc);
            if (wasSoldier) {
                static std::mt19937 rng{std::random_device{}()};
                static const std::array<sf::Vector2f, 6> soldierRespawnPoints = {
                    sf::Vector2f{180.0F, 180.0F},
                    sf::Vector2f{420.0F, 680.0F},
                    sf::Vector2f{720.0F, 150.0F},
                    sf::Vector2f{1040.0F, 200.0F},
                    sf::Vector2f{1210.0F, 600.0F},
                    sf::Vector2f{280.0F, 430.0F}
                };
                std::uniform_int_distribution<int> dist(0, static_cast<int>(soldierRespawnPoints.size() - 1));
                const sf::Vector2f spawn = pathfinder_.nearestWalkablePosition(soldierRespawnPoints[dist(rng)], {10.0F, 10.0F});
                npcs_.emplace_back(spawn, Npc::Role::Soldier, &soldierTexture_);
            }
        } else {
            ++npc;
        }
    }
    while (score_ >= nextSoldierScore_ && npcs_.size() < 12) {
        static std::mt19937 rng{std::random_device{}()};
        static const std::array<sf::Vector2f, 6> soldierBonusSpawns = {
            sf::Vector2f{230.0F, 260.0F},
            sf::Vector2f{420.0F, 540.0F},
            sf::Vector2f{670.0F, 640.0F},
            sf::Vector2f{920.0F, 220.0F},
            sf::Vector2f{1080.0F, 620.0F},
            sf::Vector2f{1160.0F, 340.0F}
        };
        std::uniform_int_distribution<int> dist(0, static_cast<int>(soldierBonusSpawns.size() - 1));
        const sf::Vector2f spawn = pathfinder_.nearestWalkablePosition(soldierBonusSpawns[dist(rng)], {10.0F, 10.0F});
        npcs_.emplace_back(spawn, Npc::Role::Soldier, &soldierTexture_);
        nextSoldierScore_ += 1000;
    }
    const sf::Vector2f cameraCenter = camera_.getCenter();
    const sf::Vector2f cameraHalfSize = camera_.getSize() / 2.0F;
    const sf::Vector2f followMargin = cameraHalfSize * 0.55F;
    sf::Vector2f newCameraCenter = cameraCenter;
    if (zombie_.position().x < cameraCenter.x - followMargin.x) newCameraCenter.x = zombie_.position().x + followMargin.x;
    if (zombie_.position().x > cameraCenter.x + followMargin.x) newCameraCenter.x = zombie_.position().x - followMargin.x;
    if (zombie_.position().y < cameraCenter.y - followMargin.y) newCameraCenter.y = zombie_.position().y + followMargin.y;
    if (zombie_.position().y > cameraCenter.y + followMargin.y) newCameraCenter.y = zombie_.position().y - followMargin.y;
    camera_.setCenter(newCameraCenter);
    keepCameraInMap();
}

void Game::keepCameraInMap() {
    const sf::Vector2f size = camera_.getSize();
    const float mapWidth = static_cast<float>(MapCellsWide * CellSize);
    const float mapHeight = static_cast<float>(MapCellsHigh * CellSize);
    const float halfWidth = size.x / 2.0F;
    const float halfHeight = size.y / 2.0F;
    camera_.setCenter({std::clamp(camera_.getCenter().x, halfWidth, mapWidth - halfWidth),
                       std::clamp(camera_.getCenter().y, halfHeight, mapHeight - halfHeight)});
}

void Game::draw() {
    window_.clear(sf::Color(42, 54, 62));
    if (screen_ == Screen::Title) {
        drawTitle();
    } else {
        window_.setView(camera_);
        if (showCollisionMap_) drawCollisionMap();
        if (destination_.has_value()) {
            sf::CircleShape marker(10.0F);
            marker.setOrigin({10.0F, 10.0F});
            marker.setPosition(destination_.value());
            marker.setFillColor(sf::Color::Transparent);
            marker.setOutlineThickness(3.0F);
            marker.setOutlineColor(sf::Color(236, 224, 188));
            window_.draw(marker);
        }
        for (const Food& food : foods_) food.draw(window_);
        for (const Projectile& projectile : projectiles_) {
            if (projectileTexture_.getSize().x > 1) {
                const int frameWidth = static_cast<int>(projectileTexture_.getSize().x / 6);
                sf::Sprite shot(projectileTexture_);
                shot.setTextureRect({{static_cast<int>(projectile.animationFrame) * frameWidth, 560},
                                     {frameWidth, 440}});
                shot.setOrigin({frameWidth / 2.0F, 220.0F});
                shot.setRotation(sf::degrees(projectile.rotation));
                shot.setScale({0.08F, 0.08F});
                shot.setPosition(projectile.position);
                window_.draw(shot);
            } else {
                sf::CircleShape shot(4.0F);
                shot.setOrigin({4.0F, 4.0F});
                shot.setPosition(projectile.position);
                shot.setFillColor(sf::Color(255, 220, 90));
                window_.draw(shot);
            }
        }
        for (const Npc& npc : npcs_) npc.draw(window_);
        zombie_.draw(window_);
        window_.setView(window_.getDefaultView());
        drawHud();
    }
    window_.display();
}

void Game::drawCollisionMap() {
    sf::RectangleShape cell({static_cast<float>(pathfinder_.cellSize()),
                             static_cast<float>(pathfinder_.cellSize())});
    cell.setFillColor(sf::Color(188, 72, 62));
    cell.setOutlineThickness(1.0F);
    cell.setOutlineColor(sf::Color(235, 185, 105));
    for (unsigned int y = 0; y < pathfinder_.height(); ++y) {
        for (unsigned int x = 0; x < pathfinder_.width(); ++x) {
            if (!pathfinder_.isBlocked({static_cast<int>(x), static_cast<int>(y)})) continue;
            cell.setPosition({static_cast<float>(x * pathfinder_.cellSize()),
                              static_cast<float>(y * pathfinder_.cellSize())});
            window_.draw(cell);
        }
    }
}

void Game::drawTitle() {
    sf::RectangleShape panel({760.0F, 300.0F});
    panel.setPosition({260.0F, 180.0F});
    panel.setFillColor(sf::Color(24, 31, 38, 245));
    panel.setOutlineThickness(3.0F);
    panel.setOutlineColor(sf::Color(177, 53, 54));
    window_.draw(panel);
    if (!font_.getInfo().family.empty()) {
        sf::Text title(font_, "ZOMBIE A*", 54);
        title.setPosition({485.0F, 235.0F});
        title.setFillColor(sf::Color(236, 224, 188));
        window_.draw(title);
        sf::Text prompt(font_, "Clic o una tecla para comenzar", 22);
        prompt.setPosition({470.0F, 350.0F});
        prompt.setFillColor(sf::Color(147, 185, 185));
        window_.draw(prompt);
    }
}

void Game::drawHud() {
    sf::RectangleShape bar({230.0F, 18.0F});
    bar.setPosition({24.0F, 24.0F});
    bar.setFillColor(sf::Color(35, 35, 35, 230));
    window_.draw(bar);
    bar.setSize({230.0F * zombie_.health() / 100.0F, 18.0F});
    bar.setFillColor(sf::Color(120, 210, 100));
    window_.draw(bar);

    bar.setPosition({24.0F, 68.0F});
    bar.setSize({230.0F, 18.0F});
    bar.setFillColor(sf::Color(35, 35, 35, 230));
    window_.draw(bar);
    bar.setSize({230.0F * zombie_.hunger() / 100.0F, 18.0F});
    bar.setFillColor(sf::Color(216, 145, 49));
    window_.draw(bar);

    bar.setPosition({24.0F, 112.0F});
    bar.setSize({230.0F, 12.0F});
    bar.setFillColor(sf::Color(35, 35, 35, 230));
    window_.draw(bar);
    bar.setSize({230.0F * zombie_.boost() / 100.0F, 12.0F});
    bar.setFillColor(sf::Color(90, 190, 110));
    window_.draw(bar);

    if (!font_.getInfo().family.empty()) {
        sf::Text healthLabel(font_, "VIDA", 16);
        healthLabel.setPosition({28.0F, 42.0F});
        healthLabel.setFillColor(sf::Color::White);
        window_.draw(healthLabel);

        sf::Text hungerLabel(font_, "HAMBRE", 16);
        hungerLabel.setPosition({28.0F, 86.0F});
        hungerLabel.setFillColor(sf::Color::White);
        window_.draw(hungerLabel);

        sf::Text boostLabel(font_, "BOOST", 14);
        boostLabel.setPosition({28.0F, 96.0F});
        boostLabel.setFillColor(sf::Color::White);
        window_.draw(boostLabel);

        sf::Text scoreLabel(font_, "PUNTOS: " + std::to_string(score_), 18);
        scoreLabel.setPosition({1050.0F, 24.0F});
        scoreLabel.setFillColor(sf::Color::White);
        window_.draw(scoreLabel);
        sf::Text hint(font_, "Clic: mover zombie | ESPACIO: usar boost | ESC: salir", 16);
        hint.setPosition({24.0F, 680.0F});
        hint.setFillColor(sf::Color::White);
        window_.draw(hint);
    }
}
