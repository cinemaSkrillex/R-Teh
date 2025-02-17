/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Launcher
*/

#pragma once

#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <asio.hpp>
#include <cstring>
#include <iostream>
#include <regex>
#include <string>
#include <thread>

#include "../Client/TCPClient.hpp"
#include "../Core/Graphics/UI/Button.hpp"
#include "../Core/Graphics/UI/InputBox.hpp"
#include "../Core/Graphics/UI/Text.hpp"
#include "../Core/Window.hpp"

#define LauncherFont false
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
    void launchGame_Pong();

    RealEngine::Window   window;
    RealEngine::InputBox ipBox;
    RealEngine::InputBox portBox;
    RealEngine::Button   button;
    RealEngine::Button   launchButton;
    RealEngine::Button   launchButtonPong;
    RealEngine::Text     LauncherText;
    RealEngine::Text     GameLauncherText;
    bool                 clientStopped          = false;
    bool                 connectedButtonClicked = false;
};