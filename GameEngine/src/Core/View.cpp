#include "../include/Core/View.hpp"

namespace RealEngine {
View::View(const sf::Vector2f center, const sf::Vector2f size) {
    _view.setCenter(center);
    _view.setSize(size);
}

View::~View() {}

void View::setCenter(const sf::Vector2f center) { _view.setCenter(center); }

void View::setSize(const sf::Vector2f size) {
    if (size.x < 1 || size.y < 1) {
        std::cerr << "View size must be greater than 0" << std::endl;
    }
    _view.setSize(size);
}

void View::setRotation(float angle) { _view.setRotation(angle); }

void View::setViewport(const sf::FloatRect viewport) { _view.setViewport(viewport); }

void View::move(const sf::Vector2f offset) { _view.move(offset); }

void View::rotate(float angle) { _view.rotate(angle); }

void View::zoom(float factor) { _view.zoom(factor); }

void View::resizeWithAspectRatio(int width, int height) {
    float viewWidth         = _view.getSize().x;
    float viewHeight        = _view.getSize().y;
    float viewAspectRatio   = static_cast<float>(viewWidth) / static_cast<float>(viewHeight);
    float windowAspectRatio = static_cast<float>(width) / static_cast<float>(height);

    float newWidth  = viewHeight * windowAspectRatio;
    float viewportX = (1.0f - (viewWidth / newWidth)) / 2.0f;
    float newHeight = viewWidth / windowAspectRatio;
    float viewportY = (1.0f - (viewHeight / newHeight)) / 2.0f;

    if (windowAspectRatio > viewAspectRatio) {
        _view.setSize(viewWidth, viewHeight);
        _view.setViewport({viewportX, 0.0f, viewWidth / newWidth, 1.0f});
    } else {
        _view.setSize(viewWidth, viewHeight);
        _view.setViewport({0.0f, viewportY, 1.0f, viewHeight / newHeight});
    }
    _view.setCenter(viewWidth / 2, viewHeight / 2);
}

}  // namespace RealEngine