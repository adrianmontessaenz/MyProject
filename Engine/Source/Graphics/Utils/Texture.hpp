/*// -----------------------------------------------------------------
*  File:		Texture.hpp
*  Brief:		Header file of Textures
*  Creation:	06/11/2022
*  Last Update:	06/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class Texture : public Identity
	{
	public:
		Texture(const std::string name, const int type);
		~Texture();
		void Bind();

		const int GetType() const;
	private:
		GLuint mTID = 0;
		int mType = 0;

		void Generate2DTexture();
	};
}