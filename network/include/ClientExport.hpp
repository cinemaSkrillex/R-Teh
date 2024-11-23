/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ClientExport.hpp
*/

// CLIENT_API is a macro for the visibility of the class UDPClient,
// its for the shared library

#ifndef CLIENT_EXPORT_HPP
#define CLIENT_EXPORT_HPP

#if defined(_WIN32) || defined(_WIN64)
#ifdef CLIENT_LIBRARY_EXPORTS
#define CLIENT_API __declspec(dllexport)
#else
#define CLIENT_API __declspec(dllimport)
#endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#ifdef CLIENT_LIBRARY_EXPORTS
#define CLIENT_API __attribute__((visibility("default")))
#else
#define CLIENT_API
#endif
#else
#define CLIENT_API
#endif

#endif // CLIENT_EXPORT_HPP