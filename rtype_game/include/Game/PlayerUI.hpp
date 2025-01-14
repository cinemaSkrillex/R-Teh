/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** PlayerUI
*/

#pragma once

#define GAUGE_WIDTH 200
#define GAUGE_HEIGHT 40

#include "Engine.hpp"

namespace rtype {

class PlayerUI {
   public:
    PlayerUI(RealEngine::Registry& registry, sf::RenderWindow& window);
    ~PlayerUI();
    void  AssignPlayerToUI(std::shared_ptr<RealEngine::Entity> playerEntity);
    bool& getHoldShoot() { return _holdShoot; }
    void  update();

   private:
    int                                 _score;
    sf::RenderWindow&                   _window;
    RealEngine::Registry&               _registry;
    std::shared_ptr<RealEngine::Entity> _playerEntity;
    RealEngine::Health*                 _playerHealth;
    RealEngine::NetvarContainer*        _playerNetvarContainer;
    RealEngine::Score*                  _playerScore;
    sf::RectangleShape                  _background;
    sf::RectangleShape                  _healthBar;
    sf::RectangleShape                  _emptyHealthBar;
    sf::RectangleShape                  _shootLoad;
    sf::RectangleShape                  _emptyShootLoad;
    RealEngine::Text                    _scoreText;
    RealEngine::Text                    _healthText;
    RealEngine::Text                    _beamText;
    bool                                _holdShoot;
};

}  // namespace rtype