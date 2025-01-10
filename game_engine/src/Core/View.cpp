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

void View::resizeWithAspectRatio(int width, int height) {
    float windowAspectRatio = static_cast<float>(width) / static_cast<float>(height);
    float viewAspectRatio   = static_cast<float>(VIEW_WIDTH) / static_cast<float>(VIEW_HEIGHT);

    float newWidth  = VIEW_HEIGHT * windowAspectRatio;
    float viewportX = (1.0f - (VIEW_WIDTH / newWidth)) / 2.0f;
    float newHeight = VIEW_WIDTH / windowAspectRatio;
    float viewportY = (1.0f - (VIEW_HEIGHT / newHeight)) / 2.0f;

    if (windowAspectRatio > viewAspectRatio) {
        _view.setSize(VIEW_WIDTH, VIEW_HEIGHT);
        _view.setViewport({viewportX, 0.0f, VIEW_WIDTH / newWidth, 1.0f});
    } else {
        _view.setSize(VIEW_WIDTH, VIEW_HEIGHT);
        _view.setViewport({0.0f, viewportY, 1.0f, VIEW_HEIGHT / newHeight});
    }
    _view.setCenter(VIEW_WIDTH / 2, VIEW_HEIGHT / 2);
}

}  // namespace RealEngine