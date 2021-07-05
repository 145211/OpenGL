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
float movementSpeed = 0.08f;
float sprint = 0.14f;
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

float yaw = 90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;

ShaderProgram* sp, *skyboxsp, *terrainsp;

unsigned int cubemapTexture;

vector<Entity*> entities;

vector<Collisions> objs;

float ambientPwr = 0.025;
float pwr = 0.05;

vec3 attenuation = vec3(1.0, 0.14, 0.07);

#define noPointLights 6

vec4 lp[noPointLights] = {
	vec4(4.3, 2, -0.8, 1),
	vec4(19.3, 2, -0.8, 1),

	vec4(18.5, 2, 24.25, 1),
	vec4(4.5, 2, 24.25, 1),

	vec4(11.15, 0.25, -52.9, 1),

	vec4(playerPos, 1)
};

std::vector<std::string> skyboxFaces{
	"textures\\skybox\\right.png",
	"textures\\skybox\\left.png",
	"textures\\skybox\\top.png",
	"textures\\skybox\\bottom.png",
	"textures\\skybox\\front.png",
	"textures\\skybox\\back.png" };

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
		if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, 1);
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
	skyboxsp = new ShaderProgram("skybox_vertex_shader.glsl", NULL, "skybox_fragment_shader.glsl");
	terrainsp = new ShaderProgram("grass_vertex_shader.glsl", NULL, "grass_fragment_shader.glsl");

	objs = collisionInit(objs);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

	delete sp;
}

void drawScene(GLFWwindow* window, float angle_x, float angle_y, glm::vec3& playerPos, vector<Entity*> entities, Entity& skybox) {
	//************Tutaj umieszczaj kod rysujący obraz******************
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Obliczanie kolizji z obiektami
	moveVec.xz = Collide(playerPos.xz, moveVec.xz, objs);
	playerPos += moveVec;

	glm::mat4 V = glm::lookAt(
		playerPos,
		playerPos + cameraFront,
		glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 100.0f); //Wylicz macierz rzutowania

	sp->setVec4f(vec4(cameraFront, 0.0), "camFront");
	sp->set1f(glm::cos(glm::radians(5.0f)), "cutoff");
	sp->set1f(glm::cos(glm::radians(15.0f)), "outerCutoff");

	sp->set1f(ambientPwr, "dirLight.ambientPwr");
	sp->setVec4f(vec4(1, 0.95, 0.95, 1.0), "dirLight.ambientColor");
	sp->set1f(pwr, "dirLight.pwr");
	sp->setVec4f(vec4(0.5, 0.5, 0.85, 1.0), "dirLight.lightColor");
	sp->setVec4f(vec4(-0.1, -1.0, -0.1, 1.0), "dirLight.direction");

	for (int i = 0; i < noPointLights - 1; i++)
	{
		sp->setVec4f(lp[i], ("pointLights[" + std::to_string(i) + "].pos").c_str());

		sp->setVec4f(vec4(1, 0.95, 0.95, 1.0), ("pointLights[" + std::to_string(i) + "].lightColor").c_str());
		sp->set1f(attenuation[0], ("pointLights[" + std::to_string(i) + "].constant").c_str());
		sp->set1f(attenuation[1], ("pointLights[" + std::to_string(i) + "].linear").c_str());
		sp->set1f(attenuation[2], ("pointLights[" + std::to_string(i) + "].quadratic").c_str());
	}

	//Fleshlight color
	sp->setVec4f(vec4(1, 1, 0.9, 1.0), ("pointLights[" + std::to_string(noPointLights - 1) + "].lightColor").c_str());
	sp->set1f(1.0, ("pointLights[" + std::to_string(noPointLights - 1) + "].constant").c_str());
	sp->set1f(0.045, ("pointLights[" + std::to_string(noPointLights - 1) + "].linear").c_str());
	sp->set1f(0.0075, ("pointLights[" + std::to_string(noPointLights - 1) + "].quadratic").c_str());

	glDepthMask(GL_FALSE);
	(*entities[0]).drawEntity(P, glm::mat4(glm::mat3(V)), Entity::drawType::NORMAL);
	glDepthMask(GL_TRUE);

	(*entities[1]).drawEntity(P, V, Entity::drawType::NORMAL); // Świątynia
	(*entities[2]).drawEntity(P, V, Entity::drawType::NORMAL); // Test
	(*entities[3]).drawEntity(P, V, Entity::drawType::NORMAL); // Venus
	(*entities[4]).drawEntity(P, V, Entity::drawType::NORMAL); // Donut
	(*entities[4]).accessModel().rotate(glm::vec3(glm::radians(0.5), glm::radians(1.0), glm::radians(0.25)));
	
	(*entities[5]).drawEntity(P, V, Entity::drawType::INSTANCED); // Terrain
	(*entities[6]).drawEntity(P, V, Entity::drawType::NORMAL); // Vase 1
	(*entities[6]).accessModel().rotate(glm::vec3(0, glm::radians(-0.5), 0));
	(*entities[7]).drawEntity(P, V, Entity::drawType::NORMAL); // Vase 2
	(*entities[7]).accessModel().rotate(glm::vec3(0, glm::radians(0.5), 0));
	//(*entities[3]).accessModel().rotate(glm::vec3(0, glm::radians(10.0), 0));

	//glUniform3fv(sp->u("playerPos"), 1, glm::value_ptr(playerPos));



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
	Texture* skyboxTex = new Texture(GL_TEXTURE_CUBE_MAP, 0);
	skyboxTex->loadCubemap(skyboxFaces);

	Texture* tex1  = new Texture("textures\\Red_Marble_002\\COLOR.png", GL_TEXTURE_2D, 0);
	Texture* spec1 = new Texture("textures\\Red_Marble_002\\SPEC.png", GL_TEXTURE_2D, 1);

	Texture* tex2  = new Texture("textures\\Metal\\COLOR.png", GL_TEXTURE_2D, 0);
	Texture* spec2 = new Texture("textures\\Metal\\SPEC.png", GL_TEXTURE_2D, 1);

	Texture* VenusT = new Texture("Venus_de_Milo.png", GL_TEXTURE_2D, 0);
	Texture* terrainT = new Texture("textures\\grass.png", GL_TEXTURE_2D, 0);

	Texture* randomMap = new Texture("random_map.png", GL_TEXTURE_2D, 1);

	Texture* vaseT = new Texture("textures\\vase.png", GL_TEXTURE_2D, 0);

	// Wczytywanie modeli assimpem
	Model skyboxModel;
	skyboxModel.assimpLoadModel("cube.obj");
	Entity skybox(skyboxTex, skyboxTex, skyboxModel, skyboxsp);
	entities.push_back(&skybox);

	Model model0; 
	model0.assimpLoadModel("Pantheon_even_smaller.obj");
	Entity ent0(tex1, spec1, model0, sp);
	entities.push_back(&ent0);

	Model model1;
	model1.assimpLoadModel("Monument_test.obj");
	Entity ent1(tex1, spec1, model1, sp);
	ent1.accessModel().setPosition(glm::vec3(0, 1, -10));
	entities.push_back(&ent1);
	
	Model model2;
	model2.assimpLoadModel("Venus_de_Milo.obj");
	Entity ent2(VenusT, spec1, model2, sp);
	ent2.accessModel().setPosition(glm::vec3(11.85, 1, 24.25));
	ent2.accessModel().setScaling(glm::vec3(1.3, 1.3, 1.3));
	ent2.accessModel().setRotation(glm::vec3(0, glm::radians(180.0), 0));
	entities.push_back(&ent2);

	Model donutM;
	donutM.assimpLoadModel("donut.obj");
	Entity donut(tex1, spec1, donutM, sp);
	donut.accessModel().setScaling(glm::vec3(30, 30, 30));
	donut.accessModel().setRotation(glm::vec3(glm::radians(90.0), 0, 0));
	donut.accessModel().setPosition(glm::vec3(11.15, 2, -53.7));
	entities.push_back(&donut);

	Model terrainM;
	terrainM.assimpLoadModel("terrain.obj");
	Entity terrain(terrainT, randomMap, terrainM, terrainsp);
	terrain.accessModel().setRotation(glm::vec3(0, 0, glm::radians(180.0)));
	terrain.accessModel().setPosition(glm::vec3(0, -2.2, 0));
	terrain.accessModel().setScaling(glm::vec3(2, 3, 3));
	entities.push_back(&terrain);

	Model vaseM;
	vaseM.assimpLoadModel("vase.obj");
	Entity vase1(vaseT, spec1, vaseM, sp);
	Entity vase2(vaseT, spec1, vaseM, sp);
	vase1.accessModel().setPosition(glm::vec3(4.3, 0.2, 19.5));
	vase2.accessModel().setPosition(glm::vec3(18.7, 0.2, 19.5));
	//vase1.accessModel().setScaling(glm::vec3(1, 1, 1));
	entities.push_back(&vase1);
	entities.push_back(&vase2);

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
		drawScene(window, angle_x, angle_y, playerPos, entities, skybox); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.

		movementKeys();
		floorLevel();

		//print player position
		printf("%f, %f, %f\n", playerPos.x, playerPos.y, playerPos.z);
		//printf("%f, %f, %f\n", cameraFront.x, cameraFront.y, cameraFront.z);
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}