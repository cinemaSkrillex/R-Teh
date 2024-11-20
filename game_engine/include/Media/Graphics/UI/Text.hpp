#pragma once

#include "../Render.hpp"

namespace engine {
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

  private:
    sf::Text _text;
    sf::Font _font;
};
} // namespace engine