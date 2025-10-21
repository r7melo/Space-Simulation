#ifndef SHADER_MANAGE
#define SHADER_MANAGE

#include <glad/glad.h> // Inclui o GLAD para carregar as funções do OpenGL
#include <string>     // Inclui a biblioteca padrão de strings
#include <fstream>    // Inclui a biblioteca para manipulação de arquivos
#include <sstream>    // Inclui a biblioteca para manipulação de streams
#include <cerrno>     // Inclui a biblioteca para manipulação de erros

std::string get_file_contents(const char* filename);

class ShaderManage
{
public:
	GLuint ID;
	ShaderManage(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
	void compileErrors(unsigned int shader, const char* type);
};

#endif