// Bibliote de entrada e saída
#include <iostream>

// Biblioteca GLAD para carregar os ponteiros das funções do OpenGL
#include <glad/glad.h>

// Biblioteca GLFW para criação de janelas e contexto OpenGL
#include <GLFW/glfw3.h>

// Biblioteca GLM para matemática (vetores e matrizes)
#include <glm/glm.hpp>

// Biblioteca personalizada para gerenciamento de shaders
#include "ShaderManage.h"
#include "VertexArrayObject.h"
#include "BufferObject.h"
#include "Texture.h"
#include "Tools.h"

// Bibliotecas GLM adicionais para transformações
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"
#include <vector>
#include "Sphere.h"


// ===================================================
// Definição de Constantes Globais
// ===================================================
const char* TITLE = "Space Simulation";
unsigned const int WIDTH = 800;
unsigned const int HEIGHT = 800;

glm::mat4 PROJECTION = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
// ===================================================



// ===================================================
// Definição das Funções de Callback (GLOBALMENTE)
// ===================================================

// Função de Callback para eventos de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	std::cout << "Evento de Tecla: " << key << std::endl;
}

// Função de Callback para redimensionamento da janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Esta função é importante para o OpenGL.
	// Ela ajusta a área de renderização (viewport) quando a janela é redimensionada.

	glViewport(0, 0, width, height);

	// Atualiza a projeção (mantendo proporção)
	float aspect = (float)width / (float)height;
	PROJECTION = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

// Função de Callback para movimento do mouse
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// A função é chamada toda vez que o mouse se move.
	// Usamos std::cout para imprimir as coordenadas (xpos e ypos).
	std::cout << "Mouse Movido! X: " << xpos << ", Y: " << ypos << std::endl;
}

// ===================================================

void drawSphere(GLuint shaderID, GLint modelLoc, float radius = 1.0f, int stacks = 20, int slices = 20)
{
	static std::vector<float> vertices;
	static std::vector<GLuint> indices;
	static GLuint VAO = 0, VBO, EBO;

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

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		// Posição
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Textura
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	// Model matrix
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 0.0f, glm::vec3(0.5f, 1.0f, 0.0f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


// FUNCAO PRINCIPAL
int main()
{
	// Initialize GLFW
    glfwInit();

	// Configure GLFW para usar OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Versão maior do OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Versão menor do OpenGL

	// Define o perfil do OpenGL, que é o núcleo (core profile)
	// remove todas as funcionalidades antigas do (legacy) e obriga 
	// o uso de Shaders (Programas de GPU) para renderização.
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

	// Define tamanho mínimo da janela
	glfwSetWindowSizeLimits(window, 500, 500, GLFW_DONT_CARE, GLFW_DONT_CARE);

	// Registra a função de teclado
	glfwSetKeyCallback(window, key_callback);

	// Registra a função de redimensionamento
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Registra a função de movimento do mouse
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Define o contexto atual para a janela criada
	glfwMakeContextCurrent(window);


	// ======================================================================== OPENGL INITIALIZE ========================================================================

	// Carrega todos os ponteiros das funções do OpenGL usando GLAD
	gladLoadGL();

	// Define a viewport (área de renderização) com as dimensões da janela
	glViewport(0, 0, WIDTH, HEIGHT);

	ShaderManage shaderProgram("default.vert", "default.frag");

	Cube cube01;
	Sphere sphere01(1.0f, 20, 20);

	#pragma region Configuracao e geometria
	// Cria os buffers e o array de vértices

	VertexArrayObject cubeVertexArrayObject, sphereVertexArrayObject;

	BufferObject VBO(GL_ARRAY_BUFFER);
	BufferObject EBO(GL_ELEMENT_ARRAY_BUFFER);

	cubeVertexArrayObject.Start();

		// Cria o Vertex Buffer Object (VBO) e o torna o objeto de estado ATIVO no contexto OpenGL (GPU)
		// Copia os dados dos vértices para o buffer de memória da GPU
		VBO.Start(cube01.getVertices(), cube01.getVerticesSize());

		// Cria o Element Buffer Object (EBO) e o torna o objeto de estado ATIVO no contexto OpenGL (GPU)
		// Copia os dados dos índices para o buffer de memória da GPU
		EBO.Start(cube01.getIndices(), cube01.getIndicesSize());

	cubeVertexArrayObject.End();

	sphereVertexArrayObject.Start();

		// Cria o Vertex Buffer Object (VBO) e o torna o objeto de estado ATIVO no contexto OpenGL (GPU)
		// Copia os dados dos vértices para o buffer de memória da GPU
		VBO.Start(sphere01.getVertices(), sphere01.getVerticesSize());

		// Cria o Element Buffer Object (EBO) e o torna o objeto de estado ATIVO no contexto OpenGL (GPU)
		// Copia os dados dos índices para o buffer de memória da GPU
		EBO.Start(sphere01.getIndices(), sphere01.getIndicesSize());

	sphereVertexArrayObject.End();

	// Define os atributos dos vértices (posição)
	VBO.End();
	EBO.End();
	#pragma endregion

	// Cria a textura
	Texture brick(Tools::getPath("..\\resources\\textures\\brick\\brick.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brick.texUnit(shaderProgram, "texture0", 0); //Ativação da textura

	// Cria a textura
	Texture popCat(Tools::getPath("..\\resources\\textures\\brick\\pop_cat.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "texture0", 0); //Ativação da textura


	glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade

	// Define a cor de limpeza da tela (preto)
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	// Limpa o buffer de cor
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Troca os buffers da janela
	glfwSwapBuffers(window);


	// Loop de renderização
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
		cubeVertexArrayObject.Bind();
		popCat.Bind();
		cube01
			.resetModel()
			.translate(glm::vec3(0.0f, 0.0f, 0.0f))
			.rotate((float)glfwGetTime() * 50.0f, glm::vec3(1.0f, 1.0f, 0.0f))
			.Draw(modelLoc);
		cubeVertexArrayObject.Unbind();

		// Desenhar esfera	
		//sphereVertexArrayObject.Bind();
		//brick.Bind();
		//// Model matrix
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 0.0f, glm::vec3(0.5f, 1.0f, 0.0f));

		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//glDrawElements(GL_TRIANGLES, sphere01.getIndicesSize(), GL_UNSIGNED_INT, 0);
		//sphereVertexArrayObject.Unbind();

		
		// Troca os buffers da janela
		glfwSwapBuffers(window);
		// Processa os eventos da janela
		glfwPollEvents();
	}

	// Deleta os recursos alocados
	cubeVertexArrayObject.Delete();
	VBO.Delete();
	EBO.Delete();
	brick.Delete();
	popCat.Delete();
	shaderProgram.Delete();

	// Destroi a janela criada
	glfwDestroyWindow(window);

	// Encerra o GLFW, limpando os recursos alocados
	glfwTerminate();
	
	return 0;
}

