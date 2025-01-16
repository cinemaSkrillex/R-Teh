/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** PlayerUI
*/

#include "Game/PlayerUI.hpp"

namespace rtype {
PlayerUI::PlayerUI(RealEngine::Registry& registry, sf::RenderTexture& window)
    : _registry(registry),
      _window(window),
      _score(0),
      _scoreText("", "../../assets/fonts/Early_GameBoy.ttf"),
      _healthText("", "../../assets/fonts/Early_GameBoy.ttf"),
      _beamText("", "../../assets/fonts/Early_GameBoy.ttf"),
      _playerEntity(nullptr) {
    float horizontal_panel_center = VIEW_WIDTH / 2;
    float vertical_panel_center   = VIEW_HEIGHT + 50;
    _background.setOrigin({VIEW_WIDTH / 2, 100 / 2});
    _background.setPosition({VIEW_WIDTH / 2, VIEW_HEIGHT + 50});
    _background.setSize({VIEW_WIDTH, 100});
    _background.setFillColor(sf::Color{20, 20, 20, 255});
    // score
    _scoreText.setPosition(horizontal_panel_center / 4, vertical_panel_center);
    _scoreText.setCharacterSize(20);
    _scoreText.setColor(255, 255, 255, 255);
    // shoot load
    _beamText.setCharacterSize(20);
    _beamText.setPosition(horizontal_panel_center - GAUGE_WIDTH / 3,
                          vertical_panel_center - GAUGE_HEIGHT + 10);
    _beamText.setString("Beam: ");
    _beamText.setColor(255, 255, 255, 255);
    _shootLoad.setOrigin({GAUGE_WIDTH / 2, GAUGE_HEIGHT / 2});
    _shootLoad.setSize({GAUGE_WIDTH, GAUGE_HEIGHT});
    _shootLoad.setPosition({horizontal_panel_center - GAUGE_WIDTH / 3, vertical_panel_center + 20});
    _shootLoad.setFillColor(sf::Color::Blue);
    _emptyShootLoad.setOrigin({GAUGE_WIDTH / 2, GAUGE_HEIGHT / 2});
    _emptyShootLoad.setSize({GAUGE_WIDTH, GAUGE_HEIGHT});
    _emptyShootLoad.setPosition(
        {horizontal_panel_center - GAUGE_WIDTH / 3, vertical_panel_center + 20});
    _emptyShootLoad.setOutlineColor(sf::Color::White);
    _emptyShootLoad.setOutlineThickness(1);
    _emptyShootLoad.setFillColor(sf::Color::Transparent);
    // health
    _healthText.setCharacterSize(20);
    _healthText.setPosition(horizontal_panel_center * 1.5f, vertical_panel_center + 20);
    _healthText.setColor(255, 255, 255, 255);
    _healthBar.setOrigin({GAUGE_WIDTH / 2, GAUGE_HEIGHT / 2});
    _healthBar.setSize({GAUGE_WIDTH, GAUGE_HEIGHT});
    _healthBar.setPosition({horizontal_panel_center * 1.5f, vertical_panel_center - 20});
    _healthBar.setFillColor(sf::Color::Green);
    _emptyHealthBar.setOrigin({GAUGE_WIDTH / 2, GAUGE_HEIGHT / 2});
    _emptyHealthBar.setSize({GAUGE_WIDTH, GAUGE_HEIGHT});
    _emptyHealthBar.setPosition({horizontal_panel_center * 1.5f, vertical_panel_center - 20});
    _emptyHealthBar.setOutlineColor(sf::Color{125, 125, 125});
    _emptyHealthBar.setOutlineThickness(1);
    _emptyHealthBar.setFillColor(sf::Color::Red);
}

PlayerUI::~PlayerUI() {}

void PlayerUI::AssignPlayerToUI(std::shared_ptr<RealEngine::Entity> playerEntity) {
    _playerEntity = playerEntity;
}

void PlayerUI::update() {
    auto playerHealth = _registry.get_component<RealEngine::Health>(_playerEntity);
    auto playerScore  = _registry.get_component<RealEngine::Score>(_playerEntity);
    auto playerNetvar = _registry.get_component<RealEngine::NetvarContainer>(_playerEntity);
    if (_playerEntity && playerHealth && playerNetvar && playerScore) {
        try {
            float shootLoadValue =
                std::any_cast<float>(playerNetvar->getNetvar("hold_shoot")->value);
            float maxShootTime = 0.75f / 2.f;
            shootLoadValue     = std::clamp(shootLoadValue, 0.0f, maxShootTime);

            if (playerHealth->amount <= 0) {
                _score += playerScore->amount;
            }
            _healthBar.setSize({(GAUGE_WIDTH * float(float(playerHealth->amount) /
                                                     float(playerHealth->maxHealth))),
                                GAUGE_HEIGHT});
            _scoreText.setString(
                std::string("Score: \n" + std::to_string(_score + playerScore->amount)));
            _healthText.setString(std::string(std::to_string(playerHealth->amount) + " ~ " +
                                              std::to_string(playerHealth->maxHealth) + " HP"));

            if (shootLoadValue > 0.f) {
                _shootLoad.setSize({(shootLoadValue / maxShootTime) * GAUGE_WIDTH, GAUGE_HEIGHT});
            } else {
                _shootLoad.setSize({shootLoadValue * GAUGE_WIDTH, GAUGE_HEIGHT});
            }
        } catch (const std::exception& e) {
            std::cout << "Error in UI update : [" << e.what() << "]" << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
    _window.draw(_background);
    _window.draw(_emptyHealthBar);
    _window.draw(_healthBar);
    _window.draw(_emptyShootLoad);
    _window.draw(_shootLoad);
    _window.draw(_scoreText.getText());
    _window.draw(_healthText.getText());
    _window.draw(_beamText.getText());
}

}  // namespace rtype