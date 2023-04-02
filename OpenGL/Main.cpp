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
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Logging.hpp"
#include "Shader.hpp"
#include "Sphere.hpp"
#include "Cube.hpp"
#include "Cylinder.hpp"
#include "Plane.hpp"
#include "Cone.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "PrimitiveBuffer.hpp"
#include "DebugVisualization.hpp"
#include "Material.hpp"
#include "AmbientLight.hpp"
#include "PointLight.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"

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
	bool normal_calculation;
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
	SetLoggingLevel(LoggingLevel::Info);

	LogInfo("Initializing GLFW");
	if (!glfwInit())
	{
		LogError("GLFW Initialization failed");
		return 0;
	}

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videomode = glfwGetVideoMode(monitor);

	glm::vec2 monitor_size(videomode->width, videomode->height);
	glm::vec2 window_size = static_cast<glm::vec2>(monitor_size) * (FullscreenMode ? 1.f : .8f);

	// Initializing window
	LogInfo("Initializing window");
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwSetErrorCallback(GLFWErrorCallback);

	GLFWwindow* window = glfwCreateWindow(window_size.x, window_size.y, "OpenGL Test", FullscreenMode ? monitor : nullptr, nullptr);
	if (!window)
	{
		LogError("Window creation failed");
		return 0;
	}

	glfwMakeContextCurrent    (window);
	glfwSetKeyCallback        (window, GLFWKeyCallback        );
	glfwSetCursorPosCallback  (window, GLFWCursorPosCallback  );
	glfwSetMouseButtonCallback(window, GLFWMouseButtonCallback);
	glfwSetScrollCallback     (window, GLFWMouseWheelCallback );
	glfwFocusWindow(window);
	glfwSetWindowPos(window, monitor_size.x / 2 - window_size.x / 2, monitor_size.y / 2 - window_size.y / 2);
	CenterCursor(window);

	// Initializing interface
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
	SetInterfaceVisible(window, false);

	// Initializing OpenGL
	LogInfo("Initializing OpenGL");
	if (glewInit() != GLEW_OK)
	{
		LogError("GLEW initalization failed");
		return false;
	}

	LogInfo("OpenGL Version: %s", glSafeCall(glGetString(GL_VERSION)));

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
	window_data.normal_calculation = false;
	window_data.camera_speed = 1.f;
	window_data.fov = 45.f;
	window_data.fov_vel = 0.f;
	window_data.aspect_ratio = window_size.x / window_size.y;
	glfwSetWindowUserPointer(window, &window_data);

	std::filesystem::path surface_base("Resources\\Textures\\wall");
	Texture surface_diffuse(surface_base/"base.png");
	surface_diffuse.setWrappingMode(Texture::WrappingMode::Repeat);
	Texture surface_normal(surface_base/"normal.png");
	surface_normal.setWrappingMode(Texture::WrappingMode::Repeat);
	Texture surface_specular(surface_base/"specular.png");
	surface_specular.setWrappingMode(Texture::WrappingMode::Repeat);
	Material surface_material;
	surface_material.setTextureScale(glm::vec2(10, 10));
	surface_material.setDiffuseMap(&surface_diffuse);
	surface_material.setNormalMap(&surface_normal);
	surface_material.setSpecularMap(&surface_specular);
	
	auto surface = scene += new Plane;
	surface->setSize(glm::vec2(10, 10));
	surface->setPosition(glm::vec3(0, -.5f, 0));
	surface->setMaterial(&surface_material);

	std::filesystem::path object_base("Resources\\Textures\\stone");
	Texture object_diffuse(object_base/"base.png");
	Texture object_normal(object_base/"normal.png");
	Texture object_specular(object_base/"specular.png");
	Material object_material;
	object_material.setDiffuseMap(&object_diffuse);
	object_material.setNormalMap(&object_normal);
	object_material.setSpecularMap(&object_specular);

	auto object = scene += new Cube;
	object->setMaterial(&object_material);

	//auto ambient = scene += new AmbientLight;
	//ambient->setColor(Color::White);
	
	//auto dir = scene += new DirectionalLight;
	//dir->setColor("#FFF7C9");
	//dir->setDirection(glm::vec3(cos(-M_PI/6), sin(-M_PI/6), 0));
	
	//auto point = scene += new PointLight;
	//point->setColor("#33A7FF");
	//point->setPosition(glm::vec3(.5f, 0, 1));
	//point->setDiffuseStrength(0.3f);
	//point->setSpecularStrength(1);

	auto spot = scene += new SpotLight;
	spot->setColor(Color::White);
	spot->setCutoffAngle(.90f);
	spot->setOuterCutoffAngle(.92f);
	spot->setDirection(glm::vec3(0, -1, 0));
	spot->setPosition(glm::vec3(0, 5, 0));

	auto spot1 = scene += new SpotLight;
	spot1->setColor("#33A7FF");
	spot1->setCutoffAngle(.90f);
	spot1->setOuterCutoffAngle(.92f);
	spot1->setDirection(glm::vec3(0.2, -1, 0));
	spot1->setPosition(glm::vec3(-3, 5, 0));

	glm::vec3 object_rotation_angle(0, 0, 0);
	glm::vec3 object_rotation_vel(0, 0, 0);
	while (!glfwWindowShouldClose(window))
	{
		DoControl(window);

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) object_rotation_vel.y ++;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) object_rotation_vel.y --;
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) object_rotation_vel.x ++;
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) object_rotation_vel.x --;

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) spot->setCutoffAngle(spot->getCutoffAngle()+0.001f);
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) spot->setCutoffAngle(spot->getCutoffAngle()-0.001f);
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) spot->setOuterCutoffAngle(spot->getOuterCutoffAngle()+0.001f);
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) spot->setOuterCutoffAngle(spot->getOuterCutoffAngle()-0.001f);

		object_rotation_angle += object_rotation_vel * 0.0008f;
		object_rotation_vel *= 0.95f;
		object->setRotation(object_rotation_angle);

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

	glSafeCallVoid(glClearColor(Color::Black));
	glSafeCallVoid(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	shader["projection"  ] = *window_data->projection;
	shader["view"        ] = window_data->camera->getView();
	shader["viewPosition"] = window_data->camera->getPosition();
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

		if (ImGui::CollapsingHeader("Debugging", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Debug break"))
				DebugBreak(); // HELLO, PIDOR!
		}

		if (ImGui::CollapsingHeader("Rendering", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Polygon render mode:");
			ImGui::RadioButton("Fill", polygon_render_mode, GL_FILL);
			ImGui::RadioButton("Wireframe", polygon_render_mode, GL_LINE);
			ImGui::RadioButton("Points", polygon_render_mode, GL_POINT);
			ImGui::Separator();
			ImGui::Checkbox("Enable depth test", &depth_test_enabled);
			ImGui::Checkbox("Calculate normals in shader", &window_data->normal_calculation);
		}

		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
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

		if (ImGui::CollapsingHeader("Logging", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static std::pair<LoggingLevel, const char*> items[] = {
				{LoggingLevel::Info,    "Log everithing"     },
				{LoggingLevel::Warning, "Warnings and errors"},
				{LoggingLevel::Error,   "Log only errors"    },
				{LoggingLevel::Silent,  "Disable logging"    }
			};
			size_t size = IM_ARRAYSIZE(items);
			static int selected_item_index = std::distance(items, std::find_if(items, items + size, [](auto item) { return item.first == GetLoggingLevel(); }));

			if (ImGui::BeginCombo("Logging level", items[selected_item_index].second))
			{
				for (size_t i = 0; i < size; i++)
				{
					bool selected = i == selected_item_index;
					if (ImGui::Selectable(items[i].second, selected))
						selected_item_index = i, SetLoggingLevel(items[i].first);

					if (selected) ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}			
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
	LogError("GLFW Error: %s\n", description);
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
		LogInfo("Creating directory %s", capture_directory.string().c_str());
		std::filesystem::create_directory(capture_directory);
	}

	else if (!std::filesystem::is_directory(capture_directory))
	{
		LogWarning("Can't save image: '%s' exists, but it is not directory", capture_directory.string().c_str());
		return;
	}

	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char filename[BUFFSIZE] = "";
	strftime(filename, BUFFSIZE, "%d-%m-%Y_%H-%M-%S.png", std::localtime(&time));

	std::filesystem::path path = capture_directory/filename;

	// Hide GUI and rerender frame if interface was active
	bool was_interface_visible = window_data->interface_visible;
	if (was_interface_visible && window_data->screenshot_hide_interface)
	{
		SetInterfaceVisible(window, false);
		DoRender(window);
	}

	Texture frame;
	frame.captureFrameBuffer();

	if (was_interface_visible && window_data->screenshot_hide_interface) 
		SetInterfaceVisible(window, true);

	if (!frame.saveToFile(path))
		 return;

	if (window_data->screenshot_show)
		SystemF("start %s", path.string().c_str());

	LogInfo("Screenshot saved as '%s'", path.string().c_str());	
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
	LogInfo("Cleaning up");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

//-----------------------------------