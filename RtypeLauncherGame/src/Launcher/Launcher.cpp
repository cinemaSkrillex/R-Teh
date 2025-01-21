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
            LauncherFont ? "arial.ttf" : "../../../assets/fonts/arial.ttf",
            RealEngine::InputBox::ContentType::IpAddress),
      portBox(sf::Vector2f(400, 50), sf::Vector2f(200, 250), "1212",
              LauncherFont ? "arial.ttf" : "../../../assets/fonts/arial.ttf",
              RealEngine::InputBox::ContentType::Numeric),
      button(sf::Vector2f(300, 50), sf::Vector2f(275, 350), "Connect to Server",
             LauncherFont ? "arial.ttf" : "../../../assets/fonts/arial.ttf"),
      launchButton(sf::Vector2f(300, 50), sf::Vector2f(275, 350), "R-Type",
                   LauncherFont ? "arial.ttf" : "../../../assets/fonts/arial.ttf"),
      launchButtonPong(sf::Vector2f(300, 50), sf::Vector2f(275, 450), "Pong",
                       LauncherFont ? "arial.ttf" : "../../../assets/fonts/arial.ttf"),
      LauncherText("Game Launcher", LauncherFont ? "arial.ttf" : "../../../assets/fonts/arial.ttf"),
      GameLauncherText("Choose Your Game",
                       LauncherFont ? "arial.ttf" : "../../../assets/fonts/arial.ttf") {
    button.setFillColor(sf::Color::Green);
    button.setTextColor(sf::Color::White);

    ipBox.setFillColor(sf::Color::Green);
    portBox.setFillColor(sf::Color::Green);

    launchButton.setFillColor(sf::Color::Green);
    launchButton.setTextColor(sf::Color::White);
    launchButtonPong.setFillColor(sf::Color::Green);
    launchButtonPong.setTextColor(sf::Color::White);
    ipBox.centerText();
    portBox.centerText();

    LauncherText.setCharacterSize(50);
    LauncherText.setPosition(350, 50);
    LauncherText.setColor(255, 255, 255, 255);

    GameLauncherText.setCharacterSize(50);
    GameLauncherText.setPosition(350, 90);
    GameLauncherText.setColor(255, 255, 255, 255);
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
                // launchButtonPong.handleEvent(event, [this]() { launchGame_Pong(); });
            }

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.update(0);
        if (!clientStopped) {
            ipBox.draw(window.getRenderTexture());
            portBox.draw(window.getRenderTexture());
            button.draw(window.getRenderTexture());
            LauncherText.draw(window.getRenderTexture());
        } else {
            launchButton.draw(window.getRenderTexture());
            // launchButtonPong.draw(window.getRenderTexture());
            GameLauncherText.draw(window.getRenderTexture());
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

void Launcher::launchGame_Pong() {
    std::cout << "Launching Pong" << std::endl;

    if (chdir("PongGame") == -1) {
        std::cerr << "Échec de chdir: " << strerror(errno) << std::endl;
        exit(1);
    }

    if (chmod("./pong", S_IRWXU) == -1) {
        std::cerr << "Échec de chmod: " << strerror(errno) << std::endl;
        exit(1);
    }

    const char* args[] = {"./pong", nullptr};
    execvp("./pong", const_cast<char* const*>(args));
    std::cerr << "Échec de execvp: " << strerror(errno) << std::endl;
    exit(1);
}

void Launcher::launchGame() {
    std::cout << "Launching game" << std::endl;

    if (chdir("RtypeGame") == -1) {
        std::cerr << "Échec de chdir: " << strerror(errno) << std::endl;
        exit(1);
    }

    if (chmod("./r_type", S_IRWXU) == -1) {
        std::cerr << "Échec de chmod: " << strerror(errno) << std::endl;
        exit(1);
    }

    const char* args[] = {"./r_type", nullptr};
    execvp("./r_type", const_cast<char* const*>(args));
    std::cerr << "Échec de execvp: " << strerror(errno) << std::endl;
    exit(1);
}

void Launcher::onConnectClick() {
    if (connectedButtonClicked) {
        return;
    }

    connectedButtonClicked = true;

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
        asio::io_context ioContext;

        auto tcpclient = std::make_shared<TCPClient>(
            ipBox.getText(), static_cast<unsigned short>(std::stoi(portBox.getText())));

        std::thread ioThread([&ioContext]() { ioContext.run(); });

        ioThread.join();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Client stopped" << std::endl;
        clientStopped = true;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}