/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMenu
*/

#pragma once

#include <iostream>

#include "Client/UDPClient.hpp"
#include "Engine.hpp"

namespace rtype {

enum KeyToAction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SHOOT,
};

class GameMenu {
   public:
    GameMenu(std::shared_ptr<UDPClient> clientUDP, RealEngine::Window& window,
             RealEngine::ControlSystem& controls);
    ~GameMenu();
    void initButtons();
    void updateButtonsStrings();

    void goToOnlineGame();
    void assignActionToButton(KeyToAction keyToReplace, sf::Keyboard::Key key);
    void assertKeyToAction(sf::Keyboard::Key key, RealEngine::Action action, bool hold = false);
    void updateControlsStrings();
    bool run(float deltaTime);

   private:
    RealEngine::Window&        _window;
    bool                       _startGame = false;
    std::shared_ptr<UDPClient> _clientUDP;
    RealEngine::ControlSystem& _controls;
    sf::Event                  _event;
    sf::Font                   _arial;
    KeyToAction                _keyToAction;

    RealEngine::Sprite _gameLogo;
    RealEngine::Button _playButton;
    RealEngine::Button _exitButton;
    RealEngine::Button _gammaPlusButton;
    RealEngine::Button _gammaMinusButton;
    RealEngine::Button _saturationPlusButton;
    RealEngine::Button _saturationMinusButton;
    RealEngine::Sprite _gammaText;
    RealEngine::Sprite _saturationText;
    RealEngine::Button _upEventBox;
    RealEngine::Button _downEventBox;
    RealEngine::Button _leftEventBox;
    RealEngine::Button _rightEventBox;
    RealEngine::Button _shootEventBox;
};
}  // namespace rtype
