/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GenerateUuid
*/

#ifndef GENERATEUUID_HPP_
#define GENERATEUUID_HPP_

#include <random>
#include <string>

class UUIDGenerator {
   public:
    UUIDGenerator() : rng(dev()), dist(0, 9) {}

    std::string generate() {
        std::string res;
        for (int i = 0; i < 32; i++) {
            res += std::to_string(dist(rng));
        }
        return res;
    }

   private:
    std::random_device                 dev;
    std::mt19937                       rng;
    std::uniform_int_distribution<int> dist;
};

#endif /* !GENERATEUUID_HPP_ */
