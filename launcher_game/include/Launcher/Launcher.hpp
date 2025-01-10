#pragma once

#include <SFML/Graphics.hpp>
#include <asio.hpp>
#include <string>
#include <thread>

#include "../Client/TCPClient.hpp"
#include "../Core/Graphics/UI/Text.hpp"

class Launcher {
   public:
    Launcher();
    void run();

   private:
    void onConnectClick();
    void connectToServer();
    void handleTextInput(sf::Event event);

    sf::RenderWindow   window;
    sf::RectangleShape button;
    RealEngine::Text   inputTextIP;
    RealEngine::Text   inputTextPort;
    sf::RectangleShape inputBoxIP;
    sf::RectangleShape inputBoxPort;
    std::string        ipInput;
    std::string        portInput;

    bool isEditingIP;
};
