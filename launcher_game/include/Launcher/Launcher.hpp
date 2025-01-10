/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Launcher
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <asio.hpp>
#include <string>
#include <thread>

#include "../Client/TCPClient.hpp"
#include "../Media/Graphics/UI/Text.hpp"
#include "../Core/Window.hpp"

class Launcher {
   public:
    Launcher();
    void run();

   private:
    void onConnectClick();
    void connectToServer();
    void handleTextInput(sf::Event event);

    RealEngine::Window window;
    sf::RectangleShape button;
    RealEngine::Text   inputTextIP;
    RealEngine::Text   inputTextPort;
    sf::RectangleShape inputBoxIP;
    sf::RectangleShape inputBoxPort;
    std::string        ipInput;
    std::string        portInput;

    bool isEditingIP;
};
