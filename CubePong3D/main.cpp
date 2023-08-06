#define GLFW_INCLUDE_NONE
#include <glfw3.h>
#include <glad/glad.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "functions.h"
#include "Shader.h"
#include "camera.h"
#include "objects.h"

using namespace glm;

const unsigned int windowWidth = 1280, windowHeight = 720;

float deltaTime = 0.0f;
float spacePressTimer = 0.0f;

camera mainCamera;

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
	initGlfw();
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Cube Pong 3D!!! The Game", NULL, NULL);
	checkWindow(window);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	loadGlad();

	mainCamera.lastPosX = windowWidth / 2;
	mainCamera.lastPosY = windowHeight / 2;

	int health = 3;
	unsigned int score = 1;
	float accelerationOfGravity = 0.09f;

	cube plane;
	plane.pos = vec3(0.0f, 0.0f, 0.0f);
	plane.scale = vec3(12.0f ,0.1f, 25.0f);
	plane.color = vec3(0.7f, 0.8f, 1.0f);

	cube rightWall;
	rightWall.scale = vec3(0.5f, 0.5f, plane.scale.z);
	rightWall.pos = vec3(plane.scale.x - rightWall.scale.x, plane.scale.y + rightWall.scale.y, 0.0f);

	cube leftWall;
	leftWall.scale = rightWall.scale;
	leftWall.pos = vec3(plane.scale.x * -1.0f + leftWall.scale.x, plane.scale.y + leftWall.scale.y, 0.0f);
	
	cube myCube;
	myCube.scale = vec3(2.5f, 0.5f, 0.8f);
	myCube.pos = vec3(0.0f, 3.0f, plane.scale.z - myCube.scale.z * 2.0f);
	myCube.color = vec3(0.0f, 0.0f, 0.9f);
	vec3 myCubeStartPos = myCube.pos;
	float myCubeAcceleration = 0.05f;

	cube enemyCube;
	enemyCube.scale = myCube.scale;
	enemyCube.pos = vec3(myCube.pos.x, myCube.pos.y, -plane.scale.z + enemyCube.scale.z * 2.0f);
	enemyCube.color = vec3(myCube.color.z, 0.0f, myCube.color.x);
	vec3 enemyCubeStartPos = enemyCube.pos;
	float enemyCubeAcceleration = 0.1f;

	cube lightCube;
	lightCube.pos = vec3(0.0f, 3.0f, plane.scale.z - myCube.scale.z * 2.0f - 3.5f);
	lightCube.scale = vec3(0.5);
	lightCube.color = vec3(1.0f);
	vec3 lightCubeStartPos = lightCube.pos;
	float lightCubeLastSpeedX = lightCube.speed.x;
	bool lightCubeRespawned = false;

	mainCamera.pos = myCube.pos + vec3(0.0f, 21.0f, 11.5f);
	mainCamera.pitch = -45.0f;
	mainCamera.fov = 50.0f;
	mainCamera.sensitivity = 0.05f;
	mainCamera.speed = 15.0f;
	mainCamera.upDownSpeed = 10.0f;

	mouse_callback(window, mainCamera.yaw, mainCamera.pitch);

	Shader lightingShader("material.vert", "material.frag");
	Shader lampShader("lamp.vert", "lamp.frag");

	obj::initCube();

	mat4 projection = perspective(radians(mainCamera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		setBackgroundColor(0.53f, 0.81f, 0.92f);

		mat4 view = mainCamera.GetViewMatrix();

		lightingShader.use();

		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setVec3("viewPos", mainCamera.pos);

		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLight.ambient", lightCube.color * 0.8f);
		lightingShader.setVec3("dirLight.diffuse", lightCube.color * 0.6f);
		lightingShader.setVec3("dirLight.specular", lightCube.color);

		lightingShader.setVec3("pointLights.position", lightCube.pos);
		lightingShader.setVec3("pointLights.ambient", lightCube.color * 0.4f);
		lightingShader.setVec3("pointLights.diffuse", lightCube.color * 0.7f);
		lightingShader.setVec3("pointLights.specular", lightCube.color);
		lightingShader.setFloat("pointLights.constant", 0.9f);
		lightingShader.setFloat("pointLights.linear", 0.09f);
		lightingShader.setFloat("pointLights.quadratic", 0.032f);

		lightingShader.setVec3("material.ambient", 0.3f, 0.3f, 0.3f);
		lightingShader.setVec3("material.diffuse", 0.55f, 0.55f, 0.55f);
		lightingShader.setVec3("material.specular", 0.1f, 0.1f, 0.1f);
		lightingShader.setFloat("material.shininess", 8.0f);

		lightingShader.setVec3("objectColor", plane.color);
		plane.draw(plane.pos, plane.scale, lightingShader);
		rightWall.draw(rightWall.pos, rightWall.scale, lightingShader);
		leftWall.draw(leftWall.pos, leftWall.scale, lightingShader);

		lightingShader.setVec3("objectColor", myCube.color);
		myCube.draw(myCube.pos, myCube.scale, lightingShader);

		lightingShader.setVec3("objectColor", enemyCube.color);
		enemyCube.draw(enemyCube.pos, enemyCube.scale, lampShader);

		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		lightCube.draw(lightCube.pos, lightCube.scale, lampShader);

		if (health <= 0)
			gameOver(window, "Game Over!");

		if (myCube.pos.y < -20.0f || myCube.pos.y > 20.0f)
			myCube.respawn(myCubeStartPos);
		if (enemyCube.pos.y < -20.0f || enemyCube.pos.y > 20.0f)
			enemyCube.respawn(enemyCubeStartPos);

		unsigned int lastScore = score;

		if ((lightCube.pos.y < -20.0f || lightCube.pos.y > 20.0f) || lightCubeRespawned)
		{	
			if (lightCube.pos.z > plane.scale.z && !lightCubeRespawned)
			{
				health--;
				myCube.respawn(myCubeStartPos);
				lightCube.respawn(lightCubeStartPos);
			}
			else if (lightCube.pos.z < -plane.scale.z || lightCubeRespawned)
			{
				if (!lightCubeRespawned)
				{
					score++;
					lightCubeLastSpeedX = lightCube.speed.x;
					enemyCube.respawn(enemyCubeStartPos);
					lightCube.respawn(vec3(0.0f, 3.0f, -plane.scale.z + enemyCube.scale.z * 2.0f + 3.5f));
				}
				else if (lightCubeRespawned && lightCube.pos.y < plane.scale.y + lightCube.scale.y + 0.1f)
				{
					lightCube.speed = vec3(lightCubeLastSpeedX * 0.5f, 0.0f, 20.0f + (float)score * 0.5f);
					lightCubeRespawned = false;
				}
			}
			else if (!lightCubeRespawned)
				lightCube.respawn(lightCubeStartPos);
		}

		if (score > lastScore)
			lightCubeRespawned = true;

		myCube.allowMovement();
		enemyCube.allowMovement();
		lightCube.allowMovement();

		if (obj::checkCollision(myCube, plane))
		{
			myCube.rigidbody(plane, 1.5f);
			myCube.friction(0.002f);
		}
		if (obj::checkCollision(myCube, rightWall))
		{
			myCube.rigidbody(rightWall, 1.8f);
			myCube.friction(0.002f);
		}
		if (obj::checkCollision(myCube, leftWall))
		{
			myCube.rigidbody(leftWall, 1.8f);
			myCube.friction(0.002f);
		}
		if (obj::checkCollision(enemyCube, plane))
		{
			enemyCube.rigidbody(plane, 1.5f);
			enemyCube.friction(0.002f);
		}
		if (obj::checkCollision(enemyCube, rightWall))
		{
			enemyCube.rigidbody(rightWall, 1.8f);
			enemyCube.friction(0.002f);
		}
		if (obj::checkCollision(enemyCube, leftWall))
		{
			enemyCube.rigidbody(leftWall, 1.8f);
			enemyCube.friction(0.002f);
		}
		if (obj::checkCollision(lightCube, plane))
			lightCube.rigidbody(plane, 1.5f);
		if (obj::checkCollision(lightCube, rightWall))
			lightCube.rigidbody(rightWall, 1.9f);
		if (obj::checkCollision(lightCube, leftWall))
			lightCube.rigidbody(leftWall, 1.9f);
		if (obj::checkCollision(lightCube, enemyCube))
			lightCube.rigidbody(enemyCube, 2.0f + (float)score * 0.025f);
		if (obj::checkCollision(lightCube, myCube))
			lightCube.rigidbody(myCube, 1.95f);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && myCube.allowMovementRight)
			myCube.speed.x += myCubeAcceleration;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && myCube.allowMovementLeft)
			myCube.speed.x -= myCubeAcceleration;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && myCube.allowMovementBack)
			myCube.speed.z -= myCubeAcceleration;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && myCube.allowMovementFront)
			myCube.speed.z += myCubeAcceleration;

		if (myCube.pos.z <= plane.scale.z - plane.scale.z * 0.45f)
			myCube.allowMovementBack = false;
		else if (myCube.pos.z >= plane.scale.z - myCube.scale.z)
			myCube.allowMovementFront = false;

		if (lightCube.speed.z < 0.0f)
		{
			if (lightCube.pos.x > enemyCube.pos.x && enemyCube.allowMovementRight)
			{
				enemyCube.speed.x += enemyCubeAcceleration * score;
				enemyCube.allowMovementLeft = false;
			}
			else if (lightCube.pos.x < enemyCube.pos.x && enemyCube.allowMovementLeft)
			{
				enemyCube.speed.x -= enemyCubeAcceleration * score;
				enemyCube.allowMovementRight = false;
			}
		}

		deltaTime = getDeltaTime();

		myCube.movement(accelerationOfGravity, deltaTime);
		enemyCube.movement(accelerationOfGravity, deltaTime);
		lightCube.movement(accelerationOfGravity, deltaTime);

		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	obj::clearBuffers();
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//mainCamera.keyboardProcess(window, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (mainCamera.firstMouse)
	{
		mainCamera.lastPosX = (float)xpos;
		mainCamera.lastPosY = (float)ypos;
		mainCamera.firstMouse = false;
	}

	float xoffset = (float)xpos - mainCamera.lastPosX;
	float yoffset = mainCamera.lastPosY - (float)ypos;
	mainCamera.lastPosX = (float)xpos;
	mainCamera.lastPosY = (float)ypos;

	xoffset *= mainCamera.sensitivity;
	yoffset *= mainCamera.sensitivity;

	mainCamera.yaw += xoffset;
	mainCamera.pitch += yoffset;

	if (mainCamera.pitch > 89.0f)
		mainCamera.pitch = 89.0f;
	if (mainCamera.pitch < -89.0f)
		mainCamera.pitch = -89.0f;

	vec3 direction;
	direction.x = cos(radians(mainCamera.yaw)) * cos(radians(mainCamera.pitch));
	direction.y = sin(radians(mainCamera.pitch));
	direction.z = sin(radians(mainCamera.yaw)) * cos(radians(mainCamera.pitch));
	mainCamera.front = normalize(direction);
}
