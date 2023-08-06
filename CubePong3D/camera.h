#ifndef CAMERA_H
#define CAMERA_H

#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera
{
private:

public:
	bool firstMouse = true;
	float lastPosX = 0.0f, lastPosY = 0.0f;

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;

	float fov = 45.0f;

	float sensitivity = 0.1f;

	float speed = 1.0f;
	float upDownSpeed = speed;

	camera();

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(pos, pos + front, up);
	}

	void moveFront(float deltaTime);
	void moveBack(float deltaTime);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
	void moveUp(float deltaTime);
	void moveDown(float deltaTime);

	void keyboardProcess(GLFWwindow* window, float deltaTime);
};
#endif

