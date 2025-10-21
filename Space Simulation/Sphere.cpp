#include "Sphere.h"
#include <vector>

Sphere::Sphere(int slices, int stacks, float radius)
{
	model = glm::mat4(1.0f);

	static std::vector<float> vertices;
	static std::vector<GLuint> indices;

	// Gerar apenas uma vez
	if (vertices.empty()) {
		for (int i = 0; i <= stacks; ++i) {
			float phi = glm::pi<float>() * i / stacks;
			for (int j = 0; j <= slices; ++j) {
				float theta = 2.0f * glm::pi<float>() * j / slices;
				float x = radius * sin(phi) * cos(theta);
				float y = radius * cos(phi);
				float z = radius * sin(phi) * sin(theta);

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				// Coordenadas de textura
				vertices.push_back((float)j / slices); // u
				vertices.push_back((float)i / stacks); // v
			}
		}

		for (int i = 0; i < stacks; ++i) {
			for (int j = 0; j < slices; ++j) {
				int first = i * (slices + 1) + j;
				int second = first + slices + 1;

				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);

				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}

		//glGenVertexArrays(1, &VAO);
		//glBindVertexArray(VAO);

		//glGenBuffers(1, &VBO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		//glGenBuffers(1, &EBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		//// Posição
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);
		//// Textura
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);

		//glBindVertexArray(0);
	}

	//// Model matrix
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, 0.0f, glm::vec3(0.5f, 1.0f, 0.0f));

	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//// Draw
	//glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}
