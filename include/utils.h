#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

#include "../include/3d_camera.h"
#include "../include/mesh.h"
#include "../include/cube_mesh.h"
#include "../include/plane_mesh.h"
#include "../include/cone_mesh.h"
#include "../include/cylinder_mesh.h"
#include "../include/terrain_mesh.h"
#include "../include/uv_sphere_mesh.h"
#include "../include/triangle_mesh.h"
#include "../include/game_object.h"

#include <vector>

//Keep track of whether the simulation is stepping
extern bool simulationRunning;

//GLFW Window
extern GLFWwindow *window;

int startApp();
int updateApp(std::vector<GameObject> *physicsObjects, PhysicsWorld* world);
int lateUpdateApp(std::vector<GameObject> *physicsObjects, PhysicsWorld* world);
int endApp();

