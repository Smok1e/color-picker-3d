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
#include "SpaceSphere.hpp"
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

	Texture texture;
	texture.loadFromFile("Resources/Textures/test.png");

	//Plane* plane = scene += new Plane;
	//plane->setColor(Color::White);
	//plane->setPosition(glm::vec3(0, 0, 0));
	//plane->setTexture(&texture);
	//plane->setSize(5.f * glm::vec2(static_cast<float>(texture.getSizeX())/texture.getSizeX(), static_cast<float>(texture.getSizeY())/texture.getSizeX()));

	//Model* model = scene += new Model;
	//if (!model->loadFromFile("Resources/Models/hyperstone.obj"))
	//	printf("Model loading failed!\n");

	//model->setColor(Color(0, .7, 1));
	//model->setScale(glm::vec3(2, 2, 2));
	//model->setPosition(glm::vec3(-5, 0, 0));

	Texture earth_texture;
	earth_texture.loadFromFile("Resources/Textures/earth_night.jpg");

	Sphere* earth = scene += new Sphere;
	earth->setRadius(2);
	earth->setColor(Color::FromBytesRGB(0, 170, 255));
	earth->setPointCount(64, 64);
	earth->setTexture(&earth_texture);
	earth->setRotation(glm::vec2(M_PI));

	Texture sun_texture;
	sun_texture.loadFromFile("Resources/Textures/sun.jpg");

	Sphere* sun = scene += new Sphere;
	sun->setLightningEnabled(false);
	sun->setPosition(glm::vec3(0, 1, 0));
	sun->setPointCount(64, 64);
	sun->setRadius(4);
	sun->setTexture(&sun_texture);

	Light* sunlight = scene += new Light;
	sunlight->setColor("#A8C0E0");
	sunlight->setDiffuseStrength(3);
	sunlight->setSpecularStrength(0);
	sunlight->setAmbientStrength(0);

	Light* moonlight = scene += new Light;
	moonlight->setAmbientStrength(0.1);
	moonlight->setDiffuseStrength(1);
	moonlight->setColor("#ABD1FF");

	Texture moon_texture;
	moon_texture.loadFromFile("Resources/Textures/mercury.jpg");

	Sphere* moon = scene += new Sphere;
	moon->setRadius(1);
	moon->setPointCount(64, 64);
	moon->setTexture(&moon_texture);

	Texture space_texture;
	space_texture.loadFromFile("Resources/Textures/space1.png");

	SpaceSphere* space = scene += new SpaceSphere;
	space->setPointCount(128, 128);
	space->setRadius(100);
	space->setTexture(&space_texture);
	space->setColor(Color::FromBytesRGB(0, 170, 255));
	space->setLightningEnabled(false);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawInterface(window);

		//glClearColor(.05, .05, .05, 1);
		glClearColor(0, 0, 0, 1);
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

		//if (glfwGetKey(window, GLFW_KEY_Q)) plane->setRotation(plane->getRotation() + glm::vec2(0.01, 0));
		//if (glfwGetKey(window, GLFW_KEY_E)) plane->setRotation(plane->getRotation() + glm::vec2(0, 0.01));

		if (glfwGetKey(window, GLFW_KEY_1)) sun->setPosition(sun->getPosition() + glm::vec3( 0.01,  0,     0   ));
		if (glfwGetKey(window, GLFW_KEY_2)) sun->setPosition(sun->getPosition() + glm::vec3(-0.01,  0,     0   ));
		if (glfwGetKey(window, GLFW_KEY_3)) sun->setPosition(sun->getPosition() + glm::vec3( 0,     0.01,  0   ));
		if (glfwGetKey(window, GLFW_KEY_4)) sun->setPosition(sun->getPosition() + glm::vec3( 0,    -0.01,  0   ));
		if (glfwGetKey(window, GLFW_KEY_5)) sun->setPosition(sun->getPosition() + glm::vec3( 0,     0,     0.01));
		if (glfwGetKey(window, GLFW_KEY_6)) sun->setPosition(sun->getPosition() + glm::vec3( 0,     0,    -0.01));

		earth->setPosition(glm::vec3(cos(glfwGetTime()/2)*14, 0, sin(glfwGetTime()/2)*14));

		moon->setPosition(earth->getPosition() + glm::vec3(cos(glfwGetTime())*3, 0, sin(glfwGetTime())*3));
		moon->setRotation(glm::vec2(0, -glfwGetTime()/2));
		moonlight->setPosition(moon->getPosition());

		space->setRotation(glm::vec2(glfwGetTime()/80, glfwGetTime()/72));

		shader["projection"] = projection;
		shader["view"      ] = camera.getView();
		shader["viewPos"   ] = camera.getPosition();
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

		case GLFW_KEY_F2:
		{
			if (action == GLFW_PRESS)
			{
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

				uint8_t* data = new uint8_t[3*framesize.x*framesize.y];
				glReadPixels(0, 0, framesize.x, framesize.y, GL_RGB, GL_UNSIGNED_BYTE, data);

				// Flipping image vertically
				for (size_t y = 0; y < framesize.y/2; y++)
					for (size_t x = 0; x < framesize.x*3; x++)
						std::swap(*(data+framesize.x*y*3+x), *(data+framesize.x*(framesize.y-y-1)*3+x));

				if (SOIL_save_image(path.string().c_str(), SOIL_SAVE_TYPE_BMP, framesize.x, framesize.y, 3, data))
				{
					system((std::string("start") + path.string()).c_str());
					printf("Capture save as '%s'\n", path.string().c_str());
				}
				else printf("Failed to save capture\n");

				delete[] data;
			}
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