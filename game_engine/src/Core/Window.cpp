#include "../include/Core/Window.hpp"

namespace RealEngine {

Window::Window(const std::string title, const sf::Vector2u size)
    : _title(title),
      _size(size),
      _style(sf::Style::Default),
      _view({VIEW_WIDTH / 2, VIEW_HEIGHT / 2}, {VIEW_WIDTH, VIEW_HEIGHT}) {
    _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Default);
    _window.setFramerateLimit(60);
    _view.resizeWithAspectRatio(_window.getSize().x, _window.getSize().y);
}

Window::Window(const std::string title, const sf::Vector2u size, View view)
    : _title(title), _size(size), _style(sf::Style::Default), _view(view) {
    _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Default);
    _window.setFramerateLimit(60);
    _view.resizeWithAspectRatio(_window.getSize().x, _window.getSize().y);
}

Window::~Window() { _window.close(); }

void Window::clear() { _window.clear(); }

void Window::display() { _window.display(); }

void Window::update() {
    _window.setView(_view.getView());
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window.close();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        _window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F11)) {
        setStyle(sf::Style::Fullscreen);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F10)) {
        setStyle(sf::Style::Default);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F9)) {
        setStyle(sf::Style::None);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F8)) {
        setStyle(sf::Style::Titlebar);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F7)) {
        setStyle(sf::Style::Close);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F6)) {
        setStyle(sf::Style::Resize);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F5)) {
        _window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F4)) {
        setStyle(sf::Style::Fullscreen | sf::Style::Close);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3)) {
        setStyle(sf::Style::Fullscreen | sf::Style::Resize);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2)) {
        setStyle(sf::Style::Fullscreen | sf::Style::Titlebar);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1)) {
        setStyle(sf::Style::Fullscreen | sf::Style::None);
    }
    if (_event.type == sf::Event::Resized) {
        _view.resizeWithAspectRatio(_window.getSize().x, _window.getSize().y);
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