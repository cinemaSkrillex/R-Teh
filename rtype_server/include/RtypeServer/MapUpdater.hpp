/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MapUpdater
*/

#ifndef MAPUPDATER_HPP_
#define MAPUPDATER_HPP_

#include "RtypeServer.hpp"

class MapUpdater {
   public:
    MapUpdater(RealEngine::Registry& registry, std::shared_ptr<GameMap> map);

    void update(float deltaTime);

   private:
    void updateBlocks(float deltaTime);
    void updateMapPosition(float deltaTime);

    RealEngine::Registry&    _registry;
    std::shared_ptr<GameMap> _map;
};

#endif  // MAPUPDATER_HPP_