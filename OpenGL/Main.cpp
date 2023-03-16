#define _USE_MATH_DEFINES
#include <cmath>

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
#include "Camera.hpp"

//-----------------------------------

constexpr bool  FullscreenMode = false;

int   SphereHorizontalPointCount = 16;
int   SphereVerticalPointCount   = 16;
float SphereRadius               = .5f;
Color SphereColor                = Color::FromBytesRGB(0, 170, 255);

float FieldOfView = 45;
Camera TheCamera;
glm::mat4 ProjectionMatrix;

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

	glm::vec2 monitor_size(videomode->width, videomode->height);
	glm::vec2 window_size = static_cast<glm::vec2>(monitor_size)*(FullscreenMode? 1.f: .9f);

	float aspect_ratio = window_size.x/window_size.y;
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
	glDepthFunc(GL_LESS); 

	Shader shader;
	if (!shader.loadFromFile("shader.vert", "shader.frag"))
	{
		printf("Failed to load shader\n");
		return 0;
	}

	Sphere sphere;

	float last_fov = -1;
	while (!glfwWindowShouldClose(window))
	{
		if (FieldOfView != last_fov)
		{
			last_fov = FieldOfView;
			ProjectionMatrix = glm::perspective(glm::radians(FieldOfView), aspect_ratio, .1f, 100.f);
		}

		glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		DrawInterface();

		glClearColor(.05, .05, .05, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 movement(
			// X
			 (glfwGetKey(window, GLFW_KEY_D         ) == GLFW_PRESS) 
			-(glfwGetKey(window, GLFW_KEY_A         ) == GLFW_PRESS), 

			// Y
			 (glfwGetKey(window, GLFW_KEY_SPACE     ) == GLFW_PRESS)
			-(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS), 

			// Z
			 (glfwGetKey(window, GLFW_KEY_W         ) == GLFW_PRESS) 
			-(glfwGetKey(window, GLFW_KEY_S         ) == GLFW_PRESS)
		);

		float speed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS? .01f: .005f;
		TheCamera.addVelocity(movement*speed);
		TheCamera.update();

		sphere.setPointCount(SphereHorizontalPointCount, SphereVerticalPointCount);
		sphere.setRadius(SphereRadius);
		sphere.setColor(SphereColor);

		shader.setUniform("projection", ProjectionMatrix   );
		shader.setUniform("view",       TheCamera.getView());
		sphere.draw(&shader);

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
		GLint polygon_render_mode[2] = {};
		glGetIntegerv(GL_POLYGON_MODE, polygon_render_mode);
		bool depth_test_enabled = glIsEnabled(GL_DEPTH_TEST);

		ImGui::Begin("Debug");

		if (ImGui::CollapsingHeader("Rendering"))
		{
			ImGui::Text("Polygon render mode:");
			ImGui::RadioButton("Fill", polygon_render_mode, GL_FILL);
			ImGui::RadioButton("Wireframe", polygon_render_mode, GL_LINE);
			ImGui::RadioButton("Points", polygon_render_mode, GL_POINT);
			ImGui::Separator();
			ImGui::Checkbox("Enable depth test", &depth_test_enabled);

			ImGui::SliderFloat("FOV", &FieldOfView, 1, 180);
		}

		if (ImGui::CollapsingHeader("Sphere"))
		{
			ImGui::SliderInt  ("Horizontal points", &SphereHorizontalPointCount, 4, 256);
			ImGui::SliderInt  ("Vertical points",   &SphereVerticalPointCount,   4, 256);
			ImGui::SliderFloat("Radius",            &SphereRadius, .1f, 5.f, "%.2f");
			ImGui::ColorEdit4 ("Color",             SphereColor.data);
		}

		ImGui::End();

		glSetEnabled(GL_DEPTH_TEST, depth_test_enabled);
		glPolygonMode(GL_FRONT_AND_BACK, *polygon_render_mode);
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
				TheCamera.reset();

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
		TheCamera.addRotation(CenterCursor(window)*glm::vec2(1, -1)*.1f);
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