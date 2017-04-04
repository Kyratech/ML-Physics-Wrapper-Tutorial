//Main functionality
#include "../include/utils.h"

//Physics
#include <PhysicsObjectTypes.h>
#include <PhysicsWorld.h>
#include <HeightfieldData.h>
#include <GravityObject.h>

int main()
{
    //Set up the opengl drawing environment + window
	int startValue = 0;
	if((startValue = startApp()) != 0)
    {
        return startValue;
    }

	//=========================//
	//Set up the physics engine//
	//=========================//

	//Set the world to have 1x gravity and use degrees
	PhysicsWorld* world = new PhysicsWorld(1.0f, false);

	//========================//
	//Add some physics objects//
	//========================//

	//Add your objects to this vector to get them drawn and reset
	std::vector<GameObject> physicsObjects;

	// Create a unit cube directly above the origin of the world
    PhysicsBox defaultBox(world);
    Mesh boxMesh(getBoxVertices(1.0f , 1.0f , 1.0f), getBoxIndices(), glm::vec4(1.0f, 0.3f, 0.0f, 1.0f));
    GameObject boxObject(&boxMesh , &defaultBox , glm::vec3(0.0f));
    physicsObjects.push_back(boxObject);

    // Create an infinite plane in the XZ plane , passing through the world origin
    PhysicsPlane defaultPlane(world);
    Mesh planeMesh(getXZPlaneVertices(20.0f), getPlaneIndices() ,glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    GameObject planeObject(&planeMesh , &defaultPlane , glm::vec3(0.0f));
    physicsObjects.push_back(planeObject);

	//===============//
	//Simulation loop//
	//===============//

	/*Game loop keeps running so that the window doesn't just quit.*/
	while (!glfwWindowShouldClose(window))
	{
	    //Does openGL stuff at the start of the loop
        updateApp(&physicsObjects, world);

		//================//
		// Run simulation //
		//================//

		if(simulationRunning)
			world->stepWorld(deltaTime);

        //Does openGL stuff at the end of the loop
		lateUpdateApp(&physicsObjects, world);
	}

	//========================//
	//Clean up after ourselves//
	//========================//

	delete world;

	//OpenGL cleanup
	endApp();

	return 0;
}
