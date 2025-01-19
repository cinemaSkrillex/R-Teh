/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMenu
*/

#include "Game/GameMenu.hpp"

namespace rtype {

GameMenu::GameMenu(std::shared_ptr<UDPClient> clientUDP, RealEngine::Window& window)
    : _window(window),
      _clientUDP(clientUDP),
      _IP("127.0.0.1"),
      _server_port(static_cast<unsigned short>(8080)),
      _client_port(static_cast<unsigned short>(8081)) {
    std::string path = "../../assets/sprites/r_type/";
    if (assetLauncher == true) {
        path = "assets/sprites/r_type/";
    }
    init_sprites();
    init_text();
}

GameMenu::~GameMenu() {}

void GameMenu::init_sprites() {
    std::string path = "../../assets/sprites/r_type/";
    if (assetLauncher == true) {
        path = "assets/sprites/r_type/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadSpriteTextureAndScale("logo", path + "logo.png", {0.85, 0.85});
    AssetManagerInstance.loadSpriteTextureAndScale("play_button", path + "play.png", {0.6, 0.6});
    AssetManagerInstance.loadSpriteTextureAndScale("exit_button", path + "exit.png", {0.6, 0.6});
    AssetManagerInstance.loadSpriteTextureAndScale("plus_button", path + "plus.png");
    AssetManagerInstance.loadSpriteTextureAndScale("minus_button", path + "minus.png");
    _GameLogo = RealEngine::Sprite(*(AssetManagerInstance.getSprite("logo")));
    _GameLogo.setPosition(VIEW_WIDTH / 2, 50);
    _GameLogo.setSmooth(true);
    _PlayButton = RealEngine::Sprite(*(AssetManagerInstance.getSprite("play_button")));
    _PlayButton.setPosition(VIEW_WIDTH / 2, 400);
    _PlayButton.setSmooth(true);
    _PlayButtonUI = RealEngine::Button(_PlayButton);
    _PlayButtonUI.setHoverColor(sf::Color::Green);
    _ExitButton = RealEngine::Sprite(*(AssetManagerInstance.getSprite("exit_button")));
    _ExitButton.setPosition(600, 640);
    _ExitButton.setSmooth(true);
    _GammaPlusButton = RealEngine::Sprite(*(AssetManagerInstance.getSprite("plus_button")));
    _GammaPlusButton.setPosition(600, 500);
    _GammaMinusButton = RealEngine::Sprite(*(AssetManagerInstance.getSprite("minus_button")));
    _GammaMinusButton.setPosition(400, 500);
    _SaturationPlusButton = RealEngine::Sprite(*(AssetManagerInstance.getSprite("plus_button")));
    _SaturationPlusButton.setPosition(600, 575);
    _SaturationMinusButton = RealEngine::Sprite(*(AssetManagerInstance.getSprite("minus_button")));
    _SaturationMinusButton.setPosition(400, 575);
}

void GameMenu::init_text() {
    std::string path = "../../assets/fonts/";
    if (assetLauncher == true) {
        path = "assets/fonts/";
    }
    _GammaText      = RealEngine::Text("Gamma", path + "arial.ttf");
    _SaturationText = RealEngine::Text("Saturation", path + "arial.ttf");
    _GammaText.setPosition(500, 500);
    _GammaText.setCharacterSize(30);
    _GammaText.setColor(255, 255, 255, 255);
    _SaturationText.setPosition(500, 575);
    _SaturationText.setCharacterSize(30);
    _SaturationText.setColor(255, 255, 255, 255);
    // _InputBoxIP =
    //     RealEngine::InputBox(sf::Vector2f(400, 50), sf::Vector2f(200, 150), "127.0.0.1",
    //                          path + "arial.ttf", RealEngine::InputBox::ContentType::IpAddress);
    // _InputBoxPort =
    //     RealEngine::InputBox(sf::Vector2f(400, 50), sf::Vector2f(200, 250), "8080",
    //                          path + "arial.ttf", RealEngine::InputBox::ContentType::Numeric);
    // _InputBoxClientPort =
    //     RealEngine::InputBox(sf::Vector2f(400, 50), sf::Vector2f(200, 350), "8081",
    //                          path + "arial.ttf", RealEngine::InputBox::ContentType::Numeric);
}

void GameMenu::go_to_online_game() {
    _clientUDP->send_new_client();
    _startGame = true;
    _window.setBlackTransition(true, 1.f);
}

bool GameMenu::run(float deltaTime) {
    _window.clear();
    if (_startGame && _window.getTransitionTimer() <= 0.f) {
        return false;
    }
    while (_window.getRenderWindow().pollEvent(_window.getEvent())) {
        _PlayButtonUI.handleEvent(_window.getEvent(), [this]() { go_to_online_game(); });
    }
    _GameLogo.draw(_window.getRenderTexture());
    _PlayButtonUI.draw(_window.getRenderTexture());
    _ExitButton.draw(_window.getRenderTexture());
    _GammaPlusButton.draw(_window.getRenderTexture());
    _GammaMinusButton.draw(_window.getRenderTexture());
    _SaturationPlusButton.draw(_window.getRenderTexture());
    _SaturationMinusButton.draw(_window.getRenderTexture());
    // _GammaText.draw(_window.getRenderTexture());
    // _SaturationText.draw(_window.getRenderTexture());
    // _InputBoxIP.draw(_window.getRenderTexture());
    // _InputBoxPort.draw(_window.getRenderTexture());
    // _InputBoxClientPort.draw(_window.getRenderTexture());
    _window.update(deltaTime);
    _window.display();
    return true;
}

}  // namespace rtype
