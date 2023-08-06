#include "camera.h"

camera::camera()
{

}

void camera::moveFront(float deltaTime)
{
	pos += (speed * deltaTime) * front;
}

void camera::moveBack(float deltaTime)
{
	pos -= (speed * deltaTime) * front;
}

void camera::moveLeft(float deltaTime)
{
	pos -= normalize(cross(front, up)) * (speed * deltaTime);
}

void camera::moveRight(float deltaTime)
{
	pos += normalize(cross(front, up)) * (speed * deltaTime);
}

void camera::moveUp(float deltaTime)
{
	pos += (upDownSpeed * deltaTime) * up;
}

void camera::moveDown(float deltaTime)
{
	pos -= (upDownSpeed * deltaTime) * up;
}

void camera::keyboardProcess(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveFront(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveBack(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveLeft(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveRight(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		moveUp(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		moveDown(deltaTime);
}
