/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerRun
*/

#include "../../include/RtypeServer/RtypeServer.hpp"
#include "../../include/Shared/RtypeServerProtocol.hpp"
#include "GameScene.hpp"
#include "WaitingRoomScene.hpp"

void RtypeServer::run() {
    auto log                   = std::make_shared<Log>("RtypeServer.log");
    int  server_tick           = _serverConfig.getConfigItem<int>("SERVER_TICK");
    int  server_broadcast_tick = _serverConfig.getConfigItem<int>("SERVER_BROADCAST_TICK");

    while (true) {
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / server_tick) {
            // Reset the clock for the next tick
            _deltaTime = _clock.restart().asSeconds();
            if (_sceneManager.getCurrentSceneType() == RealEngine::SceneType::WAITING) {
                auto waitingRoomScene =
                    std::dynamic_pointer_cast<WaitingRoomScene>(_sceneManager.getCurrentScene());
                if (waitingRoomScene) {
                    waitingRoomScene->update(_deltaTime);
                }
            } else if (_sceneManager.getCurrentSceneType() == RealEngine::SceneType::GAME) {
                auto gameScene =
                    std::dynamic_pointer_cast<GameScene>(_sceneManager.getCurrentScene());
                if (gameScene) {
                    gameScene->update(_deltaTime);
                }
            }

            handleClientMessages();
            runGameInstance(_deltaTime);
        }
        if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / server_broadcast_tick) {
            _deltaTimeBroadcast = _broadcastClock.restart().asSeconds();
            broadcastStates();
        }
    }
}

void RtypeServer::handleClientMessages() {
    for (const auto& client : _server->getClients()) {
        auto& player = _players.at(client);
        if (!player.getEntity()) {
            continue;
        }
        const auto& messages         = _server->getUnreliableMessagesFromEndpoint(client);
        const auto& reliableMessages = _server->getReliableMessagesFromEndpoint(client);

        for (const auto& message : reliableMessages) {
            auto baseMessage = RTypeProtocol::deserialize<800>(message);
            if (baseMessage.messageType == RTypeProtocol::MAP_UNLOADED) {
                handleMapUnloadedMessage(client);
            }
        }
        for (const auto& message : messages) {
            auto baseMessage = RTypeProtocol::deserialize<800>(message);

            if (baseMessage.messageType == RTypeProtocol::PLAYER_DIRECTION) {
                runSimulation(message, client, player);
            } else {
                runEvent(message, client, player);
            }
        }
    }
}

void RtypeServer::handleMapUnloadedMessage(const asio::ip::udp::endpoint& client) {
    _clientsUnloadedMap.insert(client);
}

bool RtypeServer::allClientsUnloadedMap() const {
    std::cout << "Clients unloaded map size: " << _clientsUnloadedMap.size() << std::endl;
    std::cout << "Server clients size: " << _server->getClients().size() << std::endl;
    return _clientsUnloadedMap.size() == _server->getClients().size();
}

void RtypeServer::initEventHandlers() {
    _eventHandlers[RTypeProtocol::MessageType::SHOOT_EVENT] = std::make_unique<ShootEvent>();
    _eventHandlers[RTypeProtocol::MessageType::HOLD_SHOOT_EVENT] =
        std::make_unique<HoldShootEvent>();
    _eventHandlers[RTypeProtocol::MessageType::RELEASE_SHOOT_EVENT] =
        std::make_unique<ReleaseShootEvent>();
}

void RtypeServer::runGameInstance(float deltaTime) {
    auto registry = _gameInstance->getRegistry();
    auto entities = registry->view<RealEngine::NetvarContainer>();
    for (auto entity : entities) {
        sendNewEntity(entity, registry);
    }

    auto destroyedEntities = _gameInstance->run(_deltaTime);
    if (!destroyedEntities.empty()) {
        RTypeProtocol::DestroyEntityMessage destroyMessage;
        destroyMessage.messageType = RTypeProtocol::MessageType::DESTROY_ENTITY;
        destroyMessage.uuid        = 0;
        destroyMessage.entity_ids.reserve(destroyedEntities.size());
        destroyMessage.entity_ids.insert(destroyMessage.entity_ids.end(), destroyedEntities.begin(),
                                         destroyedEntities.end());
        std::array<char, 800> serializedDestroyMessage =
            RTypeProtocol::serialize<800>(destroyMessage);
        broadcastAllReliable(serializedDestroyMessage);
    }
    for (auto entity : destroyedEntities) {
    }
}

void RtypeServer::sendNewEntity(RealEngine::Entity entity, RealEngine::Registry* registry) {
    auto* netvarContainer = registry->getComponent<RealEngine::NetvarContainer>(entity);
    if (!netvarContainer) return;

    auto newEntity = netvarContainer->netvars.find("new_entity");
    if (newEntity == netvarContainer->netvars.end()) return;

    auto* newEntityValue = std::any_cast<bool>(&newEntity->second.value);
    if (!newEntityValue || !*newEntityValue) return;

    RTypeProtocol::EntityType entityType    = RTypeProtocol::EntityType::OTHER_ENTITY;
    auto*                     collisionType = registry->getComponent<RealEngine::Collision>(entity);
    if (collisionType) {
        if (collisionType->type == RealEngine::CollisionType::SOLID ||
            collisionType->type == RealEngine::CollisionType::INACTIVE) {
            entityType = RTypeProtocol::EntityType::BLOCK;
            std::cout << "Block" << std::endl;
        } else {
            entityType = RTypeProtocol::EntityType::OTHER_ENTITY;
        }
    }

    RTypeProtocol::NewEntityMessage newEntityMessage;
    newEntityMessage.messageType = RTypeProtocol::MessageType::NEW_ENTITY;
    newEntityMessage.uuid        = entity;
    newEntityMessage.entity_type = entityType;
    std::cout << "Entity type: " << static_cast<int>(entityType) << std::endl;

    // Serialize position component
    auto* position = registry->getComponent<RealEngine::Position>(entity);
    if (position) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::POSITION,
                              sf::Vector2f(position->x, position->y));
    }

    // Serialize interpolation component
    auto* interpolation = registry->getComponent<RealEngine::Interpolation>(entity);
    if (interpolation) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::INTERPOLATION,
                              *interpolation);
    }

    // Serialize velocity component
    auto* velocity = registry->getComponent<RealEngine::Velocity>(entity);
    if (velocity) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::VELOCITY, *velocity);
    }

    // Serialize collision component
    auto* collision = registry->getComponent<RealEngine::Collision>(entity);
    if (collision) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::COLLISION,
                              *collision);
    }

    // Serialize auto destructible component
    auto* autoDestructible = registry->getComponent<RealEngine::AutoDestructible>(entity);
    if (autoDestructible) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE,
                              *autoDestructible);
    }

    // Serialize drawable component
    auto* drawable = registry->getComponent<RealEngine::Drawable>(entity);
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
    auto* rotation = registry->getComponent<RealEngine::Rotation>(entity);
    if (rotation) {
        addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::ROTATION, *rotation);
    }

    // Serialize acceleration component
    auto* acceleration = registry->getComponent<RealEngine::Acceleration>(entity);
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
    long int packet_sent = 0;
    for (const auto& player : _players) {
        if (!player.second.getEntity()) {
            continue;
        }
        broadcastPlayerState(player.second);
        packet_sent++;
    }
    // Broadcast entity states
    auto registry = _gameInstance->getRegistry();
    auto entities = registry->view<RealEngine::NetvarContainer>();
    for (auto entity : entities) {
        broadcastEntityState(entity, registry);
        packet_sent++;
    }
    // std::cout << "Sent " << packet_sent << " packets" << std::endl;
}