/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerRun
*/

#include "../../include/RtypeServer/RtypeServer.hpp"
#include "../../include/shared/RtypeServerProtocol.hpp"

void RtypeServer::run() {
    auto  log                   = std::make_shared<Log>("RtypeServer.log");
    int   server_tick           = _server_config.getConfigItem<int>("SERVER_TICK");
    int   server_broadcast_tick = _server_config.getConfigItem<int>("SERVER_BROADCAST_TICK");
    int   server_test_tick      = 5;
    float changeScene           = 5.f;
    bool  changedScene          = false;

    while (true) {
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / server_tick) {
            // Reset the clock for the next tick
            _deltaTime = _clock.restart().asSeconds();

            changeScene -= _deltaTime;
            if (changeScene < 0 && !changedScene) {
                _scene_manager.switchScene(SceneType::GAME);
                updateScene();
                changedScene = true;
            }

            handleClientMessages();
            runGameInstance(_deltaTime);
        }
        if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / server_broadcast_tick) {
            _deltaTimeBroadcast = _broadcastClock.restart().asSeconds();
            broadcastStates();
        }
        if (_gameClock.getElapsedTime().asMilliseconds() > 1000 / server_test_tick) {
            _gameClock.restart();
            auto registry = _game_instance->getRegistry();
            auto entities = registry->view<RealEngine::NetvarContainer>();
            for (auto entity : entities) {
                broadcastEntityState(entity, registry);
            }
        }
    }
}

void RtypeServer::handleClientMessages() {
    for (const auto& client : _server->getClients()) {
        auto& player = _players.at(client);
        if (!player.getEntity()) {
            continue;
        }
        const auto& messages = _server->get_unreliable_messages_from_endpoint(client);

        for (const auto& message : messages) {
            auto baseMessage = RTypeProtocol::deserialize<800>(message);

            if (baseMessage.message_type == RTypeProtocol::PLAYER_DIRECTION) {
                runSimulation(message, client, player);
            } else {
                runEvent(message, client, player);
            }
        }
    }
}

void RtypeServer::initEventHandlers() {
    eventHandlers[RTypeProtocol::MessageType::SHOOT_EVENT] = std::make_unique<ShootEvent>();
    eventHandlers[RTypeProtocol::MessageType::HOLD_SHOOT_EVENT] =
        std::make_unique<HoldShootEvent>();
    eventHandlers[RTypeProtocol::MessageType::RELEASE_SHOOT_EVENT] =
        std::make_unique<ReleaseShootEvent>();
}

void RtypeServer::runGameInstance(float deltaTime) {
    auto registry = _game_instance->getRegistry();
    auto entities = registry->view<RealEngine::NetvarContainer>();
    for (auto entity : entities) {
        sendNewEntity(entity, registry);
    }

    auto destroyedEntities = _game_instance->run(_deltaTime);
    if (!destroyedEntities.empty()) {
        RTypeProtocol::DestroyEntityMessage destroyMessage;
        destroyMessage.message_type = RTypeProtocol::MessageType::DESTROY_ENTITY;
        destroyMessage.uuid         = 0;
        destroyMessage.entity_ids.reserve(destroyedEntities.size());
        destroyMessage.entity_ids.insert(destroyMessage.entity_ids.end(), destroyedEntities.begin(),
                                         destroyedEntities.end());
        std::array<char, 800> serializedDestroyMessage =
            RTypeProtocol::serialize<800>(destroyMessage);
        broadcastAllReliable(serializedDestroyMessage);
    }
}

void RtypeServer::sendNewEntity(RealEngine::Entity entity, RealEngine::Registry* registry) {
    auto* netvarContainer = registry->get_component<RealEngine::NetvarContainer>(entity);
    if (!netvarContainer) return;

    auto newEntity = netvarContainer->netvars.find("new_entity");
    if (newEntity == netvarContainer->netvars.end()) return;

    auto* newEntityValue = std::any_cast<bool>(&newEntity->second.value);
    if (!newEntityValue || !*newEntityValue) return;

    RTypeProtocol::NewEntityMessage newEntityMessage;
    newEntityMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
    newEntityMessage.uuid         = entity;
    newEntityMessage.entity_type  = RTypeProtocol::EntityType::OTHER_ENTITY;

    // Serialize position component
    auto* position = registry->get_component<RealEngine::Position>(entity);
    if (position) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::POSITION,
                              sf::Vector2f(position->x, position->y));
    }

    // Serialize velocity component
    auto* velocity = registry->get_component<RealEngine::Velocity>(entity);
    if (velocity) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::VELOCITY, *velocity);
    }

    // Serialize collision component
    auto* collision = registry->get_component<RealEngine::Collision>(entity);
    if (collision) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::COLLISION,
                              *collision);
    }

    // Serialize auto destructible component
    auto* autoDestructible = registry->get_component<RealEngine::AutoDestructible>(entity);
    if (autoDestructible) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE,
                              *autoDestructible);
    }

    // Serialize drawable component
    auto* drawable = registry->get_component<RealEngine::Drawable>(entity);
    if (drawable) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::DRAWABLE, *drawable);
    }

    // Serialize sprite component
    auto spriteName = netvarContainer->netvars.find("sprite_name");
    if (spriteName != netvarContainer->netvars.end()) {
        auto* spriteNameValue = std::any_cast<std::string>(&spriteName->second.value);
        if (spriteNameValue) {
            std::vector<char> spriteData(spriteNameValue->begin(), spriteNameValue->end());
            addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::SPRITE,
                                  spriteData);
        }
    }

    // Serialize angle component
    auto* rotation = registry->get_component<RealEngine::Rotation>(entity);
    if (rotation) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::ROTATION, *rotation);
    }

    // Serialize acceleration component
    auto* acceleration = registry->get_component<RealEngine::Acceleration>(entity);
    if (acceleration) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::ACCELERATION,
                              *acceleration);
    }
    std::array<char, 800> serializedNewEntityMessage =
        RTypeProtocol::serialize<800>(newEntityMessage);
    broadcastAllReliable(serializedNewEntityMessage);
    netvarContainer->netvars.erase("new_entity");
}

void RtypeServer::broadcastStates() {
    // Broadcast the state of all players
    for (const auto& player : _players) {
        if (!player.second.getEntity()) {
            continue;
        }
        broadcastPlayerState(player.second);
    }
    // Broadcast entity states
    // auto registry = _game_instance->getRegistry();
    // auto entities = registry->view<RealEngine::NetvarContainer>();
    // for (auto entity : entities) {
    //     broadcastEntityState(entity, registry);
    // }
}