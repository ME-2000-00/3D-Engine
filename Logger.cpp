// Logger.cpp
#include "Logger.h"
#include <format>

std::vector<Message> Logger::messages;
std::mutex Logger::mutex;

void Logger::Log(LogLevel level, const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex);

    std::string prefix;
    Message msg;

    switch (level) {
    case LogLevel::INFO:    prefix = "[INFO] ";   msg.r = 200; msg.g = 200; msg.b = 200; break;
    case LogLevel::WARNING: prefix = "[WARN] ";   msg.r = 255; msg.g = 200; msg.b = 20;  break;
    case LogLevel::ERROR:   prefix = "[ERROR] ";  msg.r = 255; msg.g = 20;  msg.b = 20;  break;
    }

    std::string fullMessage = prefix + message;
    msg.msg = fullMessage;


    messages.push_back(msg);

    // Print to console
    std::cout << fullMessage << std::endl;
}
