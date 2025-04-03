/*
Práctica 9: Fuentes de luz
Leonardo Ariel Berdejo Guzmán
4 abril 2025
318034320
*/

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//Para el control del día y la noche
bool day = true;
bool night = false;
float moonRotate = 90.0f;
float sunRotate = 90.0f;

//Radio de la órbita del sol y la luna
float radius = 6.0f;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(1.3f, 0.2f, 0.9f),     // Mushroom position
	glm::vec3(-1.8f, -0.1f, 0.47f),    // Candle position
	glm::vec3(0.19f, -0.2f, -1.51f)     // Lamp position
};

// Point light colors
float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

//Para el cambio de colores en las luces 


//Luz 1
float R0 = 1.0f; float G0 = 0.5f; float B0 = 0.5;
//Luz 2
float R1 = 1.0f; float G1 = 0.5f; float B1 = 0.5;
//Luz 3
float R2 = 1.0f; float G2 = 0.5f; float B2 = 0.5;

//Intensidad de las luces
float con = 0.2f; float lin = 0.2f; float qua = 0.2f;

// Paso de ajuste de la especular
float specularStep = 0.1f;

glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9: Fuentes de luz-Leonardo Berdejo", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	Model Dog((char*)"Models/glass.obj");
	Model Piso((char*)"Models/piso.obj");
	//Modelos de la práctica anterior
	Model red_dog((char*)"Models/RedDog.obj");//Ruta del perro 3D del previo
	Model bird((char*)"Models/12213_Bird_v1_l3.obj");//Ruta del pajaro 3D
	Model beachBall((char*)"Models/13517_Beach_Ball_v2_L3.obj");//Ruta del balon de playa 3D
	Model duck((char*)"Models/12248_Bird_v1_L2.obj");//Ruta del pato 3D
	Model barril((char*)"Models/WoodenBarrel.obj");//Ruta del barril 3D
	Model island((char*)"Models/isla.obj");//Ruta de la isla 3D
	Model sun((char*)"Models/Sun.obj");//Ruta del sol 3D
	Model moon((char*)"Models/Moon.obj");//Ruta de la luna 3D
	Model palmTree((char*)"Models/palmTree.obj");//Ruta de la palma 3D
	//Modelos de las luces
	Model mushroom((char*)"Models/mushroom.obj");//Ruta del hongo 3D
	Model lampE((char*)"Models/lampExterior.obj");//Ruta la lámpara 3D
	Model candle((char*)"Models/candle.obj");//Ruta de la vela 3D


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	//Textura para la Luna
	GLuint moonTexture;
	glGenTextures(1, &moonTexture);
	glBindTexture(GL_TEXTURE_2D, moonTexture);
	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image;
	// Cargar la textura de la Luna
	image = stbi_load("Models/Texturas/pedra-166.jpg", &textureWidth, &textureHeight, &nrChannels, 0);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load moon texture" << std::endl;
	}
	stbi_image_free(image);

	// Configuración de parámetros de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	//Textura para el Sol
	GLuint sunTexture;
	glGenTextures(1, &sunTexture);
	glBindTexture(GL_TEXTURE_2D, sunTexture);

	stbi_set_flip_vertically_on_load(true);
	// Cargar la textura del Sol
	image = stbi_load("Models/Texturas/sun.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load sun texture" << std::endl;
	}
	stbi_image_free(image);

	// Configuración de parámetros de la textura del Sol
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		
		//Load Model
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.1f, 0.1f, 0.1f);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), R0, G0, B0); //Cambio de color en la luz 1
		//Nos ayudará a calcular la atenuación de la luz
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), con);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), lin);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), qua);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f); //Varia colores
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.05f, 0.05f, 0.05f);	 //varia colores
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), R1, G1, B1);	//Cambio de color en la luz 2
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), con);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), lin);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), qua);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), R2, G2, B2); //Cambio de color luz 3
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), con);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), lin);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), qua);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);//Para que se vea un poquito más azul
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);//Para que se vea un poquito más azul
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



		////Carga de modelo 
  //      view = camera.GetViewMatrix();	
		//model = glm::mat4(1);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Piso.Draw(lightingShader);

		//Dibujo de modelos
		// Draw the loaded model gg
		glm::mat4 model(1);

		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp
		modelTemp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		red_dog.Draw(lightingShader);

		//Dibujo del modelo del pajaro	gg	
		
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.03f, 0.11f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		bird.Draw(lightingShader);

		//Dibujo de la isla (1)gg
		model = glm::translate(modelTemp, glm::vec3(0.0f, -0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3, 0.3f, 0.3f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		island.Draw(lightingShader);

		//Dibujo del modelo del barril (2)gg
		modelTemp2 = model = glm::translate(modelTemp, glm::vec3(-2.0f, -0.39f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		barril.Draw(lightingShader);

		//Dibujo del modelo del pato (3)gg
		model = glm::translate(modelTemp2, glm::vec3(0.0f, 0.712f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		duck.Draw(lightingShader);

		//Dibujo del modelo del balon de playa (4)gg
		modelTemp2 = model = glm::translate(modelTemp, glm::vec3(1.0f, -0.25f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		beachBall.Draw(lightingShader);

		//Dibujo de la palma (5)gg
		model = glm::translate(modelTemp, glm::vec3(9.0f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		palmTree.Draw(lightingShader);

		//glBindVertexArray(0);

	
		//model = glm::mat4(1);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
	 //   Dog.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glBindVertexArray(0);
	

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(5.0f, 6.0f, 1.0f));
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.8f)); // Make it a smaller cube
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Draw the light object (using light's vertex attributes)
			// For mushroom			
			modelTemp2 = model = glm::translate(modelTemp2, glm::vec3(0.3f, 0.3f, -0.1f));
			model = glm::scale(model, glm::vec3(0.1f));
			//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotación del foco
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			mushroom.Draw(lampShader);
		
			// For candle			
			modelTemp = model = glm::translate(modelTemp2, glm::vec3(-3.1f, -0.43f, -0.42f));
			model = glm::scale(model, glm::vec3(0.1f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			candle.Draw(lampShader);
		
			// For Moscow lamp			
			model = glm::translate(modelTemp, glm::vec3(2.0f, 0.1f, -2.0f));
			model = glm::scale(model, glm::vec3(0.05f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			lampE.Draw(lampShader);

			// Dibuja cubos blancos en las posiciones de las luces para facilitar su visualización
			glBindVertexArray(VAO); // Usa el VAO que ya tienes para los vértices del cubo

			// Configura un shader básico para dibujar los cubos con un color sólido
			lightingShader.Use();

			// Para el cubo de la luz 1 (Mushroom)
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[0]);
			model = glm::scale(model, glm::vec3(0.2f)); // Escala pequeña pero visible
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			// Establece un color blanco brillante para el cubo
			glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Para el cubo de la luz 2 (Candle)
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[1]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Para el cubo de la luz 3 (Lamp)
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[2]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}
	// Limitación a 180° de la Luna y el Sol
	if (keys[GLFW_KEY_L]) {
		moonRotate += 1.0f; // Positivo para moverse a la derecha
		if (moonRotate > 180.0f) {
			moonRotate = 180.0f;
		}

		sunRotate += 1.0f;
		if (sunRotate > 180.0f) {
			sunRotate = 180.0f;
		}
	}

	if (keys[GLFW_KEY_O]) {
		moonRotate -= 1.0f; // Negativo para moverse a la izquierda
		if (moonRotate < 0.0f) {
			moonRotate = 0.0f;
		}

		sunRotate -= 1.0f;
		if (sunRotate < 0.0f) {
			sunRotate = 0.0f;
		}
	}
	if (keys[GLFW_KEY_Q])
	{
		pointLightPositions[1].x += 0.01f;
	}
	if (keys[GLFW_KEY_E])
	{
		pointLightPositions[1].x -= 0.01f;
	}

	if (keys[GLFW_KEY_P])
	{
		pointLightPositions[1].y += 0.01f;
	}

	if (keys[GLFW_KEY_M])
	{
		pointLightPositions[1].y -= 0.01f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[1].z -= 0.01f;
	}
	if (keys[GLFW_KEY_K])
	{
		pointLightPositions[1].z += 0.01f;
	}
	// Cambios para Point light 1 - Focos
	if (keys[GLFW_KEY_Z]) {
		R0 += 0.01f;
		if (R0 > 5.0f) R0 = 5.0f;
	}
	if (keys[GLFW_KEY_X]) {
		R0 -= 0.01f;
		if (R0 < 0.0f) R0 = 0.0f;
	}
	if (keys[GLFW_KEY_C]) {
		G0 += 0.01f;
		if (G0 > 5.0f) G0 = 5.0f;
	}
	if (keys[GLFW_KEY_V]) {
		G0 -= 0.01f;
		if (G0 < 0.0f) G0 = 0.0f;
	}
	if (keys[GLFW_KEY_B]) {
		B0 += 0.01f;
		if (B0 > 3.0f) B0 = 3.0f;
	}
	if (keys[GLFW_KEY_N]) {
		B0 -= 0.01f;
		if (B0 < 0.0f) B0 = 0.0f;
	}


	//Cambios para Point light 2 - Faro
	if (keys[GLFW_KEY_R]) {
		R1 += 0.01f;
		if (R1 > 5.0f) R1 = 5.0f;
	}
	if (keys[GLFW_KEY_F]) {
		R1 -= 0.01f;
		if (R1 < 0.0f) R1 = 0.0f;
	}
	if (keys[GLFW_KEY_T]) {
		G1 += 0.01f;
		if (G1 > 5.0f) G1 = 5.0f;
	}
	if (keys[GLFW_KEY_G]) {
		G1 -= 0.01f;
		if (G1 < 0.0f) G1 = 0.0f;
	}
	if (keys[GLFW_KEY_Y]) {
		B1 += 0.01f;
		if (B1 > 5.0f) B1 = 5.0f;
	}
	if (keys[GLFW_KEY_H]) {
		B1 -= 0.01f;
		if (B1 < 0.0f) B1 = 0.0f;
	}
	//Cambios para Point light 3 - Disco ballw

	if (keys[GLFW_KEY_7]) {
		R2 += 0.01f;
		if (R2 > 5.0f) R2 = 5.0f;
	}
	if (keys[GLFW_KEY_U]) {
		R2 -= 0.01f;
		if (R2 < 0.0f) R2 = 0.0f;
	}
	if (keys[GLFW_KEY_8]) {
		G2 += 0.01f;
		if (G2 > 5.0f) G2 = 5.0f;
	}
	if (keys[GLFW_KEY_I]) {
		G2 -= 0.01f;
		if (G2 < 0.0f) G2 = 0.0f;
	}
	if (keys[GLFW_KEY_9]) {
		B2 += 0.01f;
		if (B2 > 5.0f) B2 = 5.0f;
	}
	if (keys[GLFW_KEY_0]) {
		B2 -= 0.01f;
		if (B2 < 0.0f) B2 = 0.0f;
	}

	//Cambios para Directional light

	if (keys[GLFW_KEY_1]) {
		con += 0.01f;
		if (con > 5.0f) con = 5.0f;
	}
	if (keys[GLFW_KEY_2]) {
		con -= 0.01f;
		if (con < 0.0f) con = 0.0f;
	}
	if (keys[GLFW_KEY_3]) {
		lin += 0.01f;
		if (lin > 5.0f) lin = 5.0f;
	}
	if (keys[GLFW_KEY_4]) {
		lin -= 0.01f;
		if (lin < 0.0f) lin = 0.0f;
	}
	if (keys[GLFW_KEY_5]) {
		qua += 0.01f;
		if (qua > 3.0f) qua = 3.0f;
	}
	if (keys[GLFW_KEY_6]) {
		qua -= 0.01f;
		if (qua < 0.0f) qua = 0.0f;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	////Botón J y K para cambiar entre día y noche
	//if (keys[GLFW_KEY_J])
	//{
	//	night = true;
	//	day = false;
	//}

	//if (keys[GLFW_KEY_K])
	//{
	//	night = false;
	//	day = true;
	//}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}