/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ServerPlayer
*/

#ifndef SERVERPLAYER_HPP_
#define SERVERPLAYER_HPP_

class ServerPlayer {
   public:
    ServerPlayer(/* args */);
    ServerPlayer(long int uuid, long int timestamp,
                 std::shared_ptr<RealEngine::Entity> _playerEntity, RealEngine::Registry* registry);
    ~ServerPlayer();

    long int     getUUID() const { return _uuid; }
    sf::Vector2f getPosition() const {
        RealEngine::Position* component = _registry->getComponent<RealEngine::Position>(*_entity);
        if (!component) {
            std::cout << "Error: Player entity does not have a Position component" << std::endl;
            return {0, 0};
        }
        return {component->x, component->y};
    }
    RealEngine::Netvar* getNetvar(const std::string& name) {
        RealEngine::NetvarContainer* container =
            _registry->getComponent<RealEngine::NetvarContainer>(*_entity);
        if (!container) {
            std::cout << "Error: Player entity does not have a NetvarContainer component"
                      << std::endl;
            return nullptr;
        }
        return container->getNetvar(name);
    }
    const std::vector<sf::Vector2f>& getPositions() const { return _positions; }
    void     setLastTimestamp(long int timestamp) { _lastUpdate = timestamp; }
    long int getLastTimestamp() const { return _lastUpdate; }
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }
    std::shared_ptr<RealEngine::Entity> getEntity() const { return _entity; }

   private:
    long int                            _uuid;
    std::vector<sf::Vector2f>           _positions;
    std::shared_ptr<RealEngine::Entity> _entity;
    RealEngine::Registry*               _registry;
    long                                _lastUpdate;
};

#endif /* !SERVERPLAYER_HPP_ */
