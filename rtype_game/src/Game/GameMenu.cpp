/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMenu
*/

#include "Game/GameMenu.hpp"

namespace rtype {

GameMenu::GameMenu(std::shared_ptr<UDPClient> clientUDP, RealEngine::Window& window)
    : _window(window), _event(), _clientUDP(clientUDP) {
    std::string path = "../../assets/sprites/r_type/";
    if (assetLauncher == true) {
        path = "assets/sprites/r_type/";
    }
    init_buttons();
}

GameMenu::~GameMenu() {}

void GameMenu::init_buttons() {
    std::string path = "../../assets/sprites/r_type/";
    if (assetLauncher == true) {
        path = "assets/sprites/r_type/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadSpriteTextureAndScale("logo", path + "logo.png", {0.85, 0.85});
    AssetManagerInstance.loadSpriteTextureAndScale("play_button", path + "play.png", {0.85, 0.85});
    AssetManagerInstance.loadSpriteTextureAndScale("exit_button", path + "exit.png", {0.6, 0.6});
    AssetManagerInstance.loadSpriteTextureAndScale("plus_button", path + "plus.png", {1, 1});
    AssetManagerInstance.loadSpriteTextureAndScale("minus_button", path + "minus.png", {1, 1});
    AssetManagerInstance.loadSpriteTextureAndScale("gamma", path + "gamma.jpg", {1, 1});
    AssetManagerInstance.loadSpriteTextureAndScale("saturation", path + "saturation.jpg", {1, 1});
    _GameLogo = RealEngine::Sprite(*(AssetManagerInstance.getSprite("logo")));
    _GameLogo.setPosition(VIEW_WIDTH / 2, 50);
    _GameLogo.setSmooth(true);

    auto PlaySprite = *(AssetManagerInstance.getSprite("play_button"));
    PlaySprite.setPosition(400, 250);
    PlaySprite.setSmooth(true);
    _PlayButton = RealEngine::Button(PlaySprite);
    _PlayButton.setHoverColor(sf::Color::Cyan);

    auto ExitSprite = *(AssetManagerInstance.getSprite("exit_button"));
    ExitSprite.setPosition(600, 640);
    ExitSprite.setSmooth(true);
    _ExitButton = RealEngine::Button(ExitSprite);
    _ExitButton.setHoverColor(sf::Color::Yellow);

    auto plusSprite = *(AssetManagerInstance.getSprite("plus_button"));
    plusSprite.setPosition(620, 500);
    _GammaPlusButton = RealEngine::Button(plusSprite);
    _GammaPlusButton.setHoverColor(sf::Color::Green);

    auto minusSprite = *(AssetManagerInstance.getSprite("minus_button"));
    minusSprite.setPosition(380, 500);
    _GammaMinusButton = RealEngine::Button(minusSprite);
    _GammaMinusButton.setHoverColor(sf::Color::Red);

    plusSprite.setPosition(620, 575);
    _SaturationPlusButton = RealEngine::Button(plusSprite);
    _SaturationPlusButton.setHoverColor(sf::Color::Green);

    minusSprite.setPosition(380, 575);
    _SaturationMinusButton = RealEngine::Button(minusSprite);
    _SaturationMinusButton.setHoverColor(sf::Color::Red);

    _GammaText = RealEngine::Sprite(*(AssetManagerInstance.getSprite("gamma")));
    _GammaText.setPosition(500, 500);
    _GammaText.setScale(0.5, 0.5);
    _GammaText.setSmooth(true);
    _SaturationText = RealEngine::Sprite(*(AssetManagerInstance.getSprite("saturation")));
    _SaturationText.setPosition(500, 575);
    _SaturationText.setScale(0.5, 0.5);
}

void GameMenu::go_to_online_game() {
    if (_startGame) return;
    _clientUDP->send_new_client();
    _startGame = true;
    _window.setBlackTransition(true, 1.f);
}

bool GameMenu::run(float deltaTime) {
    _window.clear();
    if (_startGame && _window.getTransitionTimer() <= 0.f) {
        return false;
    }
    while (_window.getRenderWindow().pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window.close();
        }
        _PlayButton.handleEvent(
            _event, [this]() { go_to_online_game(); }, _window.getRenderWindow());
        _ExitButton.handleEvent(_event, [this]() { _window.close(); }, _window.getRenderWindow());
        _GammaPlusButton.handleEvent(
            _event, [this]() { _window.setGamma(_window.getGamma() + 0.1f); },
            _window.getRenderWindow());
        _GammaMinusButton.handleEvent(
            _event, [this]() { _window.setGamma(_window.getGamma() - 0.1f); },
            _window.getRenderWindow());
        _SaturationPlusButton.handleEvent(
            _event, [this]() { _window.setSaturation(_window.getSaturation() + 0.1f); },
            _window.getRenderWindow());
        _SaturationMinusButton.handleEvent(
            _event, [this]() { _window.setSaturation(_window.getSaturation() - 0.1f); },
            _window.getRenderWindow());
    }
    _GameLogo.draw(_window.getRenderTexture());
    _PlayButton.draw(_window.getRenderTexture());
    _ExitButton.draw(_window.getRenderTexture());
    _GammaPlusButton.draw(_window.getRenderTexture());
    _GammaMinusButton.draw(_window.getRenderTexture());
    _SaturationPlusButton.draw(_window.getRenderTexture());
    _SaturationMinusButton.draw(_window.getRenderTexture());
    _GammaText.draw(_window.getRenderTexture());
    _SaturationText.draw(_window.getRenderTexture());
    _window.update(deltaTime);
    _window.display();
    return true;
}

}  // namespace rtype
