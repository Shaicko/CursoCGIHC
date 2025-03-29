/*
Práctica 8: Materiales e Iluminacion
Leonardo Ariel Berdejo Guzmán
28 marzo 2025
318034320
*/

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 1200, HEIGHT = 1000;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//Para el control del día y la noche
bool day = true;
bool night = false;
float moonRotate = 90.0f;
float sunRotate = 90.0f;

//Radio de la órbita del sol y la luna
float radius = 6.0f;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(radius* cos(glm::radians(sunRotate)), radius* sin(glm::radians(sunRotate)), 0.0f);
glm::vec3 newlightPos(radius* cos(glm::radians(moonRotate)), radius* sin(glm::radians(moonRotate)), 0.0f);

float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

float rot = 0.0f;
bool activanim = false;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8: Materiales e Iluminacion-Leonardo Berdejo", nullptr, nullptr);

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
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

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

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    Model red_dog((char*)"Models/RedDog.obj");//Ruta del perro 3D del previo
    Model bird((char*)"Models/12213_Bird_v1_l3.obj");//Ruta del pajaro 3D
    Model beachBall((char*)"Models/13517_Beach_Ball_v2_L3.obj");//Ruta del balon de playa 3D
    Model duck((char*)"Models/12248_Bird_v1_L2.obj");//Ruta del pato 3D
    Model barril((char*)"Models/WoodenBarrel.obj");//Ruta del barril 3D
    Model island((char*)"Models/isla.obj");//Ruta de la isla 3D
    Model sun((char*)"Models/Sun.obj");//Ruta del sol 3D
	Model moon((char*)"Models/Moon.obj");//Ruta de la luna 3D
    Model palmTree((char*)"Models/palmTree.obj");//Ruta de la palma 3D

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

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

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texturas/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);

    // Configuración de parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    //Textura para la Luna
    GLuint moonTexture;
    glGenTextures(1, &moonTexture);
    glBindTexture(GL_TEXTURE_2D, moonTexture);

    stbi_set_flip_vertically_on_load(true);
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
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Configuracion de las luces
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint newlightPosLoc = glGetUniformLocation(lightingShader.Program, "newlight.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");


        // Envía las posiciones de las luces
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.position"), newlightPos.x, newlightPos.y, newlightPos.z);

        // Envía las propiedades de las luces (ambient, diffuse, specular)
        if (day) {
            //día - luz solar brillante, luna apagada
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.7f, 0.7f, 0.75f);   // Luz ambiental más fuerte
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.7f, 1.6f, 1.5f);   // Luz solar directa intensísima
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.2f, 1.2f, 1.2f);  // Reflexión más fuerte

            // Apagar la luz de la luna completamente
            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.specular"), 0.0f, 0.0f, 0.0f);
        }
        else if (night) {
            // Apagar la luz del sol completamente
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);

            //noche - luz de luna llena más visible y azulada
            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.ambient"), 0.5f, 0.5f, 0.8f);   // Luz ambiental azulada
            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.diffuse"), 1.2f, 1.2f, 1.8f);   // Luz difusa lunar más azulada
            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.specular"), 1.5f, 1.5f, 1.8f);  // Reflexión lunar más pronunciada
        }

        // Envía la posición de la cámara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        //Visualizar el día y la noche
        if (day) {
            // Materiales que simulan el día
            lightPos = glm::vec3(radius * cos(glm::radians(sunRotate)), radius * sin(glm::radians(sunRotate)), 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.7f, 0.7f, 0.5f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.8f, 0.8f, 0.6f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
        }
        else if (night) {
            // Configuración para el día (Sol)
            newlightPos = glm::vec3(radius * cos(glm::radians(moonRotate)), radius * sin(glm::radians(moonRotate)), 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.1f, 0.1f, 0.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.2f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.4f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
        }

        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
         
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
        model = glm::translate(modelTemp, glm::vec3(1.0f, -0.25f, 1.0f));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        beachBall.Draw(lightingShader);

        //Dibujo de la palma (5)gg
        model = glm::translate(modelTemp, glm::vec3(9.0f, 0.0f, -2.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        palmTree.Draw(lightingShader);

        glBindVertexArray(0);


		//Solo para la luz
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));        
        
        // Envía la posición de la cámara al shader
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        //Dibujo de luz LUNA (1)
        if (night) {
            glm::mat4 modelMoon(1);
            // Calcula la posición en el arco basado en moonRotate
            float xPos = radius * cos(glm::radians(moonRotate));
            float yPos = radius * sin(glm::radians(moonRotate));

            modelMoon = glm::translate(modelMoon, glm::vec3(xPos, yPos, 0.0f)); // Traslación para mover el sol en un arco
            modelMoon = glm::rotate(modelMoon, glm::radians(moonRotate), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación alrededor del eje Y
            modelMoon = glm::scale(modelMoon, glm::vec3(0.3f)); // Escala el modelo

            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMoon));
            glBindVertexArray(VAO);
            //aplicando textura de luna
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, moonTexture);
            glUniform1i(glGetUniformLocation(lampshader.Program, "material.diffuse"), 0);
			//Dibujo del modelo de luna
            moon.Draw(lampshader);
            glBindVertexArray(0);
        }

        //Dibujo de luz SOL (2)
        if (day) {
            glm::mat4 modelSun(1);
            // Calcula la posición en el arco basado en sunRotate
            float xPos = radius * cos(glm::radians(sunRotate));
            float yPos = radius * sin(glm::radians(sunRotate));

            modelSun = glm::translate(modelSun, glm::vec3(xPos, yPos, 0.0f)); // Traslación para mover el sol en un arco
            modelSun = glm::rotate(modelSun, glm::radians(sunRotate), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación alrededor del eje Y
            modelSun = glm::scale(modelSun, glm::vec3(0.3f)); // Escala el modelo

            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSun));
            glBindVertexArray(VAO);
            //aplicando textura
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sunTexture);
            glUniform1i(glGetUniformLocation(lampshader.Program, "material.diffuse"), 0);
            //Dibujo del modelo de sol
            sun.Draw(lampshader);
            glBindVertexArray(0);
        }

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


/// Moves/alters the camera positions based on user input
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

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
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

    // Para verificar que las variables se actualizan:
    if (keys[GLFW_KEY_L] || keys[GLFW_KEY_O]) {
        std::cout << "Sun rotation: " << sunRotate << ", Moon rotation: " << moonRotate << std::endl;
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
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

    //Botón J y K para cambiar entre día y noche
    if (keys[GLFW_KEY_J])
    {
        night = true;
        day = false;
    }

    if (keys[GLFW_KEY_K])
    {
        night = false;
        day = true;
    }

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
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


