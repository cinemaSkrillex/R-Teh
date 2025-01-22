#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#define VIEW_WIDTH 800
#define VIEW_HEIGHT 600

namespace RealEngine {
class View {
   public:
    View(const sf::Vector2f center, const sf::Vector2f size);
    ~View();

    void          setCenter(const sf::Vector2f center);
    void          setSize(const sf::Vector2f size);
    void          setRotation(float angle);
    void          setViewport(const sf::FloatRect viewport);
    void          move(const sf::Vector2f offset);
    void          rotate(float angle);
    void          zoom(float factor);
    sf::View&     getView() { return _view; }
    sf::Vector2f  getCenter() { return _view.getCenter(); }
    sf::Vector2f  getSize() { return _view.getSize(); }
    float         getRotation() { return _view.getRotation(); }
    sf::FloatRect getViewport() { return _view.getViewport(); }
    sf::Transform getTransform() { return _view.getTransform(); }
    sf::Transform getInverseTransform() { return _view.getInverseTransform(); }
    void          resizeWithAspectRatio(int width, int height);

   private:
    sf::View _view;
};
}  // namespace RealEngine