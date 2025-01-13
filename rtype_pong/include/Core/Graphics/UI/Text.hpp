#pragma once

#include "../Render.hpp"

namespace RealEngine {
class Text : public IRender {
   public:
    Text(const std::string text, const std::string fontPath);
    ~Text();
    void draw(sf::RenderWindow& window);
    void loadFile(const std::string filePath);
    void disappear() override;
    void setPosition(float x, float y) override;
    void move(float x, float y);
    void setScale(float x, float y) override;
    void setRotation(float angle) override;
    void rotate(float angle);
    void center();
    void setColor(float r, float g, float b, float a);
    void setBorderColor(float r, float g, float b, float a);
    void setBorderThickness(float thickness);
    void setString(const std::string text);
    void setCharacterSize(int size);
    std::string getString() const { return _text.getString(); }
    float getWidth() const { return _text.getGlobalBounds().width; }
    float getHeight() const { return _text.getLocalBounds().height + _text.getLocalBounds().top; }
    int getCharacterSize() const { return _text.getCharacterSize(); }
    sf::FloatRect getLocalBounds() const { return _text.getLocalBounds(); }
    sf::Vector2f getPosition() const;

   private:
    sf::Text _text;
    sf::Font _font;
};
}  // namespace RealEngine