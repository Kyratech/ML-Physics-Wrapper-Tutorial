//Main functionality
#include "../include/utils.h"

//Physics
#include <include/PhysicsObjectTypes.h>
#include <include/PhysicsWorld.h>
#include <include/HeightfieldData.h>
#include <include/GravityObject.h>

void callbackStartTest(PhysicsCollision collision);

PhysicsBox* bigBox;

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

	PhysicsWorld* world = new PhysicsWorld(1.0f, false);

	std::vector<GameObject> physicsObjects;

	bigBox = new PhysicsBox(world);

	bigBox->setCollisionID(1);
	Mesh bigBoxMesh(getCubeVertices(1.0), getCubeIndices(), glm::vec4(1.0f, 0.3f, 0.0f, 1.0f));
	GameObject bigBoxObject(&bigBoxMesh, glm::vec3(0.0f, 0.0f, 0.0f), bigBox);
	physicsObjects.push_back(bigBoxObject);

	PhysicsPlane defaultPlane(world);
	defaultPlane.setCollisionID(2);
	Mesh planeMesh(getXZPlaneVertices(20.0f), getPlaneIndices(), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
	GameObject planeObject(&planeMesh, glm::vec3(0.0f, 0.0f, 0.0f), &defaultPlane);
	physicsObjects.push_back(planeObject);

	world->setCollisionStartFunction(callbackStartTest);

	//===============//
	//Simulation loop//
	//===============//

	/*Game loop keeps running so that the window doesn't just quit.*/
	while (!glfwWindowShouldClose(window))
	{
        updateApp(&physicsObjects, world);

		//================//
		// Run simulation //
		//================//

		//60 simulation steps per second
		if(simulationRunning)
			world->stepWorld(1 / 60.0f);

		lateUpdateApp(&physicsObjects, world);
	}

	delete world;

	endApp();

	return 0;
}


void callbackStartTest(PhysicsCollision collision)
{
	//Collision between box and ground
	if (collision.contains(1, 2))
	{
		bigBox->applyImpulse(collision.getNormal() * 30.0f);
	}
}

