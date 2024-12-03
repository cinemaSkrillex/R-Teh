#include "../include/Core/View.hpp"

namespace RealEngine {
View::View(sf::RenderWindow& window, const sf::Vector2f center, const sf::Vector2f size)
    : _window(window) {
    _view.setCenter(center);
    _view.setSize(size);
}

View::~View() {}

void View::setCenter(const sf::Vector2f center) {
    _view.setCenter(center);
    _window.setView(_view);
}

void View::setSize(const sf::Vector2f size) {
    _view.setSize(size);
    _window.setView(_view);
}

void View::setRotation(float angle) {
    _view.setRotation(angle);
    _window.setView(_view);
}

void View::setViewport(const sf::FloatRect viewport) {
    _view.setViewport(viewport);
    _window.setView(_view);
}

void View::move(const sf::Vector2f offset) {
    _view.move(offset);
    _window.setView(_view);
}

void View::rotate(float angle) {
    _view.rotate(angle);
    _window.setView(_view);
}

void View::zoom(float factor) {
    _view.zoom(factor);
    _window.setView(_view);
}

}  // namespace RealEngine