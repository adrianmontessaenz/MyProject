/*// -----------------------------------------------------------------
*  File:		Texture.cpp
*  Brief:		Implementation of Textures
*  Creation:	06/11/2022
*  Last Update:	06/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Texture.hpp"

/// -----------------------------------------------------------------
/// Texture custom constructor
/// -----------------------------------------------------------------
Engine::Texture::Texture(const std::string name, const int type) : mTID(0), mType(type)
{
	//Generate depending on its type
	SetName(name);
	if(mType == 0)
		Generate2DTexture();
}

/// -----------------------------------------------------------------
/// Texture destructor
/// -----------------------------------------------------------------
Engine::Texture::~Texture()
{
	glDeleteTextures(1, &mTID);
}

/// -----------------------------------------------------------------
/// Texture bind
/// -----------------------------------------------------------------
void Engine::Texture::Bind()
{
	//Bind depending on its type
	if (mType == 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTID);
	}
}

/// -----------------------------------------------------------------
/// Gets texture type
/// -----------------------------------------------------------------
const int Engine::Texture::GetType() const
{
	return mType;
}

/// -----------------------------------------------------------------
/// Generates a 2D texture
/// -----------------------------------------------------------------
void Engine::Texture::Generate2DTexture()
{
	//Get texture name, file type and generate it
	std::string name = GetName();
	std::string fileType = name.substr(name.find_last_of('.') + 1, name.size());
	std::string path = "../Data/Textures/" + name;
	glGenTextures(1, &mTID);
	glBindTexture(GL_TEXTURE_2D, mTID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	
	//If successfull, add to opengl
	if (data)
	{
		if(fileType != "png")
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	//Free data
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}
