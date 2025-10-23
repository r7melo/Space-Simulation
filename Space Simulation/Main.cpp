// Bibliote de entrada e sa�da
#include <iostream>

// Biblioteca GLAD para carregar os ponteiros das fun��es do OpenGL
#include <glad/glad.h>

// Biblioteca GLFW para cria��o de janelas e contexto OpenGL
#include <GLFW/glfw3.h>

// Biblioteca GLM para matem�tica (vetores e matrizes)
#include <glm/glm.hpp>

// Biblioteca personalizada para gerenciamento de shaders
#include "ShaderManage.h"
#include "VertexArrayObject.h"
#include "BufferObject.h"
#include "Texture.h"
#include "Tools.h"

// Bibliotecas GLM adicionais para transforma��es
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"
#include <vector>
#include "Sphere.h"


// ===================================================
// Defini��o de Constantes Globais
// ===================================================
const char* TITLE = "Space Simulation";
unsigned const int WIDTH = 800;
unsigned const int HEIGHT = 800;

glm::mat4 PROJECTION = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
// ===================================================



// ===================================================
// Defini��o das Fun��es de Callback (GLOBALMENTE)
// ===================================================

// Fun��o de Callback para eventos de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	std::cout << "Evento de Tecla: " << key << std::endl;
}

// Fun��o de Callback para redimensionamento da janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Esta fun��o � importante para o OpenGL.
	// Ela ajusta a �rea de renderiza��o (viewport) quando a janela � redimensionada.

	glViewport(0, 0, width, height);

	// Atualiza a proje��o (mantendo propor��o)
	float aspect = (float)width / (float)height;
	PROJECTION = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

// Fun��o de Callback para movimento do mouse
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// A fun��o � chamada toda vez que o mouse se move.
	// Usamos std::cout para imprimir as coordenadas (xpos e ypos).
	std::cout << "Mouse Movido! X: " << xpos << ", Y: " << ypos << std::endl;
}

// ===================================================

// FUNCAO PRINCIPAL
int main()
{

	// Initialize GLFW
    glfwInit();

	// Configure GLFW para usar OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Vers�o maior do OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Vers�o menor do OpenGL

	// Define o perfil do OpenGL, que � o n�cleo (core profile)
	// remove todas as funcionalidades antigas do (legacy) e obriga 
	// o uso de Shaders (Programas de GPU) para renderiza��o.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Cria a janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

	// Verifica se a janela foi criada com sucesso
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Define tamanho m�nimo da janela
	glfwSetWindowSizeLimits(window, 500, 500, GLFW_DONT_CARE, GLFW_DONT_CARE);

	// Registra a fun��o de teclado
	glfwSetKeyCallback(window, key_callback);

	// Registra a fun��o de redimensionamento
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Registra a fun��o de movimento do mouse
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Define o contexto atual para a janela criada
	glfwMakeContextCurrent(window);


	// ======================================================================== OPENGL INITIALIZE ========================================================================

	// Carrega todos os ponteiros das fun��es do OpenGL usando GLAD
	gladLoadGL();

	// Define a viewport (�rea de renderiza��o) com as dimens�es da janela
	glViewport(0, 0, WIDTH, HEIGHT);

	ShaderManage shaderProgram("default.vert", "default.frag");



	#pragma region Configuracao e geometria
	// Cria os buffers e o array de v�rtices

	// Configura��o do Cubo
	Sphere sphere(1.0f, 20, 20);
	BufferObject sphereVBO(GL_ARRAY_BUFFER);
	BufferObject sphereEBO(GL_ELEMENT_ARRAY_BUFFER);

	VertexArrayObject sphereVertexArrayObject([&]() {
		sphereVBO.Start(sphere.vertices.data(), sphere.vertices.size() * sizeof(float));
		sphereEBO.Start(sphere.indices.data(), sphere.indices.size() * sizeof(GLuint));
		VertexArrayObject::LinkAttrib(0, 3, 5 * sizeof(float), 0);					 // Posi��o
		VertexArrayObject::LinkAttrib(1, 2, 5 * sizeof(float), (3 * sizeof(float))); // Textura
	}, true);

	// Define os atributos dos v�rtices (posi��o e textura)
	sphereVBO.End();
	sphereEBO.End();


	Cube cube;
	BufferObject cubeVBO(GL_ARRAY_BUFFER);
	BufferObject cubeEBO(GL_ELEMENT_ARRAY_BUFFER);

	VertexArrayObject cubeVertexArrayObject([&]() {
		cubeVBO.Start(cube.vertices.data(), cube.vertices.size() * sizeof(float));
		cubeEBO.Start(cube.indices.data(), cube.indices.size() * sizeof(GLuint));
		VertexArrayObject::LinkAttrib(0, 3, 5 * sizeof(float), 0);					 // Posi��o
		VertexArrayObject::LinkAttrib(1, 2, 5 * sizeof(float), (3 * sizeof(float))); // Textura
		}, true);

	// Define os atributos dos v�rtices (posi��o e textura)
	cubeVBO.End();
	cubeEBO.End();

	#pragma endregion

	// Cria a textura
	Texture brick(Tools::getPath("..\\resources\\textures\\brick\\brick.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brick.texUnit(shaderProgram, "texture0", 0); //Ativa��o da textura

	// Cria a textura
	Texture popCat(Tools::getPath("..\\resources\\textures\\brick\\pop_cat.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "texture0", 0); //Ativa��o da textura


	// Habilita o teste de profundidade
	glEnable(GL_DEPTH_TEST);

	// Loop de renderiza��o
	while (!glfwWindowShouldClose(window))
	{
		// Limpa a tela com a cor definida
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Usa o programa de shader
		shaderProgram.Activate();
		
		
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
		GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		GLuint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(PROJECTION));
		GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");



		// Desenhar cubo
		cubeVertexArrayObject.Context([&]() {

			popCat.Bind();
			cube
			.resetModel()
			.translate(glm::vec3(1.0f, 0.0f, 0.0f))
			.rotate((float)glfwGetTime() * -50.0f, glm::vec3(1.0f, 1.0f, 0.0f))
			.Draw(modelLoc);



		});

		// Desenhar cubo
		sphereVertexArrayObject.Context([&]() {



			brick.Bind();
			sphere
				.resetModel()
				.translate(glm::vec3(-1.0f, 0.0f, 0.0f))
				.rotate((float)glfwGetTime() * 50.0f, glm::vec3(1.0f, 1.0f, 0.0f))
				.Draw(modelLoc);

		});

		
		// Troca os buffers da janela
		glfwSwapBuffers(window);
		// Processa os eventos da janela
		glfwPollEvents();
	}

	// Deleta os recursos alocados
	//cubeVertexArrayObject.Delete();
	brick.Delete();
	popCat.Delete();
	shaderProgram.Delete();

	// Destroi a janela criada
	glfwDestroyWindow(window);

	// Encerra o GLFW, limpando os recursos alocados
	glfwTerminate();
	
	return 0;
}

