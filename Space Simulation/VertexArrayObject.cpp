#include"VertexArrayObject.h"


VertexArrayObject::VertexArrayObject(std::function<void()> context, bool is_config)
{
	// Gera o ID do VAO apenas se is_config for verdadeiro
	if (is_config)
		glGenVertexArrays(1, &ID);

	// Gera o ID do VAO
	Context(context);
}

void VertexArrayObject::Context(std::function<void()> context)
{
	// Ativa o VAO
	Bind();

	// Executa a função de contexto para configurar os atributos do VAO
	if (context) context();

	// Desativa o VAO
	Unbind();
}

void VertexArrayObject::LinkAttrib(GLuint layout, GLint size, GLsizei stride, size_t offset)
{
	// 
	glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	glEnableVertexAttribArray(layout);
}

void VertexArrayObject::Bind()
{
	// Bind do VAO
	glBindVertexArray(ID);
}

void VertexArrayObject::Unbind()
{
	// Unbind do VAO
	glBindVertexArray(0);
}

void VertexArrayObject::Delete()
{
	glDeleteVertexArrays(1, &ID); // Deleta o VAO
}