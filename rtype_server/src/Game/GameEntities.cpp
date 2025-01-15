/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameEntities
*/

#include "../include/Game/GameInstance.hpp"

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetPlayer(sf::Vector2f position) {
    rtype::PlayerEntity player(_registry, position);
    auto                playerEntity = player.getEntity();
    _players.emplace(*playerEntity, playerEntity);
    return _players.at(*playerEntity);
}

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetEntity(
    std::shared_ptr<RealEngine::Entity> entity) {
    _enemies.push_back(entity);
    return _enemies.back();
}

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetBullet(sf::Vector2f position,
                                                                  float        speed,
                                                                  std::string  spriteName,
                                                                  float damage, int health,
                                                                  size_t playerID) {
    rtype::Bullet bullet(_registry, position, speed, spriteName, damage, health, playerID);
    _bullets.push_back(bullet.getEntity());
    return bullet.getEntity();
}

void GameInstance::spawnMob(const std::string& mobName, const sf::Vector2f& position, float angle) {
    float level_speed = _game_map->getScrollingSpeed();
    if (mobName == "directional_canon") {
        rtype::DirectionalCanon directionalCanon(_registry, position, level_speed);
        addAndGetEntity(directionalCanon.getEntity());
    } else if (mobName == "eye_bigion") {
        rtype::EyeBigion eyeBigion(_registry, position);
        addAndGetEntity(eyeBigion.getEntity());
    } else if (mobName == "eye_boss") {
        rtype::EyeBoss eyeBoss(_registry, position);
        addAndGetEntity(eyeBoss.getEntity());
    } else if (mobName == "eye_bomber") {
        rtype::EyeBomber eyeBomber(_registry, position);
        addAndGetEntity(eyeBomber.getEntity());
    } else if (mobName == "eye_minion") {
        rtype::EyeMinion eyeMinion(_registry, position);
        addAndGetEntity(eyeMinion.getEntity());
    } else if (mobName == "mob_spawner_ship") {
        rtype::MobSpawnerShip mobSpawnerShip(_registry, position);
        addAndGetEntity(mobSpawnerShip.getEntity());
    } else if (mobName == "mortar") {
        rtype::Mortar mortar(_registry, position);
        addAndGetEntity(mortar.getEntity());
    } else if (mobName == "robot_boss_minion") {
        rtype::RobotBossMinion robotBossMinion(_registry, position);
        addAndGetEntity(robotBossMinion.getEntity());
    } else if (mobName == "robot_mini_boss") {
        rtype::RobotMiniBoss robotMiniBoss(_registry, position);
        addAndGetEntity(robotMiniBoss.getEntity());
    } else if (mobName == "space_drill") {
        rtype::SpaceDrill spaceDrill(_registry, position);
        addAndGetEntity(spaceDrill.getEntity());
    } else if (mobName == "space_plane") {
        rtype::SpacePlane spacePlane(_registry, position);
        addAndGetEntity(spacePlane.getEntity());
    } else if (mobName == "space_sphere") {
        rtype::SpaceSphere spaceSphere(_registry, position);
        addAndGetEntity(spaceSphere.getEntity());
    } else if (mobName == "wall_turret") {
        rtype::WallTurret turretCanon(_registry, position, 0, level_speed);
        addAndGetEntity(turretCanon.getCannonEntity());
        addAndGetEntity(turretCanon.getPedestalEntity());
    }
}