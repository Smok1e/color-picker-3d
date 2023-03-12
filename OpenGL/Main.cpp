#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

#include "Shader.hpp"
#include "Sphere.hpp"
#include "Cube.hpp"
#include "Utils.hpp"

//-----------------------------------

constexpr bool FullscreenMode = false;
constexpr float CameraSpeed = .05f;
constexpr float CameraRotationSpeed = .1f;

glm::vec3 WorldAxisY    (0, 1,  0);
glm::vec3 CameraPosition(0, 0,  3);
glm::vec3 CameraAxisX   (1, 0,  0);
glm::vec3 CameraAxisY   (0, 1,  0);
glm::vec3 CameraAxisZ   (0, 0, -1);
glm::vec2 CameraRotation(0, 0);
						    
//-----------------------------------

void GLFWErrorCallback(int error, const char* description);
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFWCursorPosCallback(GLFWwindow* window, double x, double y);
void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void CheckOpenGLError(const char* func, int line);

//-----------------------------------

int main()
{
	if (!glfwInit())
	{
		printf("GLFW Initialization failed\n");
		return 0;
	}

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videomode = glfwGetVideoMode(monitor);

	glm::ivec2 monitor_size(videomode->width, videomode->height);
	glm::ivec2 window_size = static_cast<glm::vec2>(monitor_size)*(FullscreenMode? 1.f: .7f);

	float aspect_ratio = static_cast<float>(window_size.x)/static_cast<float>(window_size.y);
	constexpr float fov = glm::radians(45.f);

	// Initializing window
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwSetErrorCallback(GLFWErrorCallback);

	GLFWwindow* window = glfwCreateWindow(window_size.x, window_size.y, "OpenGL Test", FullscreenMode? monitor: nullptr, nullptr);
	if (!window)
	{
		printf("Window creation failed\n");
		return 0;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, GLFWKeyCallback);
	glfwSetCursorPosCallback(window, GLFWCursorPosCallback);
	glfwSetMouseButtonCallback(window, GLFWMouseButtonCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwFocusWindow(window);
	glfwSetWindowPos(window, monitor_size.x/2 - window_size.x/2, monitor_size.y/2 - window_size.y/2);

	// Initializing OpenGL
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initalization failed\n");
		return false;
	}

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_ALWAYS); 

	Shader shader;
	if (!shader.loadFromFile("shader.vert", "shader.frag"))
	{
		printf("Failed to load shader\n");
		return 0;
	}

	Sphere sphere1;
	Sphere sphere2;
	Cube cube;
	sphere2.setPosition(glm::vec3(1, 1, 1));

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(.05, .05, .05, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float speed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS? 2: 1;
		if (glfwGetKey(window, GLFW_KEY_D         ) == GLFW_PRESS) CameraPosition += CameraSpeed * speed * CameraAxisX;
		if (glfwGetKey(window, GLFW_KEY_A         ) == GLFW_PRESS) CameraPosition -= CameraSpeed * speed * CameraAxisX;
		if (glfwGetKey(window, GLFW_KEY_SPACE     ) == GLFW_PRESS) CameraPosition += CameraSpeed * speed * CameraAxisY;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) CameraPosition -= CameraSpeed * speed * CameraAxisY;
		if (glfwGetKey(window, GLFW_KEY_W         ) == GLFW_PRESS) CameraPosition += CameraSpeed * speed * CameraAxisZ;
		if (glfwGetKey(window, GLFW_KEY_S         ) == GLFW_PRESS) CameraPosition -= CameraSpeed * speed * CameraAxisZ;

		glm::mat4 projection = glm::perspective(fov, aspect_ratio, 0.f, 100.f);
		glm::mat4 view = glm::lookAt(CameraPosition, CameraPosition+CameraAxisZ, CameraAxisY);
		glm::mat4 transform = projection*view;

		shader.use();
		shader.setUniform("transform", transform);
		sphere1.draw();
		//sphere2.draw();
		//cube.draw();

		glfwSwapBuffers(window);
	}
						   
	glfwTerminate();
}

//-----------------------------------

void GLFWErrorCallback(int error, const char* description)
{
	printf("GLFW Error: %s\n", description);
}

void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			break;

		case GLFW_KEY_R:
			if (action == GLFW_PRESS)
			{
				CameraPosition = glm::vec3(0, 0, 3);
				CameraRotation = glm::vec2(0, 0);
				CameraAxisZ = glm::vec3(0, 0, -1);
			}

			break;

		case GLFW_KEY_X:
			if (action == GLFW_PRESS)
			{
				GLint current_polygon_mode[2] = {};
				glGetIntegerv(GL_POLYGON_MODE, current_polygon_mode);
				glPolygonMode(GL_FRONT_AND_BACK, *current_polygon_mode == GL_LINE? GL_FILL: GL_LINE);
			}

			break;
	}
}

void GLFWCursorPosCallback(GLFWwindow* window, double x, double y)
{
	int window_size_x = 0, window_size_y = 0;
	glfwGetWindowSize(window, &window_size_x, &window_size_y);
	glm::vec2 window_center(window_size_x/2, window_size_y/2);
	glm::vec2 delta = glm::vec2(x, y) - window_center;
	glfwSetCursorPos(window, window_center.x, window_center.y);
	
	CameraRotation += delta*glm::vec2(1, -1)*CameraRotationSpeed;

	glm::vec3 camera_axis_z = glm::normalize(glm::vec3(
		cos(glm::radians(CameraRotation.y)) * cos(glm::radians(CameraRotation.x)),
		sin(glm::radians(CameraRotation.y)),
		cos(glm::radians(CameraRotation.y)) * sin(glm::radians(CameraRotation.x))
	));

	CameraAxisZ = camera_axis_z;
	CameraAxisX = glm::normalize(glm::cross(CameraAxisZ, WorldAxisY ));
	CameraAxisY = glm::normalize(glm::cross(CameraAxisX, CameraAxisZ));
}

void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// ...
}

//-----------------------------------