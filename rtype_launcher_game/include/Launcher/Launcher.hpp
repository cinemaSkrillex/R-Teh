/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Launcher
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <asio.hpp>
#include <regex>
#include <string>
#include <thread>

#include "../Client/TCPClient.hpp"
#include "../Core/Graphics/UI/Button.hpp"
#include "../Core/Graphics/UI/InputBox.hpp"
#include "../Core/Graphics/UI/Text.hpp"
#include "../Core/Window.hpp"

class Launcher {
   public:
    Launcher();
    void run();
    bool isValidIp(const std::string& ip);
    bool isValidPort(const std::string& port);

   private:
    void onConnectClick();
    void connectToServer();
    void launchGame();

    RealEngine::Window   window;
    RealEngine::InputBox ipBox;
    RealEngine::InputBox portBox;
    RealEngine::Button   button;
    RealEngine::Button   launchButton;
    bool                 clientStopped = false;
};