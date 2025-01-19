/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** LauncherBackground
*/

#include "Launcher/LauncherBackground.hpp"

namespace rtype {
LauncherBackground::LauncherBackground(const std::string& texturePath, float speed)
    : _speed(speed), _offset(0.0f) {
    if (!_texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Unable to load texture: " + texturePath);
    }

    _texture.setRepeated(true);
    _sprite1.setTexture(_texture);
    _sprite1.setTextureRect(sf::IntRect(0, 0, 800, 600));
    _sprite2.setTexture(_texture);
    _sprite2.setTextureRect(sf::IntRect(800, 0, 800, 600));
}

LauncherBackground::~LauncherBackground() {}

void LauncherBackground::update(float deltaTime) {
    _offset += _speed * deltaTime;

    // Assurer que les sprites se déplacent en boucle
    if (_offset >= 800) { // 800 est la largeur de la fenêtre
        _offset = 0;
    }

    // Mettre à jour la position des rectangles de texture pour créer un effet de boucle
    _sprite1.setTextureRect(sf::IntRect(static_cast<int>(_offset), 0, 800, 600));
    _sprite2.setTextureRect(sf::IntRect(static_cast<int>(_offset) + 800, 0, 800, 600));
}

void LauncherBackground::draw(sf::RenderTarget& target) {
    target.draw(_sprite1);
    target.draw(_sprite2);
}
}
