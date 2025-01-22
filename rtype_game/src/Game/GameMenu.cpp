/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMenu
*/

#include "Game/GameMenu.hpp"

namespace rtype {

GameMenu::GameMenu(std::shared_ptr<UDPClient> clientUDP, RealEngine::Window& window,
                   RealEngine::ControlSystem& controls)
    : _window(window), _event(), _clientUDP(clientUDP), _controls(controls) {
    std::string path = "../../assets/sprites/r_type/";
    if (assetLauncher == true) {
        path = "assets/sprites/r_type/";
    }
    initButtons();
}

GameMenu::~GameMenu() {}

void GameMenu::initButtons() {
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
    _gameLogo = RealEngine::Sprite(*(AssetManagerInstance.getSprite("logo")));
    _gameLogo.setPosition(VIEW_WIDTH / 2, 50);
    _gameLogo.setSmooth(true);

    auto playSprite = *(AssetManagerInstance.getSprite("play_button"));
    playSprite.setPosition(400, 250);
    playSprite.setSmooth(true);
    _playButton = RealEngine::Button(playSprite);
    _playButton.setHoverColor(sf::Color::Cyan);

    auto exitSprite = *(AssetManagerInstance.getSprite("exit_button"));
    exitSprite.setPosition(600, 640);
    exitSprite.setSmooth(true);
    _exitButton = RealEngine::Button(exitSprite);
    _exitButton.setHoverColor(sf::Color::Yellow);

    auto plusSprite = *(AssetManagerInstance.getSprite("plus_button"));
    plusSprite.setPosition(620, 500);
    _gammaPlusButton = RealEngine::Button(plusSprite);
    _gammaPlusButton.setHoverColor(sf::Color::Green);

    auto minusSprite = *(AssetManagerInstance.getSprite("minus_button"));
    minusSprite.setPosition(380, 500);
    _gammaMinusButton = RealEngine::Button(minusSprite);
    _gammaMinusButton.setHoverColor(sf::Color::Red);

    plusSprite.setPosition(620, 575);
    _saturationPlusButton = RealEngine::Button(plusSprite);
    _saturationPlusButton.setHoverColor(sf::Color::Green);

    minusSprite.setPosition(380, 575);
    _saturationMinusButton = RealEngine::Button(minusSprite);
    _saturationMinusButton.setHoverColor(sf::Color::Red);

    _gammaText = RealEngine::Sprite(*(AssetManagerInstance.getSprite("gamma")));
    _gammaText.setPosition(500, 500);
    _gammaText.setScale(0.5, 0.5);
    _gammaText.setSmooth(true);
    _saturationText = RealEngine::Sprite(*(AssetManagerInstance.getSprite("saturation")));
    _saturationText.setPosition(500, 575);
    _saturationText.setScale(0.5, 0.5);

    _arial.loadFromFile("../../assets/fonts/arial.ttf");
    _upEventBox = RealEngine::Button({200, 60}, {100, 300}, "UP", _arial);
    _upEventBox.setFillColor(sf::Color::Yellow);
    _upEventBox.setTextColor(sf::Color::Black);
    _upEventBox.setHoverColor(sf::Color{255, 210, 0, 255});
    _downEventBox = RealEngine::Button({200, 60}, {100, 370}, "DOWN", _arial);
    _downEventBox.setFillColor(sf::Color::Yellow);
    _downEventBox.setTextColor(sf::Color::Black);
    _downEventBox.setHoverColor(sf::Color{255, 210, 0, 255});
    _leftEventBox = RealEngine::Button({200, 60}, {100, 440}, "LEFT", _arial);
    _leftEventBox.setFillColor(sf::Color::Yellow);
    _leftEventBox.setTextColor(sf::Color::Black);
    _leftEventBox.setHoverColor(sf::Color{255, 210, 0, 255});
    _rightEventBox = RealEngine::Button({200, 60}, {100, 510}, "RIGHT", _arial);
    _rightEventBox.setFillColor(sf::Color::Yellow);
    _rightEventBox.setTextColor(sf::Color::Black);
    _rightEventBox.setHoverColor(sf::Color{255, 210, 0, 255});
    _shootEventBox = RealEngine::Button({200, 60}, {100, 580}, "SHOOT", _arial);
    _shootEventBox.setFillColor(sf::Color::Yellow);
    _shootEventBox.setTextColor(sf::Color::Black);
    _shootEventBox.setHoverColor(sf::Color{255, 210, 0, 255});
}

void GameMenu::goToOnlineGame() {
    if (_startGame) return;
    _clientUDP->send_new_client();
    _startGame = true;
    _window.setBlackTransition(true, 1.f);
}

void GameMenu::assertKeyToAction(sf::Keyboard::Key key, RealEngine::Action action, bool hold) {
    _controls.replaceKey(key, action, hold);
}

void GameMenu::updateControlsStrings() {
    _upEventBox.setLabel("UP: " +
                         RealEngine::keyToString(_controls.getBoundKey(RealEngine::Action::Up)));
    _downEventBox.setLabel(
        "DOWN: " + RealEngine::keyToString(_controls.getBoundKey(RealEngine::Action::Down)));
    _leftEventBox.setLabel(
        "LEFT: " + RealEngine::keyToString(_controls.getBoundKey(RealEngine::Action::Left)));
    _rightEventBox.setLabel(
        "RIGHT: " + RealEngine::keyToString(_controls.getBoundKey(RealEngine::Action::Right)));
    _shootEventBox.setLabel(
        "SHOOT: " + RealEngine::keyToString(_controls.getBoundKey(RealEngine::Action::Action1)));
}

void GameMenu::assignActionToButton(KeyToAction keyToReplace, sf::Keyboard::Key key) {
    switch (_keyToAction) {
        case KeyToAction::UP:
            assertKeyToAction(key, RealEngine::Action::Up);
            break;
        case KeyToAction::DOWN:
            assertKeyToAction(key, RealEngine::Action::Down);
            break;
        case KeyToAction::LEFT:
            assertKeyToAction(key, RealEngine::Action::Left);
            break;
        case KeyToAction::RIGHT:
            assertKeyToAction(key, RealEngine::Action::Right);
            break;
        case KeyToAction::SHOOT:
            assertKeyToAction(key, RealEngine::Action::Action1, true);
            break;
        default:
            break;
    }
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
        if (_event.type == sf::Event::KeyPressed)
            assignActionToButton(_keyToAction, _event.key.code);
        _playButton.handleEvent(_event, [this]() { goToOnlineGame(); }, _window.getRenderWindow());
        _exitButton.handleEvent(_event, [this]() { _window.close(); }, _window.getRenderWindow());
        _gammaPlusButton.handleEvent(
            _event, [this]() { _window.setGamma(_window.getGamma() + 0.1f); },
            _window.getRenderWindow());
        _gammaMinusButton.handleEvent(
            _event, [this]() { _window.setGamma(_window.getGamma() - 0.1f); },
            _window.getRenderWindow());
        _saturationPlusButton.handleEvent(
            _event, [this]() { _window.setSaturation(_window.getSaturation() + 0.1f); },
            _window.getRenderWindow());
        _saturationMinusButton.handleEvent(
            _event, [this]() { _window.setSaturation(_window.getSaturation() - 0.1f); },
            _window.getRenderWindow());
        _upEventBox.handleEvent(
            _event, [this]() { _keyToAction = KeyToAction::UP; }, _window.getRenderWindow());
        _downEventBox.handleEvent(
            _event, [this]() { _keyToAction = KeyToAction::DOWN; }, _window.getRenderWindow());
        _leftEventBox.handleEvent(
            _event, [this]() { _keyToAction = KeyToAction::LEFT; }, _window.getRenderWindow());
        _rightEventBox.handleEvent(
            _event, [this]() { _keyToAction = KeyToAction::RIGHT; }, _window.getRenderWindow());
        _shootEventBox.handleEvent(
            _event, [this]() { _keyToAction = KeyToAction::SHOOT; }, _window.getRenderWindow());
    }
    updateControlsStrings();
    _gameLogo.draw(_window.getRenderTexture());
    _playButton.draw(_window.getRenderTexture());
    _exitButton.draw(_window.getRenderTexture());
    _gammaPlusButton.draw(_window.getRenderTexture());
    _gammaMinusButton.draw(_window.getRenderTexture());
    _saturationPlusButton.draw(_window.getRenderTexture());
    _saturationMinusButton.draw(_window.getRenderTexture());
    _gammaText.draw(_window.getRenderTexture());
    _saturationText.draw(_window.getRenderTexture());

    _upEventBox.draw(_window.getRenderTexture());
    _downEventBox.draw(_window.getRenderTexture());
    _leftEventBox.draw(_window.getRenderTexture());
    _rightEventBox.draw(_window.getRenderTexture());
    _shootEventBox.draw(_window.getRenderTexture());
    _window.update(deltaTime);
    _window.display();
    return true;
}

}  // namespace rtype
