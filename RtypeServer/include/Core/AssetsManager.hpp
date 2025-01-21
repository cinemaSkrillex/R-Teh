/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** AssetsManager
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "Core/Audio/Music.hpp"
#include "Core/Audio/Sound.hpp"
#include "Core/Graphics/Rendering/Sprite.hpp"
#include "ECS/Components/SpriteSheet.hpp"

namespace RealEngine {

#define DEBUG 0
#define assetLauncher false
class AssetManager {
   public:
    static AssetManager& getInstance() {
        static AssetManager instance;
        return instance;
    }

    // Textures gestion

    void loadTexture(const std::string& id, const std::string& filePath) {
        try {
            auto texture = std::make_shared<sf::Texture>();
            if (!texture->loadFromFile(filePath)) {
                std::cout << "Failed to load texture: " << filePath << std::endl;
                throw std::runtime_error("Failed to load texture: " + filePath);
            }
            _textures[id] = texture;
        } catch (const std::exception& e) {
            std::cerr << "Failed to loadTexture: " << id << " - " << e.what() << std::endl;
        }
    }

    void loadTexture(const std::string& id, const std::string& filePath, const sf::IntRect& rect) {
        try {
            auto texture = std::make_shared<sf::Texture>();
            if (!texture->loadFromFile(filePath, rect)) {
                std::cout << "Failed to load texture: " << filePath << std::endl;
                throw std::runtime_error("Failed to load texture: " + filePath);
            }
            _textures[id] = texture;
        } catch (const std::exception& e) {
            std::cerr << "Failed to loadTexture: " << id << " - " << e.what() << std::endl;
        }
    }

    void loadSpriteSheet(const std::string&                                  id,
                         std::unordered_map<std::string, RealEngine::Sprite> spriteSheet,
                         const std::string& spriteIndex, int frameIndex,
                         const sf::Vector2i& frameSize, bool pause, bool loop, float animTime,
                         const sf::Vector2i& origin, sf::Clock animClock) {
        auto spriteSheetPtr = std::make_shared<RealEngine::SpriteSheet>(
            spriteSheet, spriteIndex, frameIndex, frameSize, pause, loop, animTime, origin,
            animClock);
        _spriteSheets[id] = spriteSheetPtr;
    }

    std::shared_ptr<SpriteSheet> getSpriteSheet(const std::string& id) {
        try {
            auto it = _spriteSheets.find(id);
            if (it == _spriteSheets.end()) {
                throw std::runtime_error("SpriteSheet not found: " + id);
            }
            return it->second;
        } catch (const std::exception& e) {
            // std::cerr << "Failed to get spriteSheet: " << id << " - " << e.what() << std::endl;
            // std::cerr << "Failed to get spriteSheet: " << id << " - " << e.what() << std::endl;
            return nullptr;
        }
    }

    std::shared_ptr<sf::Texture> getTexture(const std::string& id) {
        try {
            auto it = _textures.find(id);
            if (it == _textures.end()) {
                std::cout << "Texture not found: " << id << std::endl;
                throw std::runtime_error("Texture not found: " + id);
            }
            return it->second;
        } catch (const std::exception& e) {
            std::cerr << "Failed to get texture: " << id << " - " << e.what() << std::endl;
            return nullptr;
        }
    }

    void unloadTexture(const std::string& id) { _textures.erase(id); }

    // Sprites gestion

    void loadSprite(const std::string& id, const std::string& textureId) {
        try {
            std::shared_ptr<sf::Texture> texturePtr = getTexture(textureId);
            auto                         sprite = std::make_shared<RealEngine::Sprite>(texturePtr);
            _sprites[id]                        = sprite;
        } catch (const std::exception& e) {
            std::cerr << "Failed to loadSprite: " << id << " - " << e.what() << std::endl;
        }
    }

    std::shared_ptr<Sprite> getSprite(const std::string& id) {
        try {
            auto it = _sprites.find(id);
            if (it == _sprites.end()) {
                std::cout << "Sprite not found: " << id << std::endl;
                throw std::runtime_error("Sprite not found: " + id);
            }
            return it->second;
        } catch (const std::exception& e) {
            std::cerr << "Failed to get sprite: " << id << " - " << e.what() << std::endl;
            return nullptr;
        }
    }

    void loadSpriteTextureAndScale(const std::string& id, const std::string& filepath,
                                   const sf::Vector2f& scale) {
        try {
            loadTexture(id, filepath);
            loadSprite(id, id);
            getSprite(id)->setScale(scale.x, scale.y);
        } catch (const std::exception& e) {
            std::cerr << "Failed to loadSpriteTextureAndScale: " << id << " - " << e.what()
                      << std::endl;
            return;
        }
    }

    void loadSpriteTextureAndScale(const std::string& id, const std::string& filepath,
                                   const sf::IntRect& rect, const sf::Vector2f& scale) {
        try {
            loadTexture(id, filepath, rect);
            loadSprite(id, id);
            getSprite(id)->setScale(scale.x, scale.y);
        } catch (const std::exception& e) {
            std::cerr << "Failed to loadSpriteTextureAndScale: " << id << " - " << e.what()
                      << std::endl;
            return;
        }
    }

    void unloadSprite(const std::string& id) { _sprites.erase(id); }

    /**
     * @brief Loads textures from a specified folder and scales them.
     *
     * This function iterates through all files in the specified folder, loads PNG textures,
     * and scales them according to the provided scale factor. The textures are identified
     * by a unique ID, which is created by combining the folder name and the file name.
     *
     * @param folderPath The path to the folder containing the textures.
     * @param folderName The name of the folder to be used as a prefix for texture IDs.
     *                   If empty, only the file name is used as the texture ID.
     * @param scale The scale factor to be applied to the textures.
     *
     * @example
     * @code
     * sf::Vector2f scale(1.0f, 1.0f);
     * loadTexturesFromFolder("assets/textures", "textures", scale);
     * @endcode
     */
    void loadTexturesFromFolder(const std::string& folderPath, const std::string& folderName,
                                const sf::Vector2f& scale) {
        try {
            for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    std::string fileName = entry.path().stem().string();

                    if (entry.path().extension() == ".png") {
                        std::string textureId;
                        if (folderName.empty()) {
                            textureId = fileName;
                        } else {
                            textureId = folderName + "_" + fileName;
                        }
                        loadSpriteTextureAndScale(textureId, filePath, scale);
                        if (DEBUG) {
                            std::cout << "Loaded texture: " << textureId << " from " << filePath
                                      << std::endl;
                        }
                    } else {
                        std::cout << "Skipping file: " << filePath << "it's not a PNG" << std::endl;
                        continue;
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to load textures from folder: " << folderPath << " - " << e.what()
                      << std::endl;
        }
    }

    // Fonts gestion

    void loadFont(const std::string& id, const std::string& filePath) {
        try {
            auto font = std::make_shared<sf::Font>();
            if (!font->loadFromFile(filePath)) {
                std::cout << "Failed to load font: " << filePath << std::endl;
                throw std::runtime_error("Failed to load font: " + filePath);
            }
            _fonts[id] = font;
        } catch (const std::exception& e) {
            std::cerr << "Failed to loadFont: " << id << " - " << e.what() << std::endl;
        }
    }

    std::shared_ptr<sf::Font> getFont(const std::string& id) {
        try {
            auto it = _fonts.find(id);
            if (it == _fonts.end()) {
                std::cout << "Font not found: " << id << std::endl;
                throw std::runtime_error("Font not found: " + id);
            }
            return it->second;
        } catch (const std::exception& e) {
            std::cerr << "Failed to get font: " << id << " - " << e.what() << std::endl;
            return nullptr;
        }
    }

    void unloadFont(const std::string& id) { _fonts.erase(id); }

    // Musics gestion

    void loadMusic(const std::string& id, const std::string& filePath) {
        try {
            auto music = std::make_shared<RealEngine::Music>(filePath);
            if (!music) {
                std::cout << "Failed to load music: " << filePath << std::endl;
                throw std::runtime_error("Failed to load music: " + filePath);
            }
            _musics[id] = music;
        } catch (const std::exception& e) {
            std::cerr << "Failed to loadMusic: " << id << " - " << e.what() << std::endl;
        }
    }

    std::shared_ptr<Music> getMusic(const std::string& id) {
        try {
            auto it = _musics.find(id);
            if (it == _musics.end()) {
                std::cout << "Music not found: [" << id << "]" << std::endl;
                throw std::runtime_error("Music not found: " + id);
            }
            return it->second;
        } catch (const std::exception& e) {
            std::cerr << "Failed to get music: " << id << " - " << e.what() << std::endl;
            return nullptr;
        }
    }

    void unloadMusic(const std::string& id) { _musics.erase(id); }

    // Sounds gestion

    void loadSound(const std::string& id, const std::string& filePath) {
        try {
            auto sound = std::make_shared<RealEngine::Sound>();
            if (!sound->loadFile(filePath)) {
                throw std::runtime_error("Failed to load sound: " + filePath);
            }
            _sounds[id] = sound;
        } catch (const std::exception& e) {
            std::cerr << "Failed to loadSound: " << id << " - " << e.what() << std::endl;
        }
    }

    std::shared_ptr<Sound> getSound(const std::string& id) {
        try {
            auto it = _sounds.find(id);
            if (it == _sounds.end()) {
                std::cout << "Sound not found: " << id << std::endl;
                throw std::runtime_error("Sound not found: " + id);
            }
            return it->second;
        } catch (const std::exception& e) {
            std::cerr << "Failed to get sound: " << id << " - " << e.what() << std::endl;
            return nullptr;
        }
    }

    void unloadSound(const std::string& id) { _sounds.erase(id); }

   private:
    AssetManager()  = default;
    ~AssetManager() = default;

    AssetManager& operator=(const AssetManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> _textures;
    std::unordered_map<std::string, std::shared_ptr<Sprite>>      _sprites;
    std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> _spriteSheets;
    std::unordered_map<std::string, std::shared_ptr<sf::Font>>    _fonts;
    std::unordered_map<std::string, std::shared_ptr<Music>>       _musics;
    std::unordered_map<std::string, std::shared_ptr<Sound>>       _sounds;
};
}  // namespace RealEngine