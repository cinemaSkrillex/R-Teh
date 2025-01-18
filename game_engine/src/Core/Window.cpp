#include "../include/Core/Window.hpp"

namespace RealEngine {

Window::Window(const std::string title, const sf::Vector2u size)
    : _title(title),
      _size(size),
      _style(sf::Style::Default),
      _view({VIEW_WIDTH / 2, VIEW_HEIGHT / 2}, {VIEW_WIDTH, VIEW_HEIGHT}) {
    _blackScreen.setFillColor(sf::Color::Black);
    _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Default);
    _window.setFramerateLimit(60);
    _view.resizeWithAspectRatio(_window.getSize().x, _window.getSize().y);
    if (!_renderTexture.create(_size.x, _size.y)) {
        throw std::runtime_error("Impossible de créer la RenderTexture");
    }
    _renderTexture.setSmooth(false);
    _blackScreen.setSize(sf::Vector2f(_window.getSize().x, _window.getSize().y));
    _blackScreen.setFillColor(sf::Color::Black);
}

Window::Window(const std::string title, const sf::Vector2u size, View view)
    : _title(title), _size(size), _style(sf::Style::Default), _view(view) {
    _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Default);
    _window.setFramerateLimit(60);
    _view.resizeWithAspectRatio(_window.getSize().x, _window.getSize().y);
    if (!_renderTexture.create(_size.x, _size.y)) {
        throw std::runtime_error("Impossible de créer la RenderTexture");
    }
    _renderTexture.setSmooth(false);
    _blackScreen.setSize(sf::Vector2f(_window.getSize().x, _window.getSize().y));
    _blackScreen.setFillColor(sf::Color::Black);
}

Window::Window(const std::string title, const sf::Vector2u size, View view,
               const std::string shaderPath)
    : _title(title), _size(size), _style(sf::Style::Default), _view(view) {
    _blackScreen.setFillColor(sf::Color::Black);
    _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Default);
    _window.setFramerateLimit(60);
    _view.resizeWithAspectRatio(_window.getSize().x, _window.getSize().y);
    if (!_renderTexture.create(_size.x, _size.y)) {
        throw std::runtime_error("Impossible de créer la RenderTexture");
    }
    _renderTexture.setSmooth(false);
    loadShader(shaderPath);
    _blackScreen.setSize(sf::Vector2f(_window.getSize().x, _window.getSize().y));
    _blackScreen.setFillColor(sf::Color::Black);
}

Window::~Window() { _window.close(); }

void Window::clear() { _renderTexture.clear(); }

void Window::display() {
    _renderTexture.draw(_blackScreen);
    _renderTexture.display();

    sf::Sprite renderedScene(_renderTexture.getTexture());

    _shader.setUniform("texture", sf::Shader::CurrentTexture);
    _shader.setUniform("saturation", _saturation);
    _shader.setUniform("gamma", _gamma);

    _window.clear();
    _window.draw(renderedScene, &_shader);
    _window.display();
}

void Window::update(float deltaTime) {
    if (_isScreenBlack && _blackScreen.getFillColor().a < 255) {
        _blackTransitionTime -= deltaTime;
        float alpha =
            std::clamp(255 * (1 - _blackTransitionTime / _blackTransitionTimeValue), 0.0f, 255.0f);
        _blackScreen.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
    } else if (!_isScreenBlack && _blackScreen.getFillColor().a > 0) {
        _blackTransitionTime -= deltaTime;
        float alpha =
            std::clamp(255 * (_blackTransitionTime / _blackTransitionTimeValue), 0.0f, 255.0f);
        _blackScreen.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
    }
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)) {
        setGamma(1.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) {
        setGamma(_gamma - 0.1f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)) {
        setGamma(_gamma + 0.1f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) {
        setSaturation(1.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) {
        setSaturation(_saturation - 0.1f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
        setSaturation(_saturation + 0.1f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
        setVueSmooth(true);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) {
        setVueSmooth(false);
    }
    if (_event.type == sf::Event::Resized) {
        _view.resizeWithAspectRatio(_window.getSize().x, _window.getSize().y);
        _blackScreen.setSize(sf::Vector2f(_window.getSize().x, _window.getSize().y));
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

void Window::setBlackTransition(bool on, float time) {
    _isScreenBlack            = on;
    _blackTransitionTime      = std::clamp(time, 0.0f, 100.0f);
    _blackTransitionTimeValue = std::clamp(time, 0.0f, 100.0f);
}

void Window::setView(sf::View& view) { _window.setView(view); }

bool Window::isOpen() { return _window.isOpen(); }

bool Window::isFocused() { return _window.hasFocus(); }

void Window::setSaturation(float saturation) {
    _saturation = std::max(0.0f, std::min(2.0f, saturation));
}

void Window::setGamma(float gamma) { _gamma = std::max(0.1f, std::min(5.0f, gamma)); }

void Window::setVueSmooth(bool smooth) { _renderTexture.setSmooth(smooth); }

void Window::loadShader(const std::string path) {
    if (!_shader.loadFromFile(path, sf::Shader::Fragment)) {
        throw std::runtime_error("Impossible de charger le shader");
    }
}

};  // namespace RealEngine