#pragma once

#include <memory>

#include "../Render.hpp"

namespace RealEngine {
class Text : public IRender {
   public:
    Text() = default;
    Text(const std::string text, const std::string fontPath);
    Text(const std::string text, sf::Font& font);
    ~Text();
    void        draw(sf::RenderTexture& window);
    void        loadFont(const std::string filePath);
    void        disappear() override;
    void        setPosition(float x, float y) override;
    void        move(float x, float y);
    void        setScale(float x, float y) override;
    void        setRotation(float angle) override;
    void        rotate(float angle);
    void        center();
    void        setFont(sf::Font font);
    void        setOpacity(int opacity);
    void        setColor(float r, float g, float b, float a);
    void        setBorderColor(float r, float g, float b, float a);
    void        setBorderThickness(float thickness);
    void        setString(const std::string text);
    void        setCharacterSize(int size);
    std::string getString() const { return _text.getString(); }
    float getWidth() const { return _text.getGlobalBounds().width + _text.getGlobalBounds().left; }
    float getHeight() const { return _text.getLocalBounds().height + _text.getLocalBounds().top; }
    int   getCharacterSize() const { return _text.getCharacterSize(); }
    sf::FloatRect getLocalBounds() const { return _text.getLocalBounds(); }
    sf::Vector2f  getPosition() const;
    sf::Text&     getText() { return _text; }
    sf::Font      getFont() { return _font; }

   private:
    sf::Text _text;
    sf::Font _font;
};
}  // namespace RealEngine