/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** DynamicLibrary.hpp
*/

#ifndef DYNAMICLIBRARY_HPP
#define DYNAMICLIBRARY_HPP

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
        handle_ = LoadLibrary(path.c_str());
        if (!handle_) {
            throw std::runtime_error("Cannot open library: " + path);
        }
#else
        handle_ = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle_) {
            throw std::runtime_error(dlerror());
        }
#endif
    }

    ~DynamicLibrary() {
        if (handle_) {
#if defined(_WIN32) || defined(_WIN64)
            FreeLibrary(static_cast<HMODULE>(handle_));
#else
            dlclose(handle_);
#endif
        }
    }

    template <typename T> T get_symbol(const std::string& name) {
#if defined(_WIN32) || defined(_WIN64)
        T symbol = reinterpret_cast<T>(GetProcAddress(static_cast<HMODULE>(handle_), name.c_str()));
        if (!symbol) {
            throw std::runtime_error("Cannot load symbol: " + name);
        }
#else
        dlerror(); // Reset errors
        T           symbol      = reinterpret_cast<T>(dlsym(handle_, name.c_str()));
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            throw std::runtime_error(dlsym_error);
        }
#endif
        return symbol;
    }

  private:
    void* handle_;
};

#endif // DYNAMICLIBRARY_HPP