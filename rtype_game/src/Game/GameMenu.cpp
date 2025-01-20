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
      _event(),
      _clientUDP(clientUDP),
      _IP("127.0.0.1"),
      _server_port(static_cast<unsigned short>(8080)),
      _client_port(static_cast<unsigned short>(8081)) {
    std::string path = "../../assets/sprites/r_type/";
    if (assetLauncher == true) {
        path = "assets/sprites/r_type/";
    }
    init_buttons();
    init_text();
}

GameMenu::~GameMenu() {}

void GameMenu::init_buttons() {
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

    auto PlaySprite = *(AssetManagerInstance.getSprite("play_button"));
    PlaySprite.setPosition(400, 400);
    PlaySprite.setSmooth(true);
    _PlayButton = RealEngine::Button(PlaySprite);
    _PlayButton.setHoverColor(sf::Color::Cyan);

    auto ExitSprite = *(AssetManagerInstance.getSprite("exit_button"));
    ExitSprite.setPosition(600, 640);
    ExitSprite.setSmooth(true);
    _ExitButton = RealEngine::Button(ExitSprite);
    _ExitButton.setHoverColor(sf::Color::Yellow);

    auto plusSprite = *(AssetManagerInstance.getSprite("plus_button"));
    plusSprite.setPosition(600, 500);
    _GammaPlusButton = RealEngine::Button(plusSprite);
    _GammaPlusButton.setHoverColor(sf::Color::Green);

    auto minusSprite = *(AssetManagerInstance.getSprite("minus_button"));
    minusSprite.setPosition(400, 500);
    _GammaMinusButton = RealEngine::Button(minusSprite);
    _GammaMinusButton.setHoverColor(sf::Color::Red);

    plusSprite.setPosition(600, 575);
    _SaturationPlusButton = RealEngine::Button(plusSprite);
    _SaturationPlusButton.setHoverColor(sf::Color::Green);

    minusSprite.setPosition(400, 575);
    _SaturationMinusButton = RealEngine::Button(minusSprite);
    _SaturationMinusButton.setHoverColor(sf::Color::Red);
}

void GameMenu::init_text() {
    std::string path = "../../assets/fonts/";
    if (assetLauncher == true) {
        path = "assets/fonts/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadFont("arial", path + "arial.ttf");
    auto font = AssetManagerInstance.getFont("arial");
    if (font) {
        _GammaText      = RealEngine::Text("Gamma", *font);
        _SaturationText = RealEngine::Text("Saturation", *font);
        _GammaText.setPosition(500, 500);
        _GammaText.setCharacterSize(30);
        _GammaText.setColor(255, 255, 255, 255);
        _SaturationText.setPosition(500, 575);
        _SaturationText.setCharacterSize(30);
        _SaturationText.setColor(255, 255, 255, 255);
        // _InputBoxIP =
        //     RealEngine::InputBox(sf::Vector2f(500, 150), sf::Vector2f(200, 150), "127.0.0.1",
        //     font,
        //                          RealEngine::InputBox::ContentType::IpAddress);
        // _InputBoxPort = RealEngine::InputBox(sf::Vector2f(100, 150), sf::Vector2f(200, 250),
        // "8080",
        //                                      font, RealEngine::InputBox::ContentType::Numeric);
        // _InputBoxClientPort =
        //     RealEngine::InputBox(sf::Vector2f(200, 250), sf::Vector2f(200, 350), "8081", font,
        //                          RealEngine::InputBox::ContentType::Numeric);
    }
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
