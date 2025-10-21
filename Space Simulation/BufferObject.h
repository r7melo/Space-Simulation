#include <glad/glad.h>
#ifndef BUFFER_OBJECT_CLASSE_H
#define BUFFER_OBJECT_CLASSE_H

class BufferObject
{
	public:
	GLuint ID;
	GLenum TYPE;
	BufferObject(GLenum type);
	void Start(const void* data, GLsizeiptr size);
	void End();
	void Delete();
};

#endif 

