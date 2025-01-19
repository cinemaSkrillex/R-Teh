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

#include "LauncherBackground.hpp"
#include "Engine.hpp"

namespace rtype {
class LaunchGame {
   public:
    LaunchGame();
    ~LaunchGame();
    void           run();
    bool           isValidIp(const std::string& ip);
    bool           isValidPort(const std::string& port);
    std::string    getServerIp() const;
    unsigned short getServerPort() const;
    unsigned short getClientPort() const;

    bool infoProvided = false;

   private:
    void startGame();
    void initUIComponents();
    void initBackground();
    void draw();

    RealEngine::Window   window;
    RealEngine::Registry registry;
    LauncherBackground* launcherBackground;
    sf::Clock            clock;



    RealEngine::InputBox ipBox;
    RealEngine::InputBox portBox;
    RealEngine::InputBox portBoxClient;
    RealEngine::Button   launchButton;

    std::string    serverIp;
    unsigned short serverPort;
    unsigned short clientPort;
};
}  // namespace rtype
