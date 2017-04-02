
#include "../include/utils.h"

#include <include/PhysicsWorld.h>

//GUI
#include "../include/ImGUI/imgui.h"
#include "../include/ImGUI/imgui_impl_glfw_gl3.h"

#include "../include/shader.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_movement(GLFWwindow *window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);

const GLuint WIDTH = 1400;
const GLuint HEIGHT = 800;

GLFWwindow *window;

//-----------------//
//Set up the camera//
//-----------------//

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

ThreeD_Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

bool firstMouseInput = true;

//Mouse button flags
bool middleMouse = false;

//Key pressed flags
bool keys[1024];

//For calculating delta time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//Start the program with the GUI visible
bool show_guiWindow = true;
//Start the program with the simulation paused
bool simulationRunning = false;

glm::vec3 globalLightPosition(5.0f, 5.0f, 5.0f);

Shader *ourShader;

int startApp()
{
    //========================//
	//Set up the OpenGL window//
	//========================//

	//Start up glfw
	glfwInit();
	//Using version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Raises errors when trying to use outdated OpenGL functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Take a guess
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Window pointer object holds windowing data
	window = glfwCreateWindow(WIDTH, HEIGHT, "Bullet Test Application", nullptr, nullptr);
	//Exit if the window could not be created
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Thingy to ficus on this window
	glfwMakeContextCurrent(window);

	//Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_movement);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//Use modern techniquies to manage OpenGL functionality
	glewExperimental = GL_TRUE;
	//Exit if GLEW broke
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}

	/*Tell openGL what size the viewport is going to be, and where 0,0 is at (lower-left)
	*Location is on the axis (-1 <= x <= 1, -1 <= y <= 1), so we basically
	*map from (-1, 1) to (0, 800) and (0, 600)*/
	glViewport(0, 0, WIDTH, HEIGHT);

	//Turn on depth testing
	glEnable(GL_DEPTH_TEST);

	//Shader load
	ourShader = new Shader("./shader/vertex_shader.txt", "./shader/fragment_shader.txt");

	//============//
	//Set up ImGUI//
	//============//

	ImGui_ImplGlfwGL3_Init(window, false);

	return 0;
}

int updateApp(std::vector<GameObject> *physicsObjects, PhysicsWorld* world)
{
    //Calculate the time since the last frame
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //Check for events (Eg. keyboard, mouse input)
    //Usually call event processing at the start of a loop iteration
    glfwPollEvents();

    //======================//
    // ImGUI interface code //
    //======================//
    ImGui_ImplGlfwGL3_NewFrame();

    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("GUI Window", &show_guiWindow);

    ImGui::Text("Hello, world!");

    ImGui::Checkbox("Run simulation", &simulationRunning);

    if (ImGui::Button("Reset"))
    {
        for (int obj = 0; obj < physicsObjects->size(); obj++)
        {
            physicsObjects->at(obj).ResetObject();
        }

        //Brief update to move the objects even if simulation isn't running
        world->stepWorld(1 / 1000);
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();



    return 0;
}

int lateUpdateApp(std::vector<GameObject> *physicsObjects, PhysicsWorld* world)
{
    //RENDERING COMMANDS HERE
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader->Use();

    GLint fragmentColourLocation = glGetUniformLocation(ourShader->getShaderProgram(), "fragment_colour");

    //===================================//
    //Create a transformation in 3D space//
    //===================================//

    //Generate the view matrix
    glm::mat4 view;
    view = camera.GetViewMatrix();

    //Generate the projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.Fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);

    // Get their uniform location
    GLint modelLoc = glGetUniformLocation(ourShader->getShaderProgram(), "model");
    GLint viewLoc = glGetUniformLocation(ourShader->getShaderProgram(), "view");
    GLint projLoc = glGetUniformLocation(ourShader->getShaderProgram(), "projection");

    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //==============//
    // Draw objects //
    //==============//

    for(int obj = 0; obj < physicsObjects->size(); obj++)
    {
        physicsObjects->at(obj).DrawObject(modelLoc, *ourShader, globalLightPosition);
    }

    // ImGui functions end here
    ImGui::Render();

    //Swap the colour buffer used to draw, and show output on the screen
    glfwSwapBuffers(window);

    return 0;
}

//Clean out all of the resources we allocated
int endApp()
{
    delete ourShader;
	glfwTerminate();

	return 0;
}

/*
* Record the states of keys when one changes
*/
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Check to see if a new key has been pressed or released
	if (action == GLFW_PRESS)
	{
		keys[key] = true;

		if (key == GLFW_KEY_SPACE)
		{
			simulationRunning = !simulationRunning;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.scroll_input(yoffset);
}

/*
* Record the changes in position of the mouse, use it to update the camera
*/
void mouse_movement(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouseInput)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouseInput = false;
	}

	//Difference between current moose position and previous
	GLfloat deltaX = xPos - lastX;
	GLfloat deltaY = lastY - yPos;

	//Update previous
	lastX = xPos;
	lastY = yPos;

	if (middleMouse)
	{
		if (keys[GLFW_KEY_LEFT_SHIFT])
			camera.pan_camera(deltaX, deltaY);
		else
			camera.move_camera(deltaX, deltaY);
	}
}

/*
* Record the state of the middle mouse when it changes
*/
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		middleMouse = true;
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		middleMouse = false;
}


