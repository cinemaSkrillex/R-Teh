/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** launch
*/

#include "Game/Game.hpp"

namespace rtype {

LaunchGame::LaunchGame()
    : window("Game Launcher", sf::Vector2u(800, 600)),
      ipBox(sf::Vector2f(400, 50), sf::Vector2f(200, 150), "127.0.0.1",
            assetLauncher ? "assets/fonts/arial.ttf" : "../../assets/fonts/arial.ttf",
            RealEngine::InputBox::ContentType::IpAddress),
      portBox(sf::Vector2f(400, 50), sf::Vector2f(200, 250), "1212",
              assetLauncher ? "assets/fonts/arial.ttf" : "../../assets/fonts/arial.ttf",
              RealEngine::InputBox::ContentType::Numeric),
      portBoxClient(sf::Vector2f(400, 50), sf::Vector2f(200, 350), "1213",
                    assetLauncher ? "assets/fonts/arial.ttf" : "../../assets/fonts/arial.ttf",
                    RealEngine::InputBox::ContentType::Numeric),
      launchButton(sf::Vector2f(275, 50), sf::Vector2f(275, 450), "Launch Game",
                   assetLauncher ? "assets/fonts/arial.ttf" : "../../assets/fonts/arial.ttf") {
    initUIComponents();
    initBackground();
}

LaunchGame::~LaunchGame() {
    delete launcherBackground;
}

void LaunchGame::initUIComponents() {
    ipBox.setFillColor(sf::Color::Green);
    portBox.setFillColor(sf::Color::Green);
    portBoxClient.setFillColor(sf::Color::Green);

    launchButton.setFillColor(sf::Color::Green);
    launchButton.setTextColor(sf::Color::White);
    ipBox.centerText();
    portBox.centerText();
    portBoxClient.centerText();
}


void LaunchGame::initBackground() {
    launcherBackground = new LauncherBackground(
        assetLauncher ? "assets/sprites/r_type/backgrounds/stars.png"
                      : "../../assets/sprites/r_type/backgrounds/stars.png",
        50.f);
}

void LaunchGame::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.getRenderWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                if (!infoProvided) {
                    std::cout << "You need to provide server IP and ports to start the game."
                              << std::endl;
                    exit(1);
                }
                window.close();
            }
            launchButton.handleEvent(event, [this]() {
                infoProvided = true;
                startGame();
            });
            ipBox.handleEvent(event);
            portBox.handleEvent(event);
            portBoxClient.handleEvent(event);
        }

        float deltaTime = clock.restart().asSeconds();
        launcherBackground->update(deltaTime);
        window.clear();
        draw();
        window.display();
    }
}


void LaunchGame::draw() {
    window.update(0);
    launcherBackground->draw(window.getRenderTexture());
    launchButton.draw(window.getRenderTexture());
    ipBox.draw(window.getRenderTexture());
    portBox.draw(window.getRenderTexture());
    portBoxClient.draw(window.getRenderTexture());
}

std::string LaunchGame::getServerIp() const { return serverIp; }

unsigned short LaunchGame::getServerPort() const { return serverPort; }

unsigned short LaunchGame::getClientPort() const { return clientPort; }

void rtype::LaunchGame::startGame() {
    if (!isValidIp(ipBox.getText()) || !isValidPort(portBox.getText()) ||
        !isValidPort(portBoxClient.getText())) {
        std::cerr << "Invalid IP or port." << std::endl;
        return;
    }

    serverIp   = ipBox.getText();
    serverPort = static_cast<unsigned short>(std::stoi(portBox.getText()));
    clientPort = static_cast<unsigned short>(std::stoi(portBoxClient.getText()));
    window.close();
}

bool LaunchGame::isValidIp(const std::string& ip) {
    const std::regex ip_pattern(
        R"(^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$)");
    return std::regex_match(ip, ip_pattern);
}

bool LaunchGame::isValidPort(const std::string& port) {
    const std::regex port_pattern(R"(^\d{4}$)");
    return std::regex_match(port, port_pattern);
}
}  // namespace rtype