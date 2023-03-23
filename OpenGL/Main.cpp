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
#include "Plane.hpp"
#include "Model.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "PrimitiveBuffer.hpp"

//-----------------------------------

constexpr bool FullscreenMode   = false;

//-----------------------------------

struct WindowData
{
	Camera* camera;
	PrimitiveBuffer* scene;
	bool interface_visible;
};

//-----------------------------------

void DrawInterface(GLFWwindow* window);

void GLFWErrorCallback(int error, const char* description);
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFWCursorPosCallback(GLFWwindow* window, double x, double y);
void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// Measures cursor offset relative to window's center and puts cursor in center of window
glm::vec2 CenterCursor(GLFWwindow* window);

// Sets interface visibility state according to parameter; It also moves cursor back to center if interface becomes visible
void SetInterfaceVisible(GLFWwindow* window, bool visible);

//-----------------------------------

glm::vec3 FindIntersectionPlane(const Sphere& s1, const Sphere& s2);

//-----------------------------------

int main()
{
	/*const char* delimiters = " .,!-?";
	const char* line_begin = "pidor, jopa! chlen govno. klitor, sperma, fen - ono!";
	const char* line_end = StrPBRK(line_begin, delimiters);

	for (size_t token_index = 0; line_end; token_index++)
	{
		unsigned line_len = line_end-line_begin;
		printf("[%zu]: '%.*s'\n", token_index, line_len, line_begin);

		line_begin = StrPBRKI(line_end, delimiters);
		if (!line_begin)  break;
		line_end = StrPBRK(line_begin, delimiters);
	}

	return 0;*/

	if (!glfwInit())
	{
		printf("GLFW Initialization failed\n");
		return 0;
	}

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videomode = glfwGetVideoMode(monitor);

	glm::vec2 monitor_size(videomode->width, videomode->height);
	glm::vec2 window_size = static_cast<glm::vec2>(monitor_size) * (FullscreenMode ? 1.f : .8f);

	float aspect_ratio = window_size.x / window_size.y;
	constexpr float fov = glm::radians(45.f);

	// Initializing window
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwSetErrorCallback(GLFWErrorCallback);

	GLFWwindow* window = glfwCreateWindow(window_size.x, window_size.y, "OpenGL Test", FullscreenMode ? monitor : nullptr, nullptr);
	if (!window)
	{
		printf("Window creation failed\n");
		return 0;
	}

	glfwMakeContextCurrent    (window);
	glfwSetKeyCallback        (window, GLFWKeyCallback        );
	glfwSetCursorPosCallback  (window, GLFWCursorPosCallback  );
	glfwSetMouseButtonCallback(window, GLFWMouseButtonCallback);
	glfwFocusWindow(window);
	glfwSetWindowPos(window, monitor_size.x / 2 - window_size.x / 2, monitor_size.y / 2 - window_size.y / 2);

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
	if (!shader.loadFromFile("Resources/Shaders/shader.vert", "Resources/Shaders/shader.frag"))
	{
		printf("Failed to load shader\n");
		return 0;
	}

	Camera camera;
	camera.reset();

	auto projection = glm::perspective(glm::radians(45.f), aspect_ratio, .1f, 1000.f);
	PrimitiveBuffer scene;

	WindowData window_data = {};
	window_data.camera = &camera;
	window_data.scene = &scene;
	window_data.interface_visible = false;
	glfwSetWindowUserPointer(window, &window_data);

	#if 0
	#define __addsphere(x, y, z, r) 			     \
	{ 											     \
		Sphere* sphere = scene += new Sphere; 	     \
		sphere->setPosition(glm::vec3(x, y, z)*.1f); \
		sphere->setRadius(r*.1f);					 \
		sphere->setColor(Color::Random());           \
		sphere->setPointCount(128, 128);             \
	}											 

	__addsphere( 21,  0,  17, 26.763848785401); 
	__addsphere( 1,  -14, 9,  15.16227512352 );
	__addsphere(-20,  0,  14, 24.281858768853);
	__addsphere( 6,   0, -23, 24.135541900996);

	#undef __addsphere
	#endif
	
	//Plane* plane = scene += new Plane;
	//plane->setSize(glm::vec2(5, 5));
	//plane->setColor(Color::White);

	Model* model = scene += new Model;
	if (!model->loadFromFile("Resources/Models/rat.obj"))
		printf("Model loading failed!\n");

	model->setColor(Color(0, .7, 1));
	model->setScale(glm::vec3(5, 5, 5));
	model->setPosition(glm::vec3(0, -2, 0));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	camera.reset();
	camera.setRotation(glm::vec2(45, -45));
	camera.setPosition(glm::vec3(-6, 7, 6));

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawInterface(window);

		glClearColor(.05, .05, .05, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 movement(
			// X
			  (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			- (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS),

			// Y
			  (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			- (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS),

			// Z
			  (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			- (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		);

		float speed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? .01f : .005f;
		camera.addVelocity(movement * speed);
		camera.update();

		shader.setUniform("projection", projection);
		shader.setUniform("view",       camera.getView());

		double angle = glfwGetTime();
		model->setRotation(glm::vec2(0, angle));
		//model->setPosition(glm::vec3(cos(angle)*5, 0, -sin(angle)*2));

		scene.drawObjects(&shader);

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

void DrawInterface(GLFWwindow* window)
{
	WindowData* window_data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if (!window_data)
		return;

	if (window_data->interface_visible)
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
	WindowData* window_data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if (!window_data)
		return;

	switch (key)
	{
		case GLFW_KEY_ESCAPE:
		{
			if (action == GLFW_PRESS)
			{
				if (window_data->interface_visible) SetInterfaceVisible(window, false);
				else glfwSetWindowShouldClose(window, true);
			}

			break;
		}

		case GLFW_KEY_R:
		{
			if (action == GLFW_PRESS)
				window_data->camera->reset();

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
				SetInterfaceVisible(window, !window_data->interface_visible);

			break;
		}
	}
}

void GLFWCursorPosCallback(GLFWwindow* window, double x, double y)
{
	WindowData* window_data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if (!window_data)
		return;

	if (!window_data->interface_visible)
		window_data->camera->addRotation(CenterCursor(window) * glm::vec2(1, -1) * .1f);
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
	int window_size_x = 0, window_size_y = 0;
	glfwGetWindowSize(window, &window_size_x, &window_size_y);
	glm::vec2 window_center(window_size_x / 2, window_size_y / 2);
	glfwSetCursorPos(window, window_center.x, window_center.y);
	return glm::vec2(cursor_pos_x, cursor_pos_y) - window_center;
}

void SetInterfaceVisible(GLFWwindow* window, bool visible)
{
	WindowData* window_data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if(!window_data)
		return;

	if (!(window_data->interface_visible = visible))
		CenterCursor(window);
}

//-----------------------------------