#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Core/View.hpp"

namespace RealEngine {
class Window {
   public:
    Window(const std::string title, const sf::Vector2u size);
    Window(const std::string title, const sf::Vector2u size, RealEngine::View view);
    Window(const std::string title, const sf::Vector2u size, RealEngine::View view, const std::string shaderPath);
    ~Window();

    void               clear();
    void               display();
    void               update();
    void               close();
    void               setTitle(const std::string title);
    void               setSize(const sf::Vector2u size);
    void               setStyle(sf::Uint32 style);
    void               setView(sf::View& view);
    bool               isOpen();
    bool               isFocused();
    void               setSaturation(float saturation);
    void               setGamma(float gamma);
    void               loadShader(const std::string shaderPath);
    void               setVueSmooth(bool smooth);
    sf::RenderWindow&  getRenderWindow() { return _window; }
    sf::RenderTexture& getRenderTexture() { return _renderTexture; }
    View&              getView() { return _view; }

   private:
    sf::RenderWindow  _window;
    sf::RenderTexture _renderTexture;
    View              _view;
    sf::Shader        _shader;
    sf::Event         _event;
    sf::Vector2u      _size;
    std::string       _title;
    sf::Uint32        _style;
    float             _saturation = 1.0f;
    float             _gamma      = 1.0f;
};
}  // namespace RealEngine