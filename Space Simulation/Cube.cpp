#include "Cube.h"

Cube::Cube() 
{
	model = glm::mat4(1.0f);

	std::vector<float> vertices_ {
		// posX, posY, posZ,   texU, texV

		// Frente
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		// Traseira
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		// Esquerda
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		// Direita
		1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		// Topo
		0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		// Fundo
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f
	};

	std::vector<GLuint> indices_ {
		// Frente
		0, 1, 2,
		2, 3, 0,

		// Traseira
		4, 5, 6,
		6, 7, 4,

		// Esquerda
		8, 9, 10,
		10, 11, 8,

		// Direita
		12, 13, 14,
		14, 15, 12,

		// Topo
		16, 17, 18,
		18, 19, 16,

		// Fundo
		20, 21, 22,
		22, 23, 20
	};

	vertices = vertices_;
	indices = indices_;
}