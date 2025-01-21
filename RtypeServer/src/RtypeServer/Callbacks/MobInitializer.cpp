/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MobInitializer
*/

#include "MobInitializer.hpp"

void MobInitializer::initializeMobs(const asio::ip::udp::endpoint& sender) {
    for (const auto& mob : _gameInstance->getSimpleMobs()) {
        if (!mob) continue;
        auto  registry     = _gameInstance->getRegistry();
        auto* position     = registry->getComponent<RealEngine::Position>(mob);
        auto* destructible = registry->getComponent<RealEngine::AutoDestructible>(mob);
        auto* velocity     = registry->getComponent<RealEngine::Velocity>(mob);
        auto* rotation     = registry->getComponent<RealEngine::Rotation>(mob);
        auto* collision    = registry->getComponent<RealEngine::Collision>(mob);
        auto* drawable     = registry->getComponent<RealEngine::Drawable>(mob);
        auto* container    = registry->getComponent<RealEngine::NetvarContainer>(mob);
        auto* sprite       = registry->getComponent<RealEngine::SpriteComponent>(mob);
        auto* spriteSheet  = registry->getComponent<RealEngine::SpriteSheet>(mob);

        if (!container) {
            std::cerr << "Error: mob does not have a NetvarContainer component" << std::endl;
            continue;
        }
        if (!position || !velocity) {
            std::cerr << "Error: mob does not have a Position or Velocity component" << std::endl;
            continue;
        }

        RTypeProtocol::NewEntityMessage eventMessage;
        eventMessage.messageType = RTypeProtocol::MessageType::NEW_ENTITY;
        eventMessage.uuid        = *mob;

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

        auto netvar = container->getNetvar("sprite_name");
        if (netvar && netvar->value.type() == typeid(std::string)) {
            std::string sprite_str = std::any_cast<std::string>(netvar->value);
            // std::cout << "Sprite: " << sprite_str << std::endl;
            std::vector<char> spriteData(sprite_str.begin(), sprite_str.end());
            addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);
        } else {
            std::cerr << "Error: sprite_name is not of type std::string or is null" << std::endl;
        }

        std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(eventMessage);
        _UdpServer->sendReliablePacket(serializedEventMessage, sender);
    }
}