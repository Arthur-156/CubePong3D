#pragma once

using namespace std;

float result = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (result == 0.0f)
		result = (float)width / (float)height;
		
	if (width > height)
		glViewport((width / 2) - (((float)height * result) / 2), 0, (float)height * result, height);
	else
		glViewport(0, (height / 2) - ((float(width) * (1.0f / result)) / 2), width, float(width) * (1.0f / result));
}

void initGlfw()
{
	if (!glfwInit())
	{
		cout << "GLFW initialization failed" << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "GLFW initialization successful" << endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
}

void checkWindow(GLFWwindow* window)
{
	if (!window)
	{
		cout << "Window or context creation failed" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "Window created successfully" << endl;
	}
}

void loadGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "GLAD initialization failed" << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "GLAD initialization successful\n" << endl;
	}
}

void setBackgroundColor(float r, float g, float b) 
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float deltatime = 0.0f, lastframe = 0.0f;

float getDeltaTime()
{
	float currentFrame = (float)glfwGetTime();
	deltatime = currentFrame - lastframe;
	lastframe = currentFrame;

	return deltatime;
}

void gameOver(GLFWwindow* window, const char* text)
{
	cout << "\n    --====-* " << text << " *-====--\n" << endl;
	glfwSetWindowShouldClose(window, true);
}