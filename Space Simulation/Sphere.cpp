#include "Sphere.h"
#include <vector>
#include <iostream>

Sphere::Sphere(float radius, int stacks, int slices)
{
	static std::vector<float> vertices_;
	static std::vector<GLuint> indices_;

	for (int i = 0; i <= stacks; ++i) {
		float phi = glm::pi<float>() * i / stacks;
		for (int j = 0; j <= slices; ++j) {
			float theta = 2.0f * glm::pi<float>() * j / slices;
			float x = radius * sin(phi) * cos(theta);
			float y = radius * cos(phi);
			float z = radius * sin(phi) * sin(theta);

			vertices_.push_back(x);
			vertices_.push_back(y);
			vertices_.push_back(z);

			// Coordenadas de textura
			vertices_.push_back((float)j / slices); // u
			vertices_.push_back((float)i / stacks); // v
		}
	}

	for (int i = 0; i < stacks; ++i) {
		for (int j = 0; j < slices; ++j) {
			int first = i * (slices + 1) + j;
			int second = first + slices + 1;

			indices_.push_back(first);
			indices_.push_back(second);
			indices_.push_back(first + 1);

			indices_.push_back(second);
			indices_.push_back(second + 1);
			indices_.push_back(first + 1);
		}
	}

	vertices = vertices_;
	indices = indices_;

}
