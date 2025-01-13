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
#include "../Core/Graphics/UI/InputBox.hpp"
#include "../Core/Graphics/UI/Text.hpp"
#include "../Core/Window.hpp"

class Launcher {
   public:
    Launcher();
    void run();

   private:
    void onConnectClick();
    void connectToServer();

    RealEngine::Window   window;
    RealEngine::InputBox ipBox;
    RealEngine::InputBox portBox;
    sf::RectangleShape   button;
    RealEngine::Text     buttonText;
};