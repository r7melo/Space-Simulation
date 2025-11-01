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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"
#include <vector>

// Objetos da cena
#include "Sphere.h"
#include "CamManager.h"
#include "FisicsObject.h"

// Biblioteca de controle de ações de entrada
#include "Input.h"



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

// Callback para redimensionamento da janela
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// Atualiza a viewport do OpenGL
	glViewport(0, 0, width, height);

	// Atualiza a matriz de projeção global
	PROJECTION = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
}


// FUNÇÃO CONTEXTO OPENGL
void contextOpenGL(GLFWwindow* window)
{
	// ======================================================================== OPENGL INITIALIZE ========================================================================

	// Carrega todos os ponteiros das funções do OpenGL usando GLAD
	gladLoadGL();

	CamManager camManager;

	FisicsObject planeta(1000.0, 5.0, glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 0.0, 0.0));
	FisicsObject lua(1.0, 1.0, glm::dvec3(17.0, 0.0, 0.0), glm::dvec3(0.0, 7.6, 0.0));
	
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
		Input::update();

		// Calcula tempo do frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Variáveis de movimento
		float velocity = speed * deltaTime;

		camManager.processKeyboard(deltaTime);
		camManager.processMouse();

		// Limpa a tela com a cor definida
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Usa o programa de shader
		shaderProgram.Activate();

		// Atualiza matrizes
		glm::mat4 view = camManager.getViewMatrix();
		GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		GLuint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
		GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(PROJECTION));

		double distancia;
		bool colisao = lua.atualizar(planeta, deltaTime, distancia); // aqui FisicsObject calcula gravidade real
		if (colisao) {
			std::cout << "Colisão Lua-Terra!\n";
		}

		// Desenhar esferas
		sphereVertexArrayObject.Context([&]() {

			earth.Bind();
			sphere
				.resetModel()
				.translate(planeta.getPos())
				.rotate((float)glfwGetTime() * 50.0f, glm::vec3(1.0f, 1.0f, 0.0f))
				.Draw(modelLoc);

			float scaleMoon = 0.25f;
			moon.Bind();
			sphere
				.resetModel()
				.translate(lua.getPos())
				.rotate((float)glfwGetTime() * 50.0f, glm::vec3(1.0f, 1.0f, 0.0f))
				.scale(glm::vec3(0.25f))
				.Draw(modelLoc);

			});

		// Troca os buffers da janela
		glfwSwapBuffers(window);
		// Processa os eventos da janela
		glfwPollEvents();
	}


	// Deleta os recursos alocados
	earth.Delete();
	moon.Delete();
	shaderProgram.Delete();
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
	if (window == NULL) { glfwTerminate(); return -1; }

	// Define tamanho mínimo da janela
	glfwSetWindowSizeLimits(window, 500, 500, GLFW_DONT_CARE, GLFW_DONT_CARE);

	// Define o contexto atual para a janela criada
	glfwMakeContextCurrent(window);

	// Ação de atualização ao redirecionar a janela
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Inicia o monitoramento das entradas
	Input::init(window);

	// Inicia o contexto OpenGL
	contextOpenGL(window);

	// Destroi a janela criada
	glfwDestroyWindow(window);

	// Encerra o GLFW, limpando os recursos alocados
	glfwTerminate();

	return 0;
}
