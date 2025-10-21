#include "Texture.h"
#include <iostream>


Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;

	int widgthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true); // Inverte a imagem ao carregar
	unsigned char* bytes = stbi_load(image, &widgthImg, &heightImg, &numColCh, 0);



	std::cout << "Width: " << widgthImg
		<< " Height: " << heightImg
		<< " Channels: " << numColCh << std::endl;

	if (!bytes) {
		std::cout << "Falha ao carregar imagem!" << std::endl;
	}


	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// float flatColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// glTexParameterfv(texType, GL_TEXTURE_BORDER_COLOR, flatColor);

	glTexImage2D(texType, 0, GL_RGBA, widgthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(ShaderManage& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}