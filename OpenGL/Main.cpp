#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Shader.hpp"
#include "Sphere.hpp"
#include "Cube.hpp"
#include "Utils.hpp"

//-----------------------------------

constexpr bool FullscreenMode = true;
constexpr float CameraSpeed = .05f;
constexpr float CameraRotationSpeed = .1f;
constexpr float Dampling = .9f;

glm::vec3 WorldAxisY    (0, 1,  0);
glm::vec3 CameraPosition(0, 0,  3);
glm::vec3 CameraAxisX   (1, 0,  0);
glm::vec3 CameraAxisY   (0, 1,  0);
glm::vec3 CameraAxisZ   (0, 0, -1);
glm::vec2 CameraRotation(0, 0);

bool InterfaceVisible = false;
						    
//-----------------------------------

void DrawInterface();

void GLFWErrorCallback(int error, const char* description);
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFWCursorPosCallback(GLFWwindow* window, double x, double y);
void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// Measures cursor offset relative to window's center and puts cursor in center of window
glm::vec2 CenterCursor(GLFWwindow* window);

// Sets interface visibility state according to parameter; It also moves cursor back to center if interface becomes visible
void SetInterfaceVisible(GLFWwindow* window, bool visible);

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
	glfwFocusWindow(window);
	glfwSetWindowPos(window, monitor_size.x/2 - window_size.x/2, monitor_size.y/2 - window_size.y/2);

	// Initializing interface
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");															
	SetInterfaceVisible(window, false);

	// Initializing graphics
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

	glm::vec3 CameraVelocity(0, 0, 0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		DrawInterface();

		glClearColor(.05, .05, .05, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float speed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS? .2f: .1f;
		if (glfwGetKey(window, GLFW_KEY_D         ) == GLFW_PRESS) CameraVelocity.x += speed;
		if (glfwGetKey(window, GLFW_KEY_A         ) == GLFW_PRESS) CameraVelocity.x -= speed;
		if (glfwGetKey(window, GLFW_KEY_SPACE     ) == GLFW_PRESS) CameraVelocity.y += speed;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) CameraVelocity.y -= speed;
		if (glfwGetKey(window, GLFW_KEY_W         ) == GLFW_PRESS) CameraVelocity.z += speed;
		if (glfwGetKey(window, GLFW_KEY_S         ) == GLFW_PRESS) CameraVelocity.z -= speed;

		CameraPosition += CameraSpeed * CameraAxisX * CameraVelocity.x;
		CameraPosition += CameraSpeed * WorldAxisY  * CameraVelocity.y;
		CameraPosition += CameraSpeed * CameraAxisZ * CameraVelocity.z;
		CameraVelocity *= Dampling;

		glm::mat4 projection = glm::perspective(fov, aspect_ratio, 0.f, 100.f);
		glm::mat4 view = glm::lookAt(CameraPosition, CameraPosition+CameraAxisZ, CameraAxisY);
		glm::mat4 transform = projection*view;

		shader.use();
		shader.setUniform("transform", transform);
		sphere1.draw();
		//sphere2.draw();
		//cube.draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
						   
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

//-----------------------------------

void DrawInterface()
{
	if (InterfaceVisible)
	{
		if (ImGui::Begin("Debug"))
		{
			ImGui::Button("Pizda!");
		}

		ImGui::End();
	}

	else ImGui::SetMouseCursor(ImGuiMouseCursor_None);
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
		{
			if (action == GLFW_PRESS)
			{
				if (InterfaceVisible) SetInterfaceVisible(window, false);
				else glfwSetWindowShouldClose(window, true);
			}

			break;
		}

		case GLFW_KEY_R:
		{
			if (action == GLFW_PRESS)
			{
				CameraPosition = glm::vec3(0, 0,  3);
				CameraAxisX    = glm::vec3(1, 0,  0);
				CameraAxisY    = glm::vec3(0, 1,  0);
				CameraAxisZ    = glm::vec3(0, 0, -1);
				CameraRotation = glm::vec2(0, 0    );
			}

			break;
		}

		case GLFW_KEY_X:
		{
			if (action == GLFW_PRESS)
			{
				GLint current_polygon_mode[2] = {};

				glGetIntegerv(GL_POLYGON_MODE, current_polygon_mode);
				switch (*current_polygon_mode)
				{
					case GL_FILL:
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						break;

					case GL_LINE:
						glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
						break;

					case GL_POINT:
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						break;
				}

				glcheck;
			}

			break;
		}

		case GLFW_KEY_F1:
		{
			if (action == GLFW_PRESS)
				InterfaceVisible += 1;

			break;
		}
	}
}

void GLFWCursorPosCallback(GLFWwindow* window, double x, double y)
{
	if (!InterfaceVisible)
	{
		CameraRotation += CenterCursor(window)*glm::vec2(1, -1)*CameraRotationSpeed;

		glm::vec3 camera_axis_z = glm::normalize(glm::vec3(
			cos(glm::radians(CameraRotation.y)) * cos(glm::radians(CameraRotation.x)),
			sin(glm::radians(CameraRotation.y)),
			cos(glm::radians(CameraRotation.y)) * sin(glm::radians(CameraRotation.x))
		));

		CameraAxisZ = camera_axis_z;
		CameraAxisX = glm::normalize(glm::cross(CameraAxisZ, WorldAxisY ));
		CameraAxisY = glm::normalize(glm::cross(CameraAxisX, CameraAxisZ));
	}
}

void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// ...
}

//-----------------------------------

glm::vec2 CenterCursor(GLFWwindow* window)
{
	double cursor_pos_x = 0, cursor_pos_y = 0;
	glfwGetCursorPos(window, &cursor_pos_x, &cursor_pos_y);
	int window_size_x = 0,window_size_y = 0;
	glfwGetWindowSize(window, &window_size_x, &window_size_y);
	glm::vec2 window_center(window_size_x/2, window_size_y/2);
	glfwSetCursorPos(window, window_center.x, window_center.y);
	return glm::vec2(cursor_pos_x, cursor_pos_y) - window_center;
}

void SetInterfaceVisible(GLFWwindow* window, bool visible)
{
	if (!(InterfaceVisible = visible))
		CenterCursor(window);
}

//-----------------------------------