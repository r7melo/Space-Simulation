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



// ===================================================
// Definição de Constantes Globais
// ===================================================
const char* TITLE = "Space Simulation";
unsigned const int WIDTH = 800;
unsigned const int HEIGHT = 800;
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
}

// Função de Callback para movimento do mouse
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// A função é chamada toda vez que o mouse se move.
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

	// Registra a função de teclado
	glfwSetKeyCallback(window, key_callback);

	// Registra a função de redimensionamento
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Registra a função de movimento do mouse
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Define o contexto atual para a janela criada
	glfwMakeContextCurrent(window);


	// ========================================================================


	float vertices[] = {
		// posX, posY, posZ,   texU, texV
		 0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
		 1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
		 1.0f, 1.0f, 0.0f,     1.0f, 1.0f,
		 0.0f, 1.0f, 0.0f,     0.0f, 1.0f
	};


	GLuint indices[] = {
		0, 1 , 2,
		2, 3, 0,
	};


	// Carrega todos os ponteiros das funções do OpenGL usando GLAD
	gladLoadGL();

	// Define a viewport (área de renderização) com as dimensões da janela
	glViewport(0, 0, WIDTH, HEIGHT);

	ShaderManage shaderProgram("default.vert", "default.frag");

	#pragma region Configuracao e geometria
	// Cria os buffers e o array de vértices

	VertexArrayObject vertexArrayObject;

	BufferObject VBO(GL_ARRAY_BUFFER);
	BufferObject EBO(GL_ELEMENT_ARRAY_BUFFER);

	vertexArrayObject.Start();

		// Cria o Vertex Buffer Object (VBO) e o torna o objeto de estado ATIVO no contexto OpenGL (GPU)
		// Copia os dados dos vértices para o buffer de memória da GPU
		VBO.Start(vertices, sizeof(vertices));

		// Cria o Element Buffer Object (EBO) e o torna o objeto de estado ATIVO no contexto OpenGL (GPU)
		// Copia os dados dos índices para o buffer de memória da GPU
		EBO.Start(indices, sizeof(indices));

	vertexArrayObject.End();

	// Define os atributos dos vértices (posição)
	VBO.End();
	EBO.End();
	#pragma endregion

	// Cria a textura
	Texture texture(Tools::getPath("..\\resources\\textures\\brick\\brick.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.texUnit(shaderProgram, "texture0", 0); //Ativação da textura

	// Define a cor de limpeza da tela (preto)
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	// Limpa o buffer de cor
	glClear(GL_COLOR_BUFFER_BIT);
	// Troca os buffers da janela
	glfwSwapBuffers(window);

	// Loop de renderização
	while (!glfwWindowShouldClose(window))
	{
		// Limpa a tela com a cor definida
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Usa o programa de shader
		shaderProgram.Activate();
		texture.Bind();

		vertexArrayObject.Bind();

		// Desenha o triângulo
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		
		// Troca os buffers da janela
		glfwSwapBuffers(window);
		// Processa os eventos da janela
		glfwPollEvents();
	}

	// Deleta os recursos alocados
	vertexArrayObject.Delete();
	VBO.Delete();
	EBO.Delete();
	texture.Delete();
	shaderProgram.Delete();

	// Destroi a janela criada
	glfwDestroyWindow(window);

	// Encerra o GLFW, limpando os recursos alocados
	glfwTerminate();
	
	return 0;
}

