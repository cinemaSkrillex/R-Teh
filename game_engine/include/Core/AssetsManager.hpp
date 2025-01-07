/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** AssetsManager
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "Media/Graphics/Rendering/Sprite.hpp"

namespace RealEngine {
class AssetManager {
   public:
    static AssetManager& getInstance() {
        static AssetManager instance;
        return instance;
    }

    // Textures gestion

    void loadTexture(const std::string& id, const std::string& filePath) {
        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(filePath)) {
            std::cout << "Failed to load texture: " << filePath << std::endl;
            throw std::runtime_error("Failed to load texture: " + filePath);
        }
        _textures[id] = texture;
    }

    void loadTexture(const std::string& id, const std::string& filePath, const sf::IntRect& rect) {
        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(filePath, rect)) {
            std::cout << "Failed to load texture: " << filePath << std::endl;
            throw std::runtime_error("Failed to load texture: " + filePath);
        }
        _textures[id] = texture;
    }

    std::shared_ptr<sf::Texture> getTexture(const std::string& id) {
        auto it = _textures.find(id);
        if (it == _textures.end()) {
            std::cout << "Texture not found: " << id << std::endl;
            throw std::runtime_error("Texture not found: " + id);
        }
        return it->second;
    }

    void unloadTexture(const std::string& id) { _textures.erase(id); }

    // Sprites gestion

    void loadSprite(const std::string& id, const std::string& textureId) {
        std::shared_ptr<sf::Texture> texturePtr = getTexture(textureId);
        auto                         sprite     = std::make_shared<RealEngine::Sprite>(texturePtr);
        _sprites[id]                            = sprite;
    }

    std::shared_ptr<Sprite> getSprite(const std::string& id) {
        auto it = _sprites.find(id);
        if (it == _sprites.end()) {
            std::cout << "Sprite not found: " << id << std::endl;
            throw std::runtime_error("Sprite not found: " + id);
        }
        return it->second;
    }

    void loadSpriteTextureAndScale(const std::string& id, const std::string& filepath,
                                   const sf::Vector2f& scale) {
        loadTexture(id, filepath);
        loadSprite(id, id);
        getSprite(id)->setScale(scale.x, scale.y);
    }

    void loadSpriteTextureAndScale(const std::string& id, const std::string& filepath,
                                   const sf::Vector2f& scale, const sf::IntRect& rect) {
        loadTexture(id, filepath, rect);
        loadSprite(id, id);
        getSprite(id)->setScale(scale.x, scale.y);
    }

    void unloadSprite(const std::string& id) { _sprites.erase(id); }

   private:
    AssetManager()  = default;
    ~AssetManager() = default;

    // AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> _textures;
    std::unordered_map<std::string, std::shared_ptr<Sprite>>      _sprites;
};
}  // namespace RealEngine