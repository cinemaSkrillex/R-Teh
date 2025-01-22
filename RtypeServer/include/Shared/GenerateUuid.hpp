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
    UUIDGenerator() : _rng(_dev()), _dist(0, 9) {}

    std::string generate() {
        std::string res;
        for (int i = 0; i < 32; i++) {
            res += std::to_string(_dist(_rng));
        }
        return res;
    }
    long int generate_long() {
        long int res = 0;
        for (int i = 0; i < 10; i++) {
            res = res * 10 + _dist(_rng);
        }
        return res;
    }

   private:
    std::random_device                 _dev;
    std::mt19937                       _rng;
    std::uniform_int_distribution<int> _dist;
};

#endif /* !GENERATEUUID_HPP_ */
