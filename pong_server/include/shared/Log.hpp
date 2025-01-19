/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Log
*/

#ifndef LOG_HPP_
#define LOG_HPP_

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

class Log {
   public:
    Log(const std::string& filename) {
        std::ofstream create_file(filename, std::ios::out);
        create_file.close();

        log_file.open(filename, std::ios::app);
        if (!log_file.is_open()) {
            throw std::runtime_error("Unable to open log file: " + filename);
        }
    }

    ~Log() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex);
        log_file << message << std::endl;
    }

    void log_with_timestamp(const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex);
        log_file << "[" << current_time() << "] " << message << std::endl;
    }

   private:
    std::ofstream log_file;
    std::mutex    log_mutex;

    std::string current_time() {
        std::time_t now = std::time(nullptr);
        char        buf[100];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return buf;
    }
};

#endif /* !LOG_HPP_ */
