#include "Sphere.h"
#include <vector>

Sphere::Sphere(int slices, int stacks, float radius)
{
	model = glm::mat4(1.0f);

	std::vector<float> verticesTemp;
	std::vector<GLint> indicesTemp;


	for (int i = 0; i <= stacks; ++i) {
		float phi = glm::pi<float>() * i / stacks;
		for (int j = 0; j <= slices; ++j) {
			float theta = 2.0f * glm::pi<float>() * j / slices;
			float x = radius * sin(phi) * cos(theta);
			float y = radius * cos(phi);
			float z = radius * sin(phi) * sin(theta);

			verticesTemp.push_back(x);
			verticesTemp.push_back(y);
			verticesTemp.push_back(z);

			// Coordenadas de textura
			verticesTemp.push_back((float)j / slices); // u
			verticesTemp.push_back((float)i / stacks); // v
		}
	}

	for (int i = 0; i < stacks; ++i) {
		for (int j = 0; j < slices; ++j) {
			int first = i * (slices + 1) + j;
			int second = first + slices + 1;

			indicesTemp.push_back(first);
			indicesTemp.push_back(second);
			indicesTemp.push_back(first + 1);

			indicesTemp.push_back(second);
			indicesTemp.push_back(second + 1);
			indicesTemp.push_back(first + 1);
		}
	}
	
	vertices = new float[verticesTemp.size()];
	indices = new GLint[indicesTemp.size()];

	std::copy(verticesTemp.begin(), verticesTemp.end(), vertices);
	std::copy(indicesTemp.begin(), indicesTemp.end(), indices);

}
