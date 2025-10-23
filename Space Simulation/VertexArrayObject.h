#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include <glad/glad.h> // Inclui o GLAD para carregar as funções do OpenGL
#include <functional>


class VertexArrayObject
{
public:
	GLuint ID;

	VertexArrayObject(std::function<void()> context, bool is_config);

	void Context(std::function<void()> context);

	static void LinkAttrib(GLuint layout, GLint size, GLsizei stride, size_t offset);
	void Bind();
	void Unbind();
	void Delete();
};

#endif

