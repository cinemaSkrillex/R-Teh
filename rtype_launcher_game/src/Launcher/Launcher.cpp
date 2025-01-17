/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Launcher.cpp
*/

#include "../../include/Launcher/Launcher.hpp"

#include <iostream>

Launcher::Launcher()
    : window("Game Launcher", sf::Vector2u(800, 600)),
      ipBox(sf::Vector2f(400, 50), sf::Vector2f(200, 150), "127.0.0.1",
            "../../../assets/fonts/arial.ttf", RealEngine::InputBox::ContentType::Text),
      portBox(sf::Vector2f(400, 50), sf::Vector2f(200, 250), "1212",
              "../../../assets/fonts/arial.ttf", RealEngine::InputBox::ContentType::Numeric),
      button(sf::Vector2f(275, 50), sf::Vector2f(275, 350), "Connect to Server",
             "../../../assets/fonts/arial.ttf"),
      launchButton(sf::Vector2f(350, 60), sf::Vector2f(270, 265), "Launch Game",
                   "../../../assets/fonts/arial.ttf") {
    button.setFillColor(sf::Color::Green);
    button.setTextColor(sf::Color::White);

    ipBox.setFillColor(sf::Color::Green);
    portBox.setFillColor(sf::Color::Green);

    launchButton.setFillColor(sf::Color::Green);
    launchButton.setTextColor(sf::Color::White);
    ipBox.centerText();
    portBox.centerText();
}

void Launcher::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.getRenderWindow().pollEvent(event)) {
            if (!clientStopped) {
                button.handleEvent(event, [this]() { onConnectClick(); });
                ipBox.handleEvent(event);
                portBox.handleEvent(event);
            } else {
                launchButton.handleEvent(event, [this]() { launchGame(); });
            }

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        if (!clientStopped) {
            ipBox.draw(window.getRenderTexture());
            portBox.draw(window.getRenderTexture());
            button.draw(window.getRenderTexture());
        } else {
            launchButton.draw(window.getRenderTexture());
        }
        window.display();
    }
}

bool Launcher::isValidIp(const std::string& ip) {
    const std::regex ip_pattern(
        R"(^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$)");
    return std::regex_match(ip, ip_pattern);
}

bool Launcher::isValidPort(const std::string& port) {
    const std::regex port_pattern(R"(^\d{4}$)");
    return std::regex_match(port, port_pattern);
}

void Launcher::launchGame() { std::cout << "Launching game" << std::endl; }

void Launcher::onConnectClick() {
    std::string ip   = ipBox.getText();
    std::string port = portBox.getText();

    if (!isValidIp(ip)) {
        std::cout << "Invalid IP address: " << ip << std::endl;
        return;
    }

    if (!isValidPort(port)) {
        std::cout << "Invalid port number: " << port << std::endl;
        return;
    }

    std::cout << "Button clicked, connecting to server with IP: " << ip << " and port: " << port
              << std::endl;

    std::thread clientThread(&Launcher::connectToServer, this);
    clientThread.detach();
}

void Launcher::connectToServer() {
    try {
        asio::io_context io_context;

        auto tcpclient = std::make_shared<TCPClient>(
            ipBox.getText(), static_cast<unsigned short>(std::stoi(portBox.getText())));

        std::thread ioThread([&io_context]() { io_context.run(); });

        ioThread.join();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Client stopped" << std::endl;
        clientStopped = true;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}