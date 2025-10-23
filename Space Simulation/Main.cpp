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

bool leftButtonPressed = false;
double lastX = 400, lastY = 300; // posição inicial do mouse
bool firstMouse = true;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 50.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f; // ângulo horizontal
float pitch = 0.0f;   // ângulo vertical
float speed = 10.0f;   // velocidade de movimento
float sensitivity = 0.1f; // sensibilidade do mouse
float deltaTime = 0.0f; // tempo entre o frame atual e o anterior
float lastFrame = 0.0f; // tempo do frame anterior


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


glm::mat4 getViewMatrix() {
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void processInput(GLFWwindow* window, float deltaTime) {
	float velocity = speed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraFront * velocity; // frente
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraFront * velocity; // trás
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity; // esquerda
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity; // direita

	// Movimentação vertical
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos += cameraUp * velocity; // sobe
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraPos -= cameraUp * velocity; // desce
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


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			leftButtonPressed = true;
			glfwGetCursorPos(window, &lastX, &lastY); // guarda posição inicial do clique
			firstMouse = true; // garante que o primeiro delta seja correto
			std::cout << "Mouse esquerdo pressionado!" << std::endl;
		}
		else if (action == GLFW_RELEASE) {
			leftButtonPressed = false;
			std::cout << "Mouse esquerdo solto!" << std::endl;
		}
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (!leftButtonPressed) return; // só atualiza se o botão esquerdo estiver pressionado

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double dx = xpos - lastX;
	double dy = lastY - ypos; // Y invertido porque GLFW cresce pra baixo
	lastX = xpos;
	lastY = ypos;

	// --- Detecta direção do arrasto ---
	if (std::abs(dx) > std::abs(dy)) {
		if (dx > 0) std::cout << "Arrastando para a direita" << std::endl;
		else if (dx < 0) std::cout << "Arrastando para a esquerda" << std::endl;
	}
	else {
		if (dy > 0) std::cout << "Arrastando para cima" << std::endl;
		else if (dy < 0) std::cout << "Arrastando para baixo" << std::endl;
	}

	// --- Atualiza câmera FPS ---
	dx *= sensitivity;
	dy *= sensitivity;

	yaw += dx;
	pitch += dy;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
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

	// Define tamanho mínimo da janela
	glfwSetWindowSizeLimits(window, 500, 500, GLFW_DONT_CARE, GLFW_DONT_CARE);

	// Registra a função de teclado
	glfwSetKeyCallback(window, key_callback);

	// Registra a função de redimensionamento
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Registra a função de movimento do mouse
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Define o contexto atual para a janela criada
	glfwMakeContextCurrent(window);


	// ======================================================================== OPENGL INITIALIZE ========================================================================

	// Carrega todos os ponteiros das funções do OpenGL usando GLAD
	gladLoadGL();

	// Define a viewport (área de renderização) com as dimensões da janela
	glViewport(0, 0, WIDTH, HEIGHT);

	ShaderManage shaderProgram("default.vert", "default.frag");



	#pragma region Configuracao e geometria
	// Cria os buffers e o array de vértices

	// Configuração do Cubo
	Sphere sphere(1.0f, 20, 20);
	BufferObject sphereVBO(GL_ARRAY_BUFFER);
	BufferObject sphereEBO(GL_ELEMENT_ARRAY_BUFFER);

	VertexArrayObject sphereVertexArrayObject([&]() {
		sphereVBO.Start(sphere.vertices.data(), sphere.vertices.size() * sizeof(float));
		sphereEBO.Start(sphere.indices.data(), sphere.indices.size() * sizeof(GLuint));
		VertexArrayObject::LinkAttrib(0, 3, 5 * sizeof(float), 0);					 // Posição
		VertexArrayObject::LinkAttrib(1, 2, 5 * sizeof(float), (3 * sizeof(float))); // Textura
	}, true);

	// Define os atributos dos vértices (posição e textura)
	sphereVBO.End();
	sphereEBO.End();


	Cube cube;
	BufferObject cubeVBO(GL_ARRAY_BUFFER);
	BufferObject cubeEBO(GL_ELEMENT_ARRAY_BUFFER);

	VertexArrayObject cubeVertexArrayObject([&]() {
		cubeVBO.Start(cube.vertices.data(), cube.vertices.size() * sizeof(float));
		cubeEBO.Start(cube.indices.data(), cube.indices.size() * sizeof(GLuint));
		VertexArrayObject::LinkAttrib(0, 3, 5 * sizeof(float), 0);					 // Posição
		VertexArrayObject::LinkAttrib(1, 2, 5 * sizeof(float), (3 * sizeof(float))); // Textura
	}, true);

	// Define os atributos dos vértices (posição e textura)
	cubeVBO.End();
	cubeEBO.End();

	#pragma endregion

	// Cria a textura
	Texture earth(Tools::getPath("..\\resources\\textures\\earth\\earth.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	earth.texUnit(shaderProgram, "texture0", 0); //Ativação da textura
	// Cria a textura
	Texture moon(Tools::getPath("..\\resources\\textures\\earth\\moon.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	moon.texUnit(shaderProgram, "texture0", 0); //Ativação da textura

	
	
	// Habilita o teste de profundidade
	glEnable(GL_DEPTH_TEST);

	// Loop de renderização
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, deltaTime);
		// Limpa a tela com a cor definida
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Usa o programa de shader
		shaderProgram.Activate();
		
		glm::mat4 view = getViewMatrix();
		GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		GLuint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(PROJECTION));
		GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");




		// Desenhar cubo
		sphereVertexArrayObject.Context([&]() {

			earth.Bind();
			sphere
			.resetModel()
			.translate(glm::vec3(0.0f, 0.0f, 0.0f))
			.rotate((float)glfwGetTime() * 50.0f, glm::vec3(1.0f, 1.0f, 0.0f))
			.Draw(modelLoc);

			float scaleMoon = 0.25f;
			moon.Bind();
			sphere
			.resetModel()
			.translate(glm::vec3(2.0f, 0.0f, 0.0f))
			.rotate((float)glfwGetTime() * 50.0f, glm::vec3(1.0f, 1.0f, 0.0f))
			.scale(glm::vec3(scaleMoon, scaleMoon, scaleMoon))
			.Draw(modelLoc);

		});

		
		// Troca os buffers da janela
		glfwSwapBuffers(window);
		// Processa os eventos da janela
		glfwPollEvents();
	}

	// Deleta os recursos alocados
	//cubeVertexArrayObject.Delete();
	earth.Delete();
	moon.Delete();
	shaderProgram.Delete();

	// Destroi a janela criada
	glfwDestroyWindow(window);

	// Encerra o GLFW, limpando os recursos alocados
	glfwTerminate();
	
	return 0;
}

