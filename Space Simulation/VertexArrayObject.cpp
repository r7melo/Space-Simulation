#include"VertexArrayObject.h"


VertexArrayObject::VertexArrayObject()
{
}

void VertexArrayObject::Start()
{
	// Cria o Vertex Array Object (VAO) e o torna o objeto de estado ATIVO no contexto OpenGL (GPU)
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
}

void VertexArrayObject::End()
{
	// Define os atributos dos vértices (posição) e habilita o atributo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Define os atributos dos vértices (textura) e habilita o atributo
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Desfaz o bind dos buffers e do VAO (boa prática)
	glBindVertexArray(0);
}

void VertexArrayObject::Bind()
{
	// Bind do VAO
	glBindVertexArray(ID);
}

void VertexArrayObject::Delete()
{
	glDeleteVertexArrays(1, &ID); // Deleta o VAO
}