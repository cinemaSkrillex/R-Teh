#include "../include/Core/Window.hpp"

#include <iostream>

namespace RealEngine {

Window::Window(const std::string title, const sf::Vector2u size)
    : _title(title), _size(size), _style(sf::Style::Default) {
    _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Default);
    _window.setFramerateLimit(60);
}

Window::~Window() { _window.close(); }

void Window::clear() { _window.clear(); }

void Window::display() { _window.display(); }

void Window::update() {
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window.close();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        _window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F11)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Fullscreen);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F10)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F9)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::None);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F8)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Titlebar);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F7)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Close);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F6)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Resize);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F5)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Default);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F4)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title,
                       sf::Style::Fullscreen | sf::Style::Close);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title,
                       sf::Style::Fullscreen | sf::Style::Resize);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title,
                       sf::Style::Fullscreen | sf::Style::Titlebar);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1)) {
        _window.create(sf::VideoMode(_size.x, _size.y), _title,
                       sf::Style::Fullscreen | sf::Style::None);
    }
}

void Window::close() { _window.close(); }

void Window::setTitle(const std::string title) {
    _title = title;
    _window.setTitle(_title);
}

void Window::setSize(const sf::Vector2u size) {
    _size = size;
    _window.setSize(_size);
}

void Window::setStyle(sf::Uint32 style) {
    if (_style == style) return;
    _window.create(sf::VideoMode(_size.x, _size.y), _title, style);
    _style = style;
}

void Window::setView(sf::View& view) { _window.setView(view); }

bool Window::isOpen() { return _window.isOpen(); }

bool Window::isFocused() { return _window.hasFocus(); }

};  // namespace RealEngine