#ifndef OBJECT_RENDER_H
#define OBJECT_RENDER_H

#ifndef CUBO_H
#define CUBO_H

#include <glad/glad.h>
#include <glm/glm.hpp>          
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class ObjectRender
{

protected:
	GLuint VAO, VBO, EBO;
	glm::mat4 model;

public:

	std::vector<float> vertices;
	std::vector<GLuint> indices;

	void Draw(GLuint modelLoc);

	ObjectRender& translate(glm::vec3 position);
	ObjectRender& rotate(float angle, glm::vec3 axis);
	ObjectRender& scale(glm::vec3 scaleFactors);
	ObjectRender& resetModel();
	glm::mat4 getModel();
};



#endif

#endif 
