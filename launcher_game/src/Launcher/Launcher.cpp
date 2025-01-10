/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Launcher
*/

#include "../../include/Launcher/Launcher.hpp"
#include <iostream>

Launcher::Launcher()
    : ipInput("127.0.0.1"), portInput("1212"),
      inputTextIP("127.0.0.1", "../../../assets/arial.ttf"), inputTextPort("1212", "../../../assets/arial.ttf"),
      isEditingIP(true),
      window("Game Launcher", sf::Vector2u(800, 600))
{
    inputBoxIP.setSize(sf::Vector2f(400, 50));
    inputBoxIP.setPosition(200, 150);
    inputBoxIP.setFillColor(sf::Color::White);

    inputTextIP.setPosition(400, 160);
    inputTextIP.setCharacterSize(24);
    inputTextIP.setColor(0, 0, 0, 255);
    inputTextIP.center();

    inputBoxPort.setSize(sf::Vector2f(400, 50));
    inputBoxPort.setPosition(200, 250);
    inputBoxPort.setFillColor(sf::Color::White);

    inputTextPort.setPosition(400, 260);
    inputTextPort.setCharacterSize(24);
    inputTextPort.setColor(0, 0, 0, 255);
    inputTextPort.center();

    button.setSize(sf::Vector2f(200, 50));
    button.setPosition(300, 350);
    button.setFillColor(sf::Color::Green);

    RealEngine::Text buttonText("Connect to Server", "../../../assets/arial.ttf");
    buttonText.setCharacterSize(24);
    buttonText.setColor(255, 255, 255, 255);
    buttonText.setPosition(320, 365);
}

void Launcher::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.getRenderWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (inputBoxIP.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    isEditingIP = true;
                } else if (inputBoxPort.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    isEditingIP = false;
                }
            }
            if (event.type == sf::Event::TextEntered) {
                handleTextInput(event);
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left &&
                button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                onConnectClick();
            }
        }

        window.clear();
        window.getRenderWindow().draw(inputBoxIP);
        inputTextIP.draw(window.getRenderWindow());
        window.getRenderWindow().draw(inputBoxPort);
        inputTextPort.draw(window.getRenderWindow());
        window.getRenderWindow().draw(button);
        window.display();
    }
}

void Launcher::handleTextInput(sf::Event event) {
    if (event.text.unicode < 128) {
        if (event.text.unicode == 8) {
            if (isEditingIP && !ipInput.empty()) {
                ipInput.pop_back();
            } else if (!isEditingIP && !portInput.empty()) {
                portInput.pop_back();
            }
        } else if (event.text.unicode == 13) { 
        } else {
            if (isEditingIP) {
                if (ipInput.length() < 15) {
                    ipInput += static_cast<char>(event.text.unicode);
                }
            } else {
                if (portInput.length() < 4) {
                    portInput += static_cast<char>(event.text.unicode);
                }
            }
        }
        inputTextIP.setString(ipInput);
        inputTextPort.setString(portInput);
        inputTextIP.center();
        inputTextPort.center();
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

        auto tcpclient = std::make_shared<TCPClient>(ipInput, static_cast<unsigned short>(std::stoi(portInput)));

        std::thread ioThread([&io_context]() { io_context.run(); }); // Run io_context in a separate thread

        ioThread.join(); // Ensure the io_thread completes
        std::this_thread::sleep_for(std::chrono::hours(1));
        std::cout << "Client stopped" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
