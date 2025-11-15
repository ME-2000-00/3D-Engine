#include <iostream>
#include <fstream> 
#include <sstream> 
#include <string> 

#include <GL/glew.h>

#include "shader.h"

#include "Logger.h"

static std::string ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss;
	while (getline(stream, line))
	{
		ss << line << '\n';
	}

	return ss.str();
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	// make sure source is defined before calling this function
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// ERROR HANDELING
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		Logger::Log(LogLevel::ERROR, std::string("failed to compile ") + (type == GL_VERTEX_SHADER ? "-VERTEX-" : "-FRAGMENT-") + " shader");
		std::cout << message << std::endl;
		glDeleteShader(id);
		return -1;

	}

	return id;
}


unsigned int CreateShader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, ParseShader(vertex_shader_path));
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, ParseShader(fragment_shader_path));

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}