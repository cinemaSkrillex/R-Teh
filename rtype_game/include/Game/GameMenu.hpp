/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMenu
*/

#pragma once

#include "Client/UDPClient.hpp"
#include "Engine.hpp"

namespace rtype {
class GameMenu {
   public:
    GameMenu(std::shared_ptr<UDPClient> clientUDP, RealEngine::Window& window);
    ~GameMenu();
    void init_buttons();

    void go_to_online_game();
    bool run(float deltaTime);

   private:
    RealEngine::Window&        _window;
    bool                       _startGame = false;
    std::shared_ptr<UDPClient> _clientUDP;
    sf::Event                  _event;
    RealEngine::Sprite         _GameLogo;
    RealEngine::Button         _PlayButton;

    RealEngine::Button _ExitButton;
    RealEngine::Button _GammaPlusButton;
    RealEngine::Button _GammaMinusButton;
    RealEngine::Button _SaturationPlusButton;
    RealEngine::Button _SaturationMinusButton;
    RealEngine::Sprite _GammaText;
    RealEngine::Sprite _SaturationText;
};
}  // namespace rtype