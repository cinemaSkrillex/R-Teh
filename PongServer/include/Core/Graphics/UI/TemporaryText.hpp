/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** TemporaryText
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "../include/Core/Graphics/UI/Text.hpp"

namespace RealEngine {
class TemporaryText {
   public:
    TemporaryText(std::string text, sf ::Vector2f position, float duration, float disappearTime,
                  std::shared_ptr<sf::Font> font);
    ~TemporaryText();

    Text& getText() { return _text; }
    void  update(float deltaTime);
    void  draw(sf::RenderTexture& window) { _text.draw(window); }
    bool  isFinished() const;

   private:
    Text  _text;
    float _duration;
    float _disappearTime;
    float _elapsedTime;
};
}  // namespace RealEngine