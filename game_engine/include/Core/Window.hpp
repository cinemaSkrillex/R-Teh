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
    bool              isOpen();
    sf::RenderWindow* getRenderWindow() { return &_window; }

  private:
    sf::RenderWindow _window;
    sf::Vector2u     _size;
    std::string      _title;
};
} // namespace RealEngine