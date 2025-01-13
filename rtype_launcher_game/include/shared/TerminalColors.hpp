/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** TerminalColors
*/

#ifndef TERMINALCOLORS_HPP_
#define TERMINALCOLORS_HPP_

#include <string>

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD_RED "\033[1m\033[31m"
#define BOLD_GREEN "\033[1m\033[32m"
#define BOLD_YELLOW "\033[1m\033[33m"
#define BOLD_BLUE "\033[1m\033[34m"
#define BOLD_MAGENTA "\033[1m\033[35m"
#define BOLD_CYAN "\033[1m\033[36m"
#define BOLD_WHITE "\033[1m\033[37m"

inline std::string colorText(const std::string& text, const std::string& color) {
    return color + text + RESET;
}

inline std::string boldText(const std::string& text) { return BOLD + text + RESET; }

inline std::string boldColorText(const std::string& text, const std::string& color) {
    return color + BOLD + text + RESET;
}

#endif /* !TERMINALCOLORS_HPP_ */