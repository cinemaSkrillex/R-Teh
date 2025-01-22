/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** DynamicLibrary.hpp
*/

#ifndef DynamicLibrary_HPP
#define DynamicLibrary_HPP

#include <iostream>
#include <stdexcept>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

class DynamicLibrary {
   public:
    DynamicLibrary(const std::string& path) {
#if defined(_WIN32) || defined(_WIN64)
        _handle = LoadLibrary(path.c_str());
        if (!_handle) {
            throw std::runtime_error("Cannot open library: " + path);
        }
#else
        _handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!_handle) {
            throw std::runtime_error(dlerror());
        }
#endif
    }

    ~DynamicLibrary() {
        if (_handle) {
#if defined(_WIN32) || defined(_WIN64)
            FreeLibrary(static_cast<HMODULE>(_handle));
#else
            dlclose(_handle);
#endif
        }
    }

    template <typename T>
    T getSymbol(const std::string& name) {
#if defined(_WIN32) || defined(_WIN64)
        T symbol = reinterpret_cast<T>(GetProcAddress(static_cast<HMODULE>(_handle), name.c_str()));
        if (!symbol) {
            throw std::runtime_error("Cannot load symbol: " + name);
        }
#else
        dlerror();  // Reset errors
        T           symbol      = reinterpret_cast<T>(dlsym(_handle, name.c_str()));
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            throw std::runtime_error(dlsym_error);
        }
#endif
        return symbol;
    }

   private:
    void* _handle;
};

#endif  // DynamicLibrary_HPP