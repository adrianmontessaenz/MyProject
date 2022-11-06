/*// -----------------------------------------------------------------
*  File:		Shader.cpp
*  Brief:		Implementation of shaders
*  Creation:	04/11/2022
*  Last Update:	04/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Shader.hpp"

/// -----------------------------------------------------------------
/// Shader custom constructor
/// -----------------------------------------------------------------
Engine::Shader::Shader(const std::string vtxshader, const std::string fgshader)
{
	SetName(vtxshader.substr(0, vtxshader.find_last_of('.')));
	//Gets shaders from files
	std::string vtxStr = ReadFromFile(vtxshader);
	const char* vtx = vtxStr.c_str();
	std::string fgStr = ReadFromFile(fgshader);
	const char* fg = fgStr.c_str();

	GLint success;
	//Create shaders
	GLuint vtxID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vtxID, 1, &vtx, NULL);
	glCompileShader(vtxID);
	glGetShaderiv(vtxID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(mShaderID, 1024, NULL, infoLog);
		std::cout << infoLog << std::endl;
		abort();
	}

	GLuint fgID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fgID, 1, &fg, NULL);
	glCompileShader(fgID);
	glGetShaderiv(fgID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(mShaderID, 1024, NULL, infoLog);
		std::cout << infoLog << std::endl;
		abort();
	}

	//Create program
	mShaderID = glCreateProgram();
	glAttachShader(mShaderID, vtxID);
	glAttachShader(mShaderID, fgID);
	glLinkProgram(mShaderID);
	glGetProgramiv(mShaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetProgramInfoLog(mShaderID, 1024, NULL, infoLog);
		std::cout << infoLog << std::endl;
		abort();
	}

	//Delete shaders
	glDeleteShader(vtxID);
	glDeleteShader(fgID);
}

/// -----------------------------------------------------------------
/// Shader destructor
/// -----------------------------------------------------------------
Engine::Shader::~Shader()
{
	glDeleteProgram(mShaderID);
}

/// -----------------------------------------------------------------
/// Activates shader program
/// -----------------------------------------------------------------
void Engine::Shader::Activate()
{
	glUseProgram(mShaderID);
}

/// -----------------------------------------------------------------
/// Add matrix4 as uniform to shader
/// -----------------------------------------------------------------
void Engine::Shader::UniformMat4(glm::mat4 mat4_, std::string name, int layout_loc)
{
	int loc = layout_loc;
	if (loc < 0)
		loc = glGetUniformLocation(mShaderID, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4_));
}

/// -----------------------------------------------------------------
/// Add vec3 as uniform to shader
/// -----------------------------------------------------------------
void Engine::Shader::UniformVec3(glm::vec3 vec3_, std::string name, int layout_loc)
{
	int loc = layout_loc;
	if (loc < 0)
		loc = glGetUniformLocation(mShaderID, name.c_str());
	glUniform3fv(loc, 1, glm::value_ptr(vec3_));
}

/// -----------------------------------------------------------------
/// Add vec4 as uniform to shader
/// -----------------------------------------------------------------
void Engine::Shader::UniformVec4(glm::vec4 vec4_, std::string name, int layout_loc)
{
	int loc = layout_loc;
	if (loc < 0)
		loc = glGetUniformLocation(mShaderID, name.c_str());
	glUniform4fv(loc, 1, glm::value_ptr(vec4_));
}

/// -----------------------------------------------------------------
/// Add int as uniform to shader
/// -----------------------------------------------------------------
void Engine::Shader::UniformInt(int int_, std::string name, int layout_loc)
{
	int loc = layout_loc;
	if (loc < 0)
		loc = glGetUniformLocation(mShaderID, name.c_str());
	glUniform1i(loc, int_);
}

/// -----------------------------------------------------------------
/// Reads shader from file (Temporal until resource manager)
/// -----------------------------------------------------------------
const std::string Engine::Shader::ReadFromFile(const std::string shaderfile)
{
	std::ifstream file;
	std::string result;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		std::string file_path = "../Data/Shaders/" + shaderfile;
		file.open(file_path);
		std::stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		result = buffer.str();
	}
	catch (std::ifstream::failure error)
	{
		abort();
	}

	return result;
}
