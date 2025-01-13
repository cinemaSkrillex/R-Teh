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
      ipBox(sf::Vector2f(400, 50), sf::Vector2f(200, 150), "127.0.0.1", "../../../assets/arial.ttf", RealEngine::InputBox::ContentType::Text),
      portBox(sf::Vector2f(400, 50), sf::Vector2f(200, 250), "1212", "../../../assets/arial.ttf", RealEngine::InputBox::ContentType::Numeric),
      button(sf::Vector2f(250, 50)),
      buttonText("Connect to Server", "../../../assets/arial.ttf") {

    button.setPosition(275, 350);
    button.setFillColor(sf::Color::Green);
    buttonText.setCharacterSize(24);
    buttonText.setColor(255, 255, 255, 255);
    buttonText.setPosition(420, 365);

    ipBox.setFillColor(sf::Color::Green);
    portBox.setFillColor(sf::Color::Green);
    ipBox.centerText();
    portBox.centerText();
}

void Launcher::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.getRenderWindow().pollEvent(event)) {
            ipBox.handleEvent(event);
            portBox.handleEvent(event);

            if (event.type == sf::Event::MouseButtonPressed) {
                if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && event.mouseButton.button == sf::Mouse::Left) {
                    onConnectClick();
                }
            }

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        ipBox.draw(window.getRenderWindow());
        ipBox.drawDebug(window.getRenderWindow());  // Draw debug for ipBox
        portBox.draw(window.getRenderWindow());
        portBox.drawDebug(window.getRenderWindow());  // Draw debug for portBox
        window.getRenderWindow().draw(button);
        buttonText.draw(window.getRenderWindow());
        window.display();
    }
}



void Launcher::onConnectClick() {
    std::cout << "Button clicked, connecting to server..." << std::endl;

    std::thread clientThread(&Launcher::connectToServer, this);
    clientThread.detach();  // Detach the thread to allow it to run independently
}

void Launcher::connectToServer() {
    try {
        asio::io_context io_context;

        auto tcpclient = std::make_shared<TCPClient>(ipBox.getText(), static_cast<unsigned short>(std::stoi(portBox.getText())));

        std::thread ioThread([&io_context]() { io_context.run(); }); // Run io_context in a separate thread

        ioThread.join(); // Ensure the io_thread completes
        std::this_thread::sleep_for(std::chrono::hours(1));
        std::cout << "Client stopped" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}