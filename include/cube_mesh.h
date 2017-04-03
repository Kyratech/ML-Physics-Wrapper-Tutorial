#ifndef CUBE_MESH_H
#define CUBE_MESH_H

#include "Tester.h"

/*
* Produces a vector of vertex locations for constructing a cube
* Cube is centred around local origin
		 ____________
		/|			/|			y
	   / |		   / |			^	z
	  /	 |		  /	 |			|  ¬
	 /___________/   |			| /
	|	 |__O___|____|			|/____> x
	|	/		|	/			O
	|  /		|  /
	| /			| /
	|/__________|/

*/
static std::vector<struct Vertex> getBoxVertices(float width, float height, float depth)
{
	//Centred around local (0,0), the vertices have xyz locations at +/- half the side length
	float halfWidth = width / 2.0;
	float halfHeight = height / 2.0;
	float halfDepth = depth / 2.0;

	std::vector<struct Vertex> boxVerts = {
		//Front
		{{-halfWidth,	-halfHeight,	-halfDepth},	{0.0f, 0.0f, -1.0f}},	//Front bottom left
		{{halfWidth,    -halfHeight,	-halfDepth},	{0.0f, 0.0f, -1.0f}},	//Front bottom right
		{{halfWidth,    halfHeight,	    -halfDepth},	{0.0f, 0.0f, -1.0f}},	//Front top right
		{{-halfWidth,	halfHeight,	    -halfDepth},	{0.0f, 0.0f, -1.0f}},	//Front top left

		//Right
		{{halfWidth,	-halfHeight,	-halfDepth},	{1.0f, 0.0f, 0.0f}},	//Front bottom right
		{{halfWidth,	halfHeight,	    -halfDepth},	{1.0f, 0.0f, 0.0f}},	//Front top right
		{{halfWidth,	halfHeight,	    halfDepth},	    {1.0f, 0.0f, 0.0f}},	//Back top right
		{{halfWidth,	-halfHeight,	halfDepth},	    {1.0f, 0.0f, 0.0f}},	//Back bottom right

		//Back
		{{-halfWidth,	-halfHeight,	halfDepth},	    {0.0f, 0.0f, 1.0f}},	//Back bottom left
		{{halfWidth,	-halfHeight,	halfDepth},	    {0.0f, 0.0f, 1.0f}},	//Back bottom right
		{{halfWidth,	halfHeight,	    halfDepth},	    {0.0f, 0.0f, 1.0f} },	//Back top right
		{{-halfWidth,	halfHeight,	    halfDepth},	    {0.0f, 0.0f, 1.0f}},	//Back top left

		//Left
		{{-halfWidth,	-halfHeight,	-halfDepth},	{-1.0f, 0.0f, 0.0f}},	//Front bottom left
		{{-halfWidth,	-halfHeight,	halfDepth},	    {-1.0f, 0.0f, 0.0f}},	//Back bottom left
		{{-halfWidth,	halfHeight,	    -halfDepth},	{-1.0f, 0.0f, 0.0f}},	//Front top left
		{{-halfWidth,	halfHeight,	    halfDepth},	    {-1.0f, 0.0f, 0.0f}},	//Back top left

		//Bottom
		{{-halfWidth,	-halfHeight,	-halfDepth},	{0.0f, -1.0f, 0.0f}},	//Front bottom left
		{{halfWidth,	-halfHeight,	-halfDepth},	{0.0f, -1.0f, 0.0f}},	//Front bottom right
		{{-halfWidth,	-halfHeight,	halfDepth},	    {0.0f, -1.0f, 0.0f}},	//Back bottom left
		{{halfWidth,	-halfHeight,	halfDepth},	    {0.0f, -1.0f, 0.0f}},	//Back bottom right

		//Top
		{{halfWidth,	halfHeight,	    -halfDepth},	{0.0f, 1.0f, 0.0f}},	//Front top right
		{{-halfWidth,	halfHeight,	    -halfDepth},	{0.0f, 1.0f, 0.0f}},	//Front top left
		{{halfWidth,	halfHeight,	    halfDepth},	    {0.0f, 1.0f, 0.0f}},	//Back top right
		{{-halfWidth,	halfHeight,	    halfDepth},	    {0.0f, 1.0f, 0.0f}}	    //Back top left
	};

	return boxVerts;
}

/*
* Produces list of indices for OpenGL to construct a cube from the above vertex locations
* Note that this implementation depends on the order of vertices above
*/
static std::vector<GLuint> getBoxIndices()
{
	std::vector<GLuint> boxInds = {
		0, 1, 2,
		0, 2, 3,	//Front
		4, 5, 6,
		4, 6, 7,	//Right
		8, 9, 11,
		9, 10, 11,	//Back
		12, 13, 14,
		13, 14, 15,	//Left
		16, 17, 19,
		16, 18, 19,	//Bottom
		20, 21, 23,
		20, 22, 23	//Top
	};

	return boxInds;
}

#endif
