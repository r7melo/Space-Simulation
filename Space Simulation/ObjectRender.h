#ifndef OBJECT_RENDER_H
#define OBJECT_RENDER_H

#ifndef CUBO_H
#define CUBO_H

#include <glad/glad.h>
#include <glm/glm.hpp>          
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ObjectRender
{

protected:
	GLint VAO, VBO, EBO;
	float* vertices;
	GLint* indices;
	glm::mat4 model;

public:

	void Draw(GLint modelLoc);

	ObjectRender& translate(glm::vec3 position);
	ObjectRender& rotate(float angle, glm::vec3 axis);
	ObjectRender& scale(glm::vec3 scaleFactors);
	ObjectRender& resetModel();
	glm::mat4 getModel();

	float* getVertices() { return vertices; }
	GLint* getIndices() { return indices; }
	GLsizeiptr getVerticesSize() { return 120 * sizeof(float); }
	GLsizeiptr getIndicesSize() { return 36 * sizeof(GLint); }
};



#endif

#endif 
