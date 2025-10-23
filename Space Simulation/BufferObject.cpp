#include "BufferObject.h"

BufferObject::BufferObject(GLenum type) : ID(0), TYPE(type) {}

void BufferObject::Start(const void* data, GLsizeiptr size)
{
	// Cria o Buffer Object (VBO, EBO, etc.) e o torna o objeto de estado ATIVO no contexto OpenGL (GPU)
	glGenBuffers(1, &ID);
	glBindBuffer(TYPE, ID);
	glBufferData(TYPE, size, data, GL_STATIC_DRAW);
}

void BufferObject::End()
{
	// Desfaz o bind do buffer (boa prática)
	glBindBuffer(TYPE, 0);
}

void BufferObject::Delete()
{
	// Deleta o Buffer Object
	glDeleteBuffers(1, &ID);
}