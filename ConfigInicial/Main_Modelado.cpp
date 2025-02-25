/*
Pr�ctica 4: Modelado Geom�trico
Leonardo Ariel Berdejo Guzm�n
24 febrero 2025
318034320
*/
#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f;
float movY=0.0f;
float movZ=-5.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4 Leonardo Berdejo", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificaci�n de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
	//	0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
	//	0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
	//	0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
	//	-0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
	//	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,
	//	
	//    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
	//	 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//	 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//	 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//    -0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	//	
	//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
	//	 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,
 //     
	//	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	//	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	//	
	//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
	//	-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
	//	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
	//	
	//	-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	//	0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
	//	0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
	//	-0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
	//	-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	//};
	float verticesVerde[] = {
		-0.5f, -0.5f, 0.5f, 0.13f, 0.76f, 0.16f,//Front
		0.5f, -0.5f, 0.5f,  0.13f, 0.76f, 0.16f,
		0.5f,  0.5f, 0.5f,  0.13f, 0.76f, 0.16f,
		0.5f,  0.5f, 0.5f,  0.13f, 0.76f, 0.16f,
		-0.5f,  0.5f, 0.5f, 0.13f, 0.76f, 0.16f,
		-0.5f, -0.5f, 0.5f, 0.13f, 0.76f, 0.16f,

		-0.5f, -0.5f,-0.5f, 0.13f, 0.76f, 0.16f,//Back
		 0.5f, -0.5f,-0.5f, 0.13f, 0.76f, 0.16f,
		 0.5f,  0.5f,-0.5f, 0.13f, 0.76f, 0.16f,
		 0.5f,  0.5f,-0.5f, 0.13f, 0.76f, 0.16f,
		-0.5f,  0.5f,-0.5f, 0.13f, 0.76f, 0.16f,
		-0.5f, -0.5f,-0.5f, 0.13f, 0.76f, 0.16f,

		 0.5f, -0.5f,  0.5f,  0.13f, 0.6f, 0.16f,
		 0.5f, -0.5f, -0.5f,  0.13f, 0.6f, 0.16f,
		 0.5f,  0.5f, -0.5f,  0.13f, 0.6f, 0.16f,
		 0.5f,  0.5f, -0.5f,  0.13f, 0.6f, 0.16f,
		 0.5f,  0.5f,  0.5f,  0.13f, 0.6f, 0.16f,
		 0.5f,  -0.5f, 0.5f, 0.13f, 0.6f, 0.16f,

		-0.5f,  0.5f,  0.5f,  0.13f, 0.6f, 0.16f,
		-0.5f,  0.5f, -0.5f,  0.13f, 0.6f, 0.16f,
		-0.5f, -0.5f, -0.5f,  0.13f, 0.6f, 0.16f,
		-0.5f, -0.5f, -0.5f,  0.13f, 0.6f, 0.16f,
		-0.5f, -0.5f,  0.5f,  0.13f, 0.6f, 0.16f,
		-0.5f,  0.5f,  0.5f,  0.13f, 0.6f, 0.16f,

		-0.5f, -0.5f, -0.5f, 0.13f, 0.76f, 0.16f,
		0.5f, -0.5f, -0.5f,  0.13f, 0.76f, 0.16f,
		0.5f, -0.5f,  0.5f,  0.13f, 0.76f, 0.16f,
		0.5f, -0.5f,  0.5f,  0.13f, 0.76f, 0.16f,
		-0.5f, -0.5f,  0.5f, 0.13f, 0.76f, 0.16f,
		-0.5f, -0.5f, -0.5f, 0.13f, 0.76f, 0.16f,

		-0.5f,  0.5f, -0.5f, 0.13f, 0.76f, 0.16f,
		0.5f,  0.5f, -0.5f,  0.13f, 0.76f, 0.16f,
		0.5f,  0.5f,  0.5f,  0.13f, 0.76f, 0.16f,
		0.5f,  0.5f,  0.5f,  0.13f, 0.76f, 0.16f,
		-0.5f,  0.5f,  0.5f, 0.13f, 0.76f, 0.16f,
		-0.5f,  0.5f, -0.5f, 0.13f, 0.76f, 0.16f,
	};

	float verticesBlanco[] = {
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f,1.0f,//Front
		0.5f, -0.5f, 0.5f,  1.0f, 1.0f,1.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 1.0f,1.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 1.0f,1.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 1.0f,1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f,1.0f,

		-0.5f, -0.5f,-0.5f, 1.0f, 1.0f,1.0f,//Back
		 0.5f, -0.5f,-0.5f, 1.0f, 1.0f,1.0f,
		 0.5f,  0.5f,-0.5f, 1.0f, 1.0f,1.0f,
		 0.5f,  0.5f,-0.5f, 1.0f, 1.0f,1.0f,
		-0.5f,  0.5f,-0.5f, 1.0f, 1.0f,1.0f,
		-0.5f, -0.5f,-0.5f, 1.0f, 1.0f,1.0f,

		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,1.0f,//Right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,
		 0.5f,  -0.5f, 0.5f, 1.0f, 1.0f,1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,//Left
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,1.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f,1.0f,//Bottom
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f,1.0f,

		-0.5f,  0.5f, -0.5f, 0.13f, 0.76f, 0.16f,//Top
		0.5f,  0.5f, -0.5f,  0.13f, 0.76f, 0.16f,
		0.5f,  0.5f,  0.5f,  0.13f, 0.76f, 0.16f,
		0.5f,  0.5f,  0.5f,  0.13f, 0.76f, 0.16f,
		-0.5f,  0.5f,  0.5f, 0.13f, 0.76f, 0.16f,
		-0.5f,  0.5f, -0.5f, 0.13f, 0.76f, 0.16f,
	};




	// Generar y configurar VAO y VBO para verticesVerde
	GLuint VBOVerde, VAOVerde;
	glGenVertexArrays(1, &VAOVerde);
	glGenBuffers(1, &VBOVerde);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAOVerde);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBOVerde);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesVerde), verticesVerde, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	// Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	// Generar y configurar VAO y VBO para verticesBlanco
	GLuint VBOBlanco, VAOBlanco;
	glGenVertexArrays(1, &VAOBlanco);
	glGenBuffers(1, &VBOBlanco);

	// Enlazar Vertex Array Object
	glBindVertexArray(VAOBlanco);

	// Copiar arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBOBlanco);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBlanco), verticesBlanco, GL_STATIC_DRAW);

	// Configurar las caracter�sticas de los vertices
	// Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // Unbind VAO

	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
	

		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAOVerde);
		
		//Cuerpo verde
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 3.0f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Posici�n del cuerpo
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Brazo izquierdo verde
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 2.5f, 1.0f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(-2.5f, 0.1f, 0.2f)); // Posici�n del brazo izquierdo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Brazo derecho verde
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 2.5f, 1.0f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(2.5f, 0.1f, 0.2f)); // Posici�n del brazo derecho
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pierna izquierda verde
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(-1.0f, -2.5f, 0.0f)); // Posici�n de la pierna izquierda
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pierna derecha verde
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(1.0f, -2.5f, 0.0f)); // Posici�n de la pierna derecha
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// Dibujar el cuello blanco
		glBindVertexArray(VAOBlanco);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(3.0f, 2.2f, 2.0f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // Posici�n del cuello
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ojo derecho blanco
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(-2.5f, 7.7f, 2.51f)); // Posici�n del ojo derecho
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ojo izquierdo blanco
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(2.5f, 7.7f, 2.51f)); // Posici�n del ojo izquierdo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cabeza verde
		glBindVertexArray(VAOVerde);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(4.0f, 2.0f, 3.0f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(0.0f, 1.8f, 0.0f)); // Posici�n de la cabeza
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Antena derecha verde cubo 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(-2.5f, 9.7f, 0.0f)); // Posici�n de la antena derecha
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Antena derecha verde cubo 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(-3.5f, 10.7f, 0.0f)); // Posici�n de la antena derecha
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Antena izquierda verde cubo 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(2.5f, 9.7f, 0.0f)); // Posici�n de la antena izquierda
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Antena izquierda verde cubo 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Ancho, grosor, profundidad
		model = glm::translate(model, glm::vec3(3.5f, 10.7f, 0.0f)); // Posici�n de la antena izquierda
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAOVerde);
	glDeleteBuffers(1, &VBOVerde);
	glDeleteVertexArrays(1, &VAOBlanco);
	glDeleteBuffers(1, &VBOBlanco);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		 movY += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		 movY -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.4f;
 }


