/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeServerUtils
*/

#ifndef RTYPESERVERUTILS_HPP_
#define RTYPESERVERUTILS_HPP_

#include "RtypeServerProtocol.hpp"

RTypeProtocol::NewEntityMessage addDrawableComponent(RTypeProtocol::NewEntityMessage& message);
RTypeProtocol::NewEntityMessage addAutoDestructibleComponent(
    RTypeProtocol::NewEntityMessage& message, float autoDestructibleTime);

#endif /* !RTYPESERVERUTILS_HPP_ */
