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
#include "Cylinder.hpp"
#include "Plane.hpp"
#include "Cone.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "PrimitiveBuffer.hpp"
#include "Light.hpp"
#include "DebugVisualization.hpp"

#pragma warning(disable: 4244)

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
	float camera_speed;
	float fov;
	float fov_vel;
	float aspect_ratio;
};

//-----------------------------------

void DoRender(GLFWwindow* window);
void DoControl(GLFWwindow* window);
void DrawInterface(GLFWwindow* window);

void GLFWErrorCallback(int error, const char* description);
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFWCursorPosCallback(GLFWwindow* window, double x, double y);
void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void GLFWMouseWheelCallback(GLFWwindow* window, double x, double y);

void OnToggleRenderMode(GLFWwindow* window);
void OnSaveScreenshot(GLFWwindow* window);

// Measures cursor offset relative to window's center and puts cursor in center of window
glm::vec2 CenterCursor(GLFWwindow* window);

// Sets interface visibility state according to parameter; It also moves cursor back to center if interface becomes visible
void SetInterfaceVisible(GLFWwindow* window, bool visible);

void Cleanup();

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
	glfwSetScrollCallback     (window, GLFWMouseWheelCallback );
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

	printf("OpenGL Version: %s\n", glSafeCall(glGetString(GL_VERSION)));

	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	glSafeCallVoid(glViewport(0, 0, width, height));

	glSafeCallVoid(glEnable(GL_DEPTH_TEST));
	glSafeCallVoid(glEnable(GL_BLEND));
	glSafeCallVoid(glEnable(GL_MULTISAMPLE));
	glSafeCallVoid(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glSafeCallVoid(glDepthFunc(GL_LESS));

	Shader shader;
	if (!shader.loadFromFile("Resources/Shaders/shader.vert", "Resources/Shaders/shader.frag", "Resources/Shaders/shader.geom"))
		return 0;

	Camera camera;
	PrimitiveBuffer scene;
	glm::mat4 projection;

	WindowData window_data = {};
	window_data.camera = &camera;
	window_data.scene = &scene;
	window_data.interface_visible = false;
	window_data.shader = &shader;
	window_data.projection = &projection;
	window_data.should_save_screenshot = false;
	window_data.screenshot_hide_interface = true;
	window_data.screenshot_show = true;
	window_data.camera_speed = 1.f;
	window_data.fov = 45.f;
	window_data.fov_vel = 0.f;
	window_data.aspect_ratio = window_size.x / window_size.y;
	glfwSetWindowUserPointer(window, &window_data);

	Texture texture;
	texture.loadFromFile("Resources/Textures/rock/base.png");
	texture.setID(Texture::ID::BaseColor);
	texture.setFilters(Texture::Filter::NearestNeightbour);

	Texture normalmap;
	normalmap.loadFromFile("Resources/Textures/rock/normal.png");
	normalmap.setID(Texture::ID::NormalMap);
	normalmap.setFilters(Texture::Filter::NearestNeightbour);

	auto object = scene += new Plane;
	object->setColor(Color(0, .7, 1));
	//object->setTexture(&texture);
	object->setNormalMap(&normalmap);

	auto light1 = scene += new Light;
	light1->setColor("#FFF7C9");
	light1->setPosition(glm::vec3(-.5f, 0, 1));

	auto light1_shape = scene += new Sphere;
	light1_shape->setLightningEnabled(false);
	light1_shape->setRadius(.1f);
	light1_shape->setColor(light1->getColor());
	light1_shape->setPosition(light1->getPosition());
	light1_shape->setPointCount(64, 64);

	auto light2 = scene += new Light;
	light2->setColor("#33A7FF");
	light2->setPosition(glm::vec3(.5f, 0, 1));
	light2->setAmbientStrength(0);

	auto light2_shape = scene += new Cube;
	light2_shape->setLightningEnabled(false);
	light2_shape->setSize(glm::vec3(.1f));
	light2_shape->setColor(light2->getColor());
	light2_shape->setPosition(light2->getPosition());

	double object_rotation_angle = 0;
	while (!glfwWindowShouldClose(window))
	{
		DoControl(window);

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)  object_rotation_angle += 0.01;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)  object_rotation_angle -= 0.01;
		object->setDirection(glm::vec3(0, sin(object_rotation_angle), cos(object_rotation_angle)));

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) object->setPosition(object->getPosition()+glm::vec3(0.01, 0, 0));
		if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS) object->setPosition(object->getPosition()-glm::vec3(0.01, 0, 0));
		if (glfwGetKey(window, GLFW_KEY_UP   ) == GLFW_PRESS) object->setPosition(object->getPosition()+glm::vec3(0, 0.01, 0));
		if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS) object->setPosition(object->getPosition()-glm::vec3(0, 0.01, 0));

		DoRender(window);
	}

	atexit(Cleanup);
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

	glSafeCallVoid(glClearColor(Color(.05f, .05f, .05f)));
	glSafeCallVoid(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

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
		glSafeCallVoid(glGetIntegerv(GL_POLYGON_MODE, polygon_render_mode));

		bool depth_test_enabled = glSafeCall(glIsEnabled(GL_DEPTH_TEST));

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
			glm::vec3 pos = window_data->camera->getPosition();
			ImGui::Text("Position: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);

			ImGui::SliderFloat("Camera speed", &window_data->camera_speed, .01f, 5.f, "%.2f");

			if (ImGui::Button("Save camera state")) window_data->camera->saveToFile  (camera_state_filename);
			if (ImGui::Button("Load camera state")) window_data->camera->loadFromFile(camera_state_filename);
			if (ImGui::Button("Save screenshot")) window_data->should_save_screenshot = true;

			ImGui::Checkbox("Hide interface on screenshot", &window_data->screenshot_hide_interface);
			ImGui::Checkbox("Show screenshot", &window_data->screenshot_show);
		}

		ImGui::End();

		glSafeCallVoid(glSetEnabled(GL_DEPTH_TEST, depth_test_enabled));
		glSafeCallVoid(glPolygonMode(GL_FRONT_AND_BACK, *polygon_render_mode));
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
	window_data->camera->addVelocity(movement * speed * window_data->camera_speed);
	window_data->camera->update();

	window_data->fov = std::clamp(window_data->fov + (window_data->fov_vel*=.9f), 1.f, 100.f);

	static float last_fov = -1;
	if (window_data->fov != last_fov)
		*window_data->projection = glm::perspective(glm::radians(window_data->fov), window_data->aspect_ratio, .1f, 1000.f);

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
			window_data->fov = 45.f;
			window_data->fov_vel = 0;
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

void GLFWMouseWheelCallback(GLFWwindow* window, double x, double y)
{
	WindowData* window_data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if (!window_data)
		return;

	window_data->fov_vel -= 2*y;
}

//-----------------------------------

void OnToggleRenderMode(GLFWwindow* window)
{
	GLint current_polygon_mode[2] = {};

	glSafeCallVoid(glGetIntegerv(GL_POLYGON_MODE, current_polygon_mode));
	switch (*current_polygon_mode)
	{
		case GL_FILL:
			glSafeCallVoid(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
			break;

		case GL_LINE:
			glSafeCallVoid(glPolygonMode(GL_FRONT_AND_BACK, GL_POINT));
			break;

		case GL_POINT:
			glSafeCallVoid(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
			break;
	}
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
	glSafeCallVoid(glGetIntegerv(GL_VIEWPORT, dims));
	glm::uvec2 framesize(dims[2], dims[3]);

	// Hide GUI and rerender frame if interface was active
	bool was_interface_visible = window_data->interface_visible;
	if (was_interface_visible && window_data->screenshot_hide_interface)
	{
		SetInterfaceVisible(window, false);
		DoRender(window);
	}

	uint8_t* data = new uint8_t[3*framesize.x*framesize.y];
	glSafeCallVoid(glReadPixels(0, 0, framesize.x, framesize.y, GL_RGB, GL_UNSIGNED_BYTE, data));

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

void Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

//-----------------------------------