#include "ObjectRender.h"

void ObjectRender::Draw(GLint modelLoc)
{
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(getModel()));
	glDrawElements(GL_TRIANGLES, getIndicesSize() / sizeof(int), GL_UNSIGNED_INT, 0);
}

ObjectRender& ObjectRender::translate(glm::vec3 position)
{
	model = glm::translate(model, position);
	return *this;
}

ObjectRender& ObjectRender::rotate(float angle, glm::vec3 axis)
{
	model = glm::rotate(model, glm::radians(angle), axis);
	return *this;
}

ObjectRender& ObjectRender::scale(glm::vec3 scaleFactors)
{
	model = glm::scale(model, scaleFactors);
	return *this;
}

glm::mat4 ObjectRender::getModel()
{
	return model;
}

ObjectRender& ObjectRender::resetModel()
{
	model = glm::mat4(1.0f);
	return *this;
}

ObjectRender::~ObjectRender()
{
	delete[] vertices;
	delete[] indices;
}