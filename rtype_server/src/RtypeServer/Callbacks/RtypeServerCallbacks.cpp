/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::init_callback_mobs(const asio::ip::udp::endpoint& sender) {
    for (const auto& mob : _game_instance->getSimpleMobs()) {
        if (!mob) continue;

        auto* position = _game_instance->getRegistry()->get_component<RealEngine::Position>(mob);
        auto* destructible =
            _game_instance->getRegistry()->get_component<RealEngine::AutoDestructible>(mob);
        auto* velocity  = _game_instance->getRegistry()->get_component<RealEngine::Velocity>(mob);
        auto* rotation  = _game_instance->getRegistry()->get_component<RealEngine::Rotation>(mob);
        auto* collision = _game_instance->getRegistry()->get_component<RealEngine::Collision>(mob);
        auto* drawable  = _game_instance->getRegistry()->get_component<RealEngine::Drawable>(mob);
        auto* container =
            _game_instance->getRegistry()->get_component<RealEngine::NetvarContainer>(mob);
        auto* sprite =
            _game_instance->getRegistry()->get_component<RealEngine::SpriteComponent>(mob);
        auto* spriteSheet =
            _game_instance->getRegistry()->get_component<RealEngine::SpriteSheet>(mob);

        if (!container) {
            std::cerr << "Error: mob does not have a NetvarContainer component" << std::endl;
            continue;
        }
        if (!position || !velocity) {
            std::cerr << "Error: mob does not have a Position or Velocity component" << std::endl;
            continue;
        }

        RTypeProtocol::NewEntityMessage eventMessage;
        eventMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
        eventMessage.uuid         = *mob;

        // Serialize position and velocity component
        addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::POSITION, *position);
        addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::VELOCITY, *velocity);

        // Serialize rotation component
        if (rotation) {
            std::vector<char> rotationData(sizeof(RealEngine::Rotation));
            std::memcpy(rotationData.data(), rotation, sizeof(RealEngine::Rotation));
            eventMessage.components.push_back(
                {RTypeProtocol::ComponentList::ROTATION, rotationData});
        }

        // Serialize collision component
        if (collision) {
            sf::FloatRect             bounds      = collision->bounds;
            std::string               id          = collision->id;
            bool                      isColliding = collision->isColliding;
            RealEngine::CollisionType type        = collision->type;
            addCollisionComponentToMessage(eventMessage, bounds, id, isColliding, type);
        }
        // Serialize auto destructible component
        if (destructible) {
            float autoDestructible = destructible->lifeTime;
            addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE,
                                  autoDestructible);
        }

        // Serialize drawable component
        if (drawable) {
            bool drawable = true;
            addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::DRAWABLE, drawable);
        }

        // Serialize sprite component

        // if (container->getNetvar("sprite_name")) {
        // if (sprite) {

        // auto netvar = container->getNetvar("dropChance");
        // if (netvar && netvar->value.type() == typeid(float)) {
        //     float dropChance = std::any_cast<float>(netvar->value);
        //     std::cout << "DropChance: " << dropChance << std::endl;
        //     // std::vector<char> spriteData(sprite_str.begin(), sprite_str.end());
        //     // addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::SPRITE,
        //     // spriteData);
        // } else {
        //     std::cerr << "Error: dropChance is not of type float or is null" << std::endl;
        // }

        auto netvar = container->getNetvar("sprite_name");
        if (netvar && netvar->value.type() == typeid(std::string)) {
            std::string sprite_str = std::any_cast<std::string>(netvar->value);
            std::cout << "Sprite: " << sprite_str << std::endl;
            std::vector<char> spriteData(sprite_str.begin(), sprite_str.end());
            addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);
        } else {
            std::cerr << "Error: sprite_name is not of type std::string or is null" << std::endl;
        }

        std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(eventMessage);
        _server->send_reliable_packet(serializedEventMessage, sender);
    }
}

void RtypeServer::initCallbacks() {
    auto mapInitializer = std::make_shared<MapInitializer>(_game_instance, _server, _server_config);
    auto playerInitializer = std::make_shared<PlayerInitializer>(this);
    _server->setNewClientCallback(
        [this, mapInitializer, playerInitializer](const asio::ip::udp::endpoint& sender) {
            auto player = playerInitializer->initializePlayer(sender);

            // Send all the entities to the new client, so it can synchronize and move
            mapInitializer->initializeMap(sender);

            init_callback_mobs(sender);

            _players[sender] = player;
        });
}
