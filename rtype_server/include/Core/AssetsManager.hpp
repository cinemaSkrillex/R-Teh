/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** AssetsManager
*/

namespace RealEngine {
template <typename Resource>
class AssetsManager {
   public:
    void load(const std::string& id, const std::string& filePath) {
        std::shared_ptr<Resource> resource = std::make_shared<Resource>();
        if (!resource->loadFromFile(filePath)) {
            throw std::runtime_error("Failed to load resource: " + filePath);
        }
        _resources[id] = resource;
    }

    std::shared_ptr<Resource> get(const std::string& id) const {
        auto it = _resources.find(id);
        if (it == _resources.end()) {
            throw std::runtime_error("Resource not found: " + id);
        }
        return it->second;
    }

   private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> _resources;
};
}  // namespace RealEngine
