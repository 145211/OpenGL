#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderprogram.h"
#include "constants.h"
#include "Texture.h"
#include "Vertex.h"
#include "Model.h"
#include "Vertex.h"
#include "Object.h"
#include "Entity.h"

#include "Collisions.h"

#include "OBJLoader.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <vector>

#include "myCube.h"

float aspectRatio = 16.f/9.f;
float movementSpeed = 0.05f;
float sprint = 0.1f;
float sensitivity = 0.1f;
double cursorxpos = 0, cursorypos = 0;
bool firstMouse = true;

glm::vec4 pressedKeys = glm::vec4(0,0,0,0);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 startPos = glm::vec3(11.5f, 3.0f, -30.0f);
glm::vec3 playerPos = startPos;
glm::vec3 moveVec = glm::vec3(0.0f);

//Texture tex(0, GL_TEXTURE_2D);

float yaw = 90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;

ShaderProgram* sp, *skyboxsp;

vector<Entity*> entities;

vector<Collisions> objs;

float ambientPwr = 0.8;
vec3 lightColor = vec3(1, 0.95, 0.95);
vec4 ambientLight = vec4(lightColor * ambientPwr, 1);


//Error handling
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Keyboard handilng
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) pressedKeys[0] = 1;
		if (key == GLFW_KEY_D) pressedKeys[1] = 1;
		if (key == GLFW_KEY_W) pressedKeys[2] = 1;
		if (key == GLFW_KEY_S) pressedKeys[3] = 1;
		if (key == GLFW_KEY_R) playerPos = startPos;
		if (key == GLFW_KEY_LEFT_SHIFT) movementSpeed += sprint;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) pressedKeys[0] = 0;
		if (key == GLFW_KEY_D) pressedKeys[1] = 0;
		if (key == GLFW_KEY_W) pressedKeys[2] = 0;
		if (key == GLFW_KEY_S) pressedKeys[3] = 0;
		if (key == GLFW_KEY_LEFT_SHIFT) movementSpeed -= sprint;
	}
}

//Mouse handling
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		//int width, height;
		//glfwGetWindowSize(window, &width, &height);
		cursorxpos = xpos;
		cursorypos = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - cursorxpos;
	float yoffset = cursorypos - ypos; // reversed since y-coordinates go from bottom to top
	cursorxpos = xpos;
	cursorypos = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Load into computer's memory
	std::vector<unsigned char> image;   //Allocate a vector for image storage
	unsigned width, height;   //Variables for image size
	//Read image
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import into graphics card's memory
	glGenTextures(1, &tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate the handle
	//Import image into graphics card's memory associated with the handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

GLuint loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	unsigned width, height;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		std::vector<unsigned char> image;
		unsigned error = lodepng::decode(image, width, height, faces[i].c_str());
		if (image.size())
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
			image.clear();
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			image.clear();
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // automatycznie centruje kursor w aplikacji oraz go ukrywa
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	//tex.loadTexture("textures\\Red_Marble_002\\Red_Marble_002_COLOR.png");
	sp = new ShaderProgram("vertex_shader.glsl", NULL, "fragment_shader.glsl");
	//skyboxsp = new ShaderProgram("skybox_vertex_shader.glsl", NULL, "skybox_fragment_shader.glsl");
	
	sp->use();
	glUniform4f(sp->u("ambientLight"), ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);

	objs = collisionInit(objs);
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

	delete sp;
}

//Vertex* loadArrayToVertexArray(float* vertices, float* normals, float* colors, float* texCoords, const unsigned arrSize) {
//	Vertex* varr = new Vertex[arrSize];
//	for (unsigned i = 0; i < arrSize; ++i) {
//		varr[i].position = glm::vec4(vertices[4 * i], vertices[4 * i + 1], vertices[4 * i + 2], vertices[4 * i + 3]);
//		varr[i].normal = glm::vec4(normals[4 * i], normals[4 * i + 1], normals[4 * i + 2], normals[4 * i + 3]);
//		varr[i].color = glm::vec4(colors[4 * i], colors[4 * i + 1], colors[4 * i + 2], colors[4 * i + 3]);
//		varr[i].texCoord = glm::vec2(texCoords[2 * i], texCoords[2 * i + 1]);
//	}
//
//	return varr;
//}

void drawScene(GLFWwindow* window, float angle_x, float angle_y, glm::vec3& playerPos, vector<Entity*> entities) {
	//************Tutaj umieszczaj kod rysujący obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Obliczanie kolizji z obiektami
	//moveVec.xz = Collide(playerPos.xz, moveVec.xz, objs);
	playerPos += moveVec;

	glm::mat4 V = glm::lookAt(
		playerPos,
		playerPos + cameraFront,
		glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 100.0f); //Wylicz macierz rzutowania

	glUniform4f(sp->u("ambientLight"), ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);

	(*entities[0]).drawEntity(P, V);

	(*entities[1]).accessModel().setPosition(vec3(0, 1, -10));
	(*entities[1]).drawEntity(P, V);
	
	(*entities[2]).accessModel().setPosition(vec3(20, 1, -10));
	(*entities[2]).drawEntity(P, V);


	//otest.activateShader();
	//glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	//glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	//glUniform3fv(sp->u("playerPos"), 1, glm::value_ptr(playerPos));

	////otest.setRotation(glm::vec3(angle_x, angle_y, 0));
	//tex.bindTexture();
	//otest.render();

	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}

void floorLevel(){
	if (playerPos.z > -2.5)
		playerPos.y = 3;
	else if (playerPos.z > -4.5)
		playerPos.y = 2.f + ((playerPos.z + 4.5f)/2.f);
	else 
		playerPos.y = 2;
}

void movementKeys()
{
	glm::vec2 horiVec;
	glm::vec2 vertVec;
	
	horiVec = glm::normalize(glm::cross(cameraFront, cameraUp)).xz * (pressedKeys[1] - pressedKeys[0]);

	vertVec = cameraFront.xz * (pressedKeys[2] - pressedKeys[3]);
	
	if (horiVec + vertVec == glm::vec2(0, 0))
		moveVec.xz = glm::vec3(0, 0, 0);
	else
		moveVec.xz = normalize(horiVec + vertVec) * movementSpeed;

	if (pressedKeys == glm::vec4(0,0,0,0)) moveVec = glm::vec3(0.0f);
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback); //Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	std::cout << glGetString(GL_VERSION) << std::endl;

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	// Wczytywanie tekstur
	Texture* tex1 = new Texture("textures\\Red_Marble_002\\Red_Marble_002_COLOR.png", GL_TEXTURE_2D, 0);
	Texture* tex2 = new Texture("textures\\Marble_White_006_SD\\Marble_White_006_basecolor.png", GL_TEXTURE_2D, 0);

	// Wczytywanie modeli assimpem
	Model model1; 
	model1.assimpLoadModel("Pantheon_even_smaller.obj");
	Entity ent1(tex1, model1, sp);
	entities.push_back(&ent1);

	Model model2;
	model2.assimpLoadModel("Monument_test.obj");
	Entity ent2(tex2, model2, sp);
	entities.push_back(&ent2);
	
	Model model3;
	model3.assimpLoadModel("Venus_de_Milo.obj");
	Entity ent3(tex2, model3, sp);
	entities.push_back(&ent3);

	Entity::playerPos = &playerPos;

	//Główna pętla
	float angle_x = 0; //Aktualny kąt obrotu obiektu
	float angle_y = 0; //Aktualny kąt obrotu obiektu
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle_x += 1 * glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
		angle_y += 1 * glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
		
		glfwSetTime(0); //Zeruj timer
		drawScene(window, angle_x, angle_y, playerPos, entities); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.

		movementKeys();
		floorLevel();

		//print player position
		printf("%f, %f, %f\n", playerPos.x, playerPos.y, playerPos.z);
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}