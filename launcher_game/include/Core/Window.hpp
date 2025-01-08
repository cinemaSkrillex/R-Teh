#pragma once

#include <SFML/Graphics.hpp>

namespace RealEngine {
class Window {
   public:
    Window(const std::string title, const sf::Vector2u size);
    ~Window();

    void              clear();
    void              display();
    void              update();
    void              close();
    void              setTitle(const std::string title);
    void              setSize(const sf::Vector2u size);
    void              setStyle(sf::Uint32 style);
    void              setView(sf::View& view);
    bool              isOpen();
    bool              isFocused();
    sf::RenderWindow& getRenderWindow() { return _window; }

   private:
    sf::RenderWindow _window;
    sf::Event        _event;
    sf::Vector2u     _size;
    std::string      _title;
    sf::Uint32       _style;
};
}  // namespace RealEngine