#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <cmath>
#include <cstdio>
#include <filesystem>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Shader.hpp"
#include "Sphere.hpp"
#include "Cube.hpp"
#include "Plane.hpp"
#include "Model.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "PrimitiveBuffer.hpp"
#include "Light.hpp"

//-----------------------------------

constexpr bool FullscreenMode = false;

//-----------------------------------

struct WindowData
{
	Camera* camera;
	Shader* shader;
	PrimitiveBuffer* scene;
	glm::mat4* projection;
	bool interface_visible;		
	bool should_save_screenshot;
	bool screenshot_hide_interface;
	bool screenshot_show;
};

//-----------------------------------

void DoRender(GLFWwindow* window);
void DoControl(GLFWwindow* window);
void DrawInterface(GLFWwindow* window);

void GLFWErrorCallback(int error, const char* description);
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFWCursorPosCallback(GLFWwindow* window, double x, double y);
void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void OnToggleRenderMode(GLFWwindow* window);
void OnSaveScreenshot(GLFWwindow* window);

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
		return 0;

	Camera camera;
	camera.reset();

	auto projection = glm::perspective(glm::radians(45.f), aspect_ratio, .1f, 1000.f);
	PrimitiveBuffer scene;

	WindowData window_data = {};
	window_data.camera = &camera;
	window_data.scene = &scene;
	window_data.interface_visible = false;
	window_data.shader = &shader;
	window_data.projection = &projection;
	window_data.should_save_screenshot = false;
	window_data.screenshot_hide_interface = true;
	window_data.screenshot_show = true;
	glfwSetWindowUserPointer(window, &window_data);

	Texture texture;
	texture.loadFromFile("Resources/Textures/rock/base.png");
	texture.setID(Texture::ID::BaseColor);

	Texture normalmap;
	normalmap.loadFromFile("Resources/Textures/rock/normal.png");
	normalmap.setID(Texture::ID::Normal);

	auto plane = scene += new Plane;
	plane->setTexture(&texture);
	plane->setNormalMap(&normalmap);
	plane->setRotation(glm::vec2(M_PI/2, 0.f));

	auto light1 = scene += new Light;
	light1->setAmbientStrength(0);
	light1->setColor("#FFF7C9");
	light1->setPosition(glm::vec3(-1, 0, 1));

	auto light_shape1 = scene += new Sphere;
	light_shape1->setRadius(.1);
	light_shape1->setColor(light1->getColor());
	light_shape1->setPosition(light1->getPosition());
	light_shape1->setPointCount(64, 64);
	light_shape1->setLightningEnabled(false);

	while (!glfwWindowShouldClose(window))
	{
		DoControl(window);

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) plane->setRotation(plane->getRotation()+glm::vec2(0.01, 0));
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) plane->setRotation(plane->getRotation()-glm::vec2(0.01, 0));

		DoRender(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

//-----------------------------------

void DoRender(GLFWwindow* window)
{
	WindowData* window_data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if (!window_data)
		return;

	Shader& shader = *window_data->shader;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	DrawInterface(window);	

	glClearColor(Color(.05, .05, .05));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader["projection"] = *window_data->projection;
	shader["view"      ] = window_data->camera->getView();
	shader["viewPos"   ] = window_data->camera->getPosition();
	window_data->scene->drawObjects(window_data->shader);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}

//-----------------------------------

void DrawInterface(GLFWwindow* window)
{
	WindowData* window_data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if (!window_data)
		return;

	const std::filesystem::path camera_state_filename = "camera.dat";

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

		if (ImGui::CollapsingHeader("Camera"))
		{
			if (ImGui::Button("Save camera state")) window_data->camera->saveToFile  (camera_state_filename);
			if (ImGui::Button("Load camera state")) window_data->camera->loadFromFile(camera_state_filename);
			if (ImGui::Button("Save screenshot")) window_data->should_save_screenshot = true;

			ImGui::Checkbox("Hide interface on screenshot", &window_data->screenshot_hide_interface);
			ImGui::Checkbox("Show screenshot", &window_data->screenshot_show);
		}

		ImGui::End();

		glSetEnabled(GL_DEPTH_TEST, depth_test_enabled);
		glPolygonMode(GL_FRONT_AND_BACK, *polygon_render_mode);
	}

	else ImGui::SetMouseCursor(ImGuiMouseCursor_None);
}

//-----------------------------------

void DoControl(GLFWwindow* window)
{
	glfwPollEvents();

	WindowData* window_data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if (!window_data)
		return;

	glm::vec3 movement(0);
	#define _move(key, coordinate, direction) \
	if (glfwGetKey(window, GLFW_KEY_##key) == GLFW_PRESS) movement.##coordinate direction##= 1;

	_move(D,          x, +);
	_move(A,          x, -);
	_move(SPACE,      y, +);
	_move(LEFT_SHIFT, y, -);
	_move(W,          z, +);
	_move(S,          z, -);

	#undef _move

	float speed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS? .01f: .005f;
	window_data->camera->addVelocity(movement * speed);
	window_data->camera->update();

	if (window_data->should_save_screenshot)
		OnSaveScreenshot(window), window_data->should_save_screenshot = false;
}

//-----------------------------------

void GLFWErrorCallback(int error, const char* description)
{
	printf("GLFW Error: %s\n", description);
}

void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	WindowData* window_data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if (!window_data)
		return;

	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			if (window_data->interface_visible) SetInterfaceVisible(window, false);
			else glfwSetWindowShouldClose(window, true);

			break;

		case GLFW_KEY_R:
			window_data->camera->reset();
			break;

		case GLFW_KEY_X:
			OnToggleRenderMode(window);
			break;

		case GLFW_KEY_F1:
			SetInterfaceVisible(window, !window_data->interface_visible);
			break;

		case GLFW_KEY_F2:
			OnSaveScreenshot(window);
			break;
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

void OnToggleRenderMode(GLFWwindow* window)
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

//-----------------------------------

void OnSaveScreenshot(GLFWwindow* window)
{
	WindowData* window_data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if (!window_data)
		return;

	const std::filesystem::path capture_directory("./captures");
	if (!std::filesystem::exists(capture_directory))
	{
		printf("Creating directory %s\n", capture_directory.string().c_str());
		std::filesystem::create_directory(capture_directory);
	}

	else if (!std::filesystem::is_directory(capture_directory))
	{
		printf("Can't save image: '%s' exists, but it is not directory\n", capture_directory.string().c_str());
		return;
	}

	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char filename[BUFFSIZE] = "";
	strftime(filename, BUFFSIZE, "%d-%m-%Y_%H-%M-%S.bmp", std::localtime(&time));

	std::filesystem::path path = capture_directory/filename;

	GLint dims[4] = {0};
	glGetIntegerv(GL_VIEWPORT, dims);
	glm::uvec2 framesize(dims[2], dims[3]);

	// Hide GUI and rerender frame if interface was active
	bool was_interface_visible = window_data->interface_visible;
	if (was_interface_visible && window_data->screenshot_hide_interface)
	{
		SetInterfaceVisible(window, false);
		DoRender(window);
	}

	uint8_t* data = new uint8_t[3*framesize.x*framesize.y];
	glReadPixels(0, 0, framesize.x, framesize.y, GL_RGB, GL_UNSIGNED_BYTE, data);

	if (was_interface_visible && window_data->screenshot_hide_interface) 
		SetInterfaceVisible(window, true);

	// Flipping image vertically
	for (size_t y = 0; y < framesize.y/2; y++)
		for (size_t x = 0; x < framesize.x*3; x++)
			std::swap(*(data+framesize.x*y*3+x), *(data+framesize.x*(framesize.y-y-1)*3+x));

	if (SOIL_save_image(path.string().c_str(), SOIL_SAVE_TYPE_BMP, framesize.x, framesize.y, 3, data))
	{
		if (window_data->screenshot_show)
		{
			char command[BUFFSIZE] = "";
			sprintf_s(command, "start %s", path.string().c_str());
			system(command);
		}

		printf("Capture save as '%s'\n", path.string().c_str());
	}
	else printf("Failed to save capture\n");

	delete[] data;
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
