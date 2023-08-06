#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


class cube
{
public:

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	//glm::vec3 angle = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 color = glm::vec3(1.0f);

	//glm::vec3 colliderSize = scale;
	//glm::vec3 colliderPos = pos;
	bool collisionOn = true;
	
	glm::vec3 speed = glm::vec3(0.0f, 0.0f, 0.0f);

	bool allowMovementFront = true;
	bool allowMovementBack = true;
	bool allowMovementLeft = true;
	bool allowMovementRight = true;
	bool allowMovementUp = true;
	bool allowMovementDown = true;

	void draw(Shader shader);
	void draw(glm::vec3 pos, glm::vec3 scale, Shader shader);

	void allowMovement();

	void moveFront(float speed, float deltaTime);
	void moveBack(float speed, float deltaTime);
	void moveLeft(float speed, float deltaTime);
	void moveRight(float speed, float deltaTime);
	void moveUp(float speed, float deltaTime);
	void moveDown(float speed, float deltaTime);

	void movement(float accelerationOfGravity, float deltaTime);
	void rigidbody(cube obj2, float rebound);
	void friction(float strength);

	void respawn(glm::vec3 pos);
};

namespace obj
{
	void initCube();
	void clearBuffers();
	bool checkCollision(cube obj1, cube obj2);
}