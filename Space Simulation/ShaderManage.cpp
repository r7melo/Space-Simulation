#include "ShaderManage.h"
#include <iostream>

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

ShaderManage::ShaderManage(const char* vertexFile, const char* fragmentFile)
{
	// Lê o código-fonte dos arquivos de shader
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Obtém os ponteiros para os códigos-fonte
	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	// Cria o programa de shader e linka os shaders
	ID = glCreateProgram();

	// Cria e compila os shaders, e os anexa ao programa de shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Cria o vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Define o código-fonte do vertex shader
	glCompileShader(vertexShader); // Compila o vertex shader
	compileErrors(vertexShader, "VERTEX"); // Verifica erros de compilação do vertex shader

	glAttachShader(ID, vertexShader); // Anexa o vertex shader
	glDeleteShader(vertexShader); // Deleta o vertex shader, pois já está linkado no programa

	// Cria e compila o fragment shader, e o anexa ao programa de shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Cria o fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Define o código-fonte do fragment shader
	glCompileShader(fragmentShader); // Compila o fragment shader
	compileErrors(fragmentShader, "FRAGMENT"); // Verifica erros de compilação do fragment shader

	glAttachShader(ID, fragmentShader); // Anexa o fragment shader
	glDeleteShader(fragmentShader); // Deleta o fragment shader, pois já está linkado no programa

	glLinkProgram(ID); // Linka o programa de shader	
	compileErrors(ID, "PROGRAM"); // Verifica erros de linkagem do programa
}

void ShaderManage::Activate()
{
	// Usa o programa de shader
	glUseProgram(ID);
}

void ShaderManage::Delete()
{
	// Deleta o programa de shader
	glDeleteProgram(ID);
}

void ShaderManage::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}