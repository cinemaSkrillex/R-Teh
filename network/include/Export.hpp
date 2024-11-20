/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Export.hpp
*/

#ifndef EXPORT_HPP
#define EXPORT_HPP

#if defined(_WIN32) || defined(_WIN64)
#ifdef SERVER_LIBRARY_EXPORTS
#define SERVER_API __declspec(dllexport)
#else
#define SERVER_API __declspec(dllimport)
#endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#ifdef SERVER_LIBRARY_EXPORTS
#define SERVER_API __attribute__((visibility("default")))
#else
#define SERVER_API
#endif
#else
#define SERVER_API
#endif

#endif // EXPORT_HPP