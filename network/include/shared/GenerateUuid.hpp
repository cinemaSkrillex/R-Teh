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
    UUIDGenerator() : rng(dev()), dist(0, 15) {}

    std::string generate() {
        const char *v      = "0123456789abcdef";
        const bool  dash[] = {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0};

        std::string res;
        for (int i = 0; i < 16; i++) {
            if (dash[i]) res += "-";
            res += v[dist(rng)];
            res += v[dist(rng)];
        }
        return res;
    }

   private:
    std::random_device                 dev;
    std::mt19937                       rng;
    std::uniform_int_distribution<int> dist;
};

#endif /* !GENERATEUUID_HPP_ */
