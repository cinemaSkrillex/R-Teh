#include "../include/Core/View.hpp"

namespace RealEngine {
View::View(const sf::Vector2f center, const sf::Vector2f size) {
    _view.setCenter(center);
    _view.setSize(size);
}

View::~View() {}

void View::setCenter(const sf::Vector2f center) { _view.setCenter(center); }

void View::setSize(const sf::Vector2f size) { _view.setSize(size); }

void View::setRotation(float angle) { _view.setRotation(angle); }

void View::setViewport(const sf::FloatRect viewport) { _view.setViewport(viewport); }

void View::move(const sf::Vector2f offset) { _view.move(offset); }

void View::rotate(float angle) { _view.rotate(angle); }

void View::zoom(float factor) { _view.zoom(factor); }

}  // namespace RealEngine