#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include <string>

enum class LogLevel {
	INFO,
	WARNING,
	ERROR
};

struct Message {
	std::string msg;
	int r, g, b;
};


class Logger {
public:
	static void Log(LogLevel level, const std::string& msg);
	static const std::vector<Message>& GetMessages() { return messages; }

private:
	static std::vector<Message> messages;
	static std::mutex mutex;
};