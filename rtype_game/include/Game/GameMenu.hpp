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
    void init_text();

    void go_to_online_game();
    bool run(float deltaTime);

   private:
    RealEngine::Window&        _window;
    bool                       _startGame = false;
    std::shared_ptr<UDPClient> _clientUDP;
    sf::Event                  _event;
    RealEngine::Sprite         _GameLogo;
    RealEngine::Button         _PlayButton;

    RealEngine::Button   _ExitButton;
    RealEngine::Button   _GammaPlusButton;
    RealEngine::Button   _GammaMinusButton;
    RealEngine::Button   _SaturationPlusButton;
    RealEngine::Button   _SaturationMinusButton;
    RealEngine::Text     _GammaText;
    RealEngine::Text     _SaturationText;
    RealEngine::InputBox _InputBoxIP;
    std::string          _IP;
    RealEngine::InputBox _InputBoxPort;
    unsigned short       _server_port;
    RealEngine::InputBox _InputBoxClientPort;
    unsigned short       _client_port;
};
}  // namespace rtype