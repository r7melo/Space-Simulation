#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include <glad/glad.h> // Inclui o GLAD para carregar as funções do OpenGL

class VertexArrayObject
{
public:
	GLuint ID;
	VertexArrayObject();

	void Start();
	void End();
	void Bind();
	void Unbind();
	void Delete();
};

#endif