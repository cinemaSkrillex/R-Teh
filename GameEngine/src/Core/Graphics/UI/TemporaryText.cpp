/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** TemporaryText
*/

#include "../include/Core/Graphics/UI/TemporaryText.hpp"

namespace RealEngine {

TemporaryText::TemporaryText(std::string text, sf::Vector2f position, float duration,
                             float disappearTime, std::shared_ptr<sf::Font> font)
    : _elapsedTime(0), _text(text, "") {
    if (font) {
        _text.setFont(*font);
    }
    _duration      = duration;
    _disappearTime = std::clamp(disappearTime, 0.0f, duration);
    _text.setPosition(position.x, position.y);
}

TemporaryText::~TemporaryText() {}

void TemporaryText::update(float deltaTime) {
    if (deltaTime < 0) {
        std::cerr << "Error: deltaTime is negative" << std::endl;
        return;
    }
    _elapsedTime += deltaTime;
    if (_elapsedTime >= _duration) {
        _text.disappear();
        return;
    } else if (_elapsedTime >= _duration - _disappearTime) {
        _text.setOpacity(
            (255 - (255 * (_elapsedTime - (_duration - _disappearTime)) / _disappearTime)));
    }
}

bool TemporaryText::isFinished() const { return _elapsedTime >= _duration; }

}  // namespace RealEngine