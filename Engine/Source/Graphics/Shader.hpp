/*// -----------------------------------------------------------------
*  File:		Shader.hpp
*  Brief:		Header file of shaders
*  Creation:	04/11/2022
*  Last Update:	04/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#pragma once

namespace Engine
{
	class Shader : public Identity
	{
	public:
		Shader(const std::string vtxshader, const std::string fgshader);
		~Shader();
		void Activate();

		void UniformMat4(glm::mat4 mat4_, std::string name, int layout_loc = -1);
		void UniformVec3(glm::vec3 vec3_, std::string name, int layout_loc = -1);

	private:
		GLuint mShaderID = 0;
		const std::string ReadFromFile(const std::string shaderfile);
	};
}