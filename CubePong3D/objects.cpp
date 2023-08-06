#include "objects.h"

float vertices[] = {
	-1.0f,-1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	 1.0f,-1.0f, 1.0f, 	 1.0f, 0.0f, 0.0f,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f, 1.0f,

	-1.0f,-1.0f,-1.0f,  -1.0f, 0.0f, 0.0f,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,	 1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,   1.0f, 0.0f, 0.0f,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f,-1.0f
};

unsigned int indices[] = {
	0, 1, 2,  0, 2, 3,
	3, 2, 6,  3, 6, 7,
	4, 5, 1,  4, 1, 0,
	7, 6, 5,  7, 5, 4,
	1, 5, 6,  1, 6, 2,
	4, 0, 3,  4, 3, 7
};

unsigned int vao, vbo, ibo;

void cube::draw(Shader shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, nullptr);
}

void cube::draw(glm::vec3 pos, glm::vec3 scale, Shader shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);
	shader.setMat4("model", model);

	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, nullptr);
}

void cube::allowMovement()
{
	allowMovementFront = true;
	allowMovementBack = true;
	allowMovementLeft = true;
	allowMovementRight = true;
	allowMovementUp = true;
	allowMovementDown = true;
}

void cube::moveFront(float speed, float deltaTime)
{
	pos -= (speed * deltaTime) * glm::vec3(0.0f, 0.0f, -1.0f);
}

void cube::moveBack(float speed, float deltaTime)
{
	pos += (speed * deltaTime) * glm::vec3(0.0f, 0.0f, -1.0f);
}

void cube::moveLeft(float speed, float deltaTime)
{
	pos -= (speed * deltaTime) * glm::vec3(1.0f, 0.0f, 0.0f);
}

void cube::moveRight(float speed, float deltaTime)
{
	pos += (speed * deltaTime) * glm::vec3(1.0f, 0.0f, 0.0f);
}

void cube::moveUp(float speed, float deltaTime)
{
	pos += (speed * deltaTime) * glm::vec3(0.0f, 1.0f, 0.0f);
}

void cube::moveDown(float speed, float deltaTime)
{
	pos -= (speed * deltaTime) * glm::vec3(0.0f, 1.0f, 0.0f);
}

void cube::movement(float accelerationOfGravity, float deltaTime)
{
	if (allowMovementLeft && speed.x < 0.0f)
		moveLeft(speed.x * -1.0f, deltaTime);
	if (allowMovementRight && speed.x > 0.0f)
		moveRight(speed.x, deltaTime);
	if (allowMovementDown)
	{
		speed.y -= accelerationOfGravity;
		if (speed.y < 0.0f)
			moveDown(speed.y * -1.0f, deltaTime);
	}
	if (allowMovementUp && speed.y > 0.0f)
		moveUp(speed.y, deltaTime);
	if (allowMovementBack && speed.z < 0.0f)
		moveBack(speed.z * -1.0f, deltaTime);
	if (allowMovementFront && speed.z > 0.0f)
		moveFront(speed.z, deltaTime);
	
}

void cube::rigidbody(cube obj2, float rebound)
{
	if (pos.y - (scale.y - 0.1f) < obj2.pos.y + (obj2.scale.y - 0.1f))
	{
		if (pos.x + (scale.x - 0.1f) < obj2.pos.x - (obj2.scale.x - 0.1f))
		{
			allowMovementRight = false;
			speed.x -= (speed.x - obj2.speed.x) * rebound;
			speed.z += obj2.speed.z;
		}
		if (pos.x - (scale.x - 0.1f) > obj2.pos.x + (obj2.scale.x - 0.1f))
		{
			allowMovementLeft = false;
			speed.x += ((speed.x * -1) + obj2.speed.x) * rebound;
			speed.z += obj2.speed.z;
		}
		if (pos.z + (scale.z - 0.1f) < obj2.pos.z - (obj2.scale.z - 0.1f))
		{
			allowMovementFront = false;
			speed.z -= (speed.z - obj2.speed.z) * rebound;
			if(obj2.speed.z != 0.0f)
				speed.x += obj2.speed.x;
		}
		if (pos.z - (scale.z - 0.1f) > obj2.pos.z + (obj2.scale.z - 0.1f))
		{
			allowMovementBack = false;
			speed.z += ((speed.z * -1) + obj2.speed.z) * rebound;
			if (obj2.speed.z != 0.0f)
				speed.x += obj2.speed.x;
		}
	}
	if (pos.y + (scale.y - 0.1f) < obj2.pos.y - (obj2.scale.y - 0.1f))
	{
		//allowMovementUp = false;
		speed.y -= (speed.y - obj2.speed.y) * rebound;
	}
	if (pos.y - (scale.y - 0.1f) > obj2.pos.y + (obj2.scale.y - 0.1f))
	{
		allowMovementDown = false;
		speed.y += ((speed.y * -1) + obj2.speed.y) * rebound;
	}
}

void cube::friction(float strength)
{
	if (speed.x < 0.0f)
		speed.x -= speed.x * strength;
	else if (speed.x > 0.0f)
		speed.x -= speed.x * strength;
	if (speed.x < strength && speed.x > strength * -1)
		speed.x = 0.0f;

	if (speed.y < 0.0f)
		speed.y -= speed.y * strength;
	else if (speed.y > 0.0f)
		speed.y -= speed.y * strength;
	if (speed.y < strength && speed.y > strength * -1)
		speed.y = 0.0f;

	if (speed.z < 0.0f)
		speed.z -= speed.z * strength;
	else if (speed.z > 0.0f)
		speed.z -= speed.z * strength;
	if (speed.z < strength && speed.z > strength * -1)
		speed.z = 0.0f;
}

void cube::respawn(glm::vec3 pos)
{
	cube::pos = pos;
	speed = glm::vec3(0.0f);
}

void obj::initCube()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);
}

void obj::clearBuffers()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

bool obj::checkCollision(cube obj1, cube obj2)
{
	if (obj1.collisionOn && obj2.collisionOn)
	{
		bool collisionX = obj1.pos.x + (obj1.scale.x + 0.1f) >= obj2.pos.x - (obj2.scale.x + 0.1f) &&
			obj2.pos.x + (obj2.scale.x + 0.1f) >= obj1.pos.x - (obj1.scale.x + 0.1f);
		bool collisionY = obj1.pos.y + obj1.scale.y >= obj2.pos.y - obj2.scale.y &&
			obj2.pos.y + obj2.scale.y >= obj1.pos.y - obj1.scale.y;
		bool collisionZ = obj1.pos.z + obj1.scale.z >= obj2.pos.z - obj2.scale.z &&
			obj2.pos.z + obj2.scale.z >= obj1.pos.z - obj1.scale.z;

		return collisionX && collisionY && collisionZ;
	}
	else 
		return false;
}
