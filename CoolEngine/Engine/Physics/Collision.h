#pragma once
#include <math.h>
#include "Engine/GameObjects/GameObject.h"

class Box;
class Circle;
class OBB;

class Collision
{
public:
	/// <summary>
	/// Return true if two boxes overlap, otherwise false
	/// </summary>
	/// <param name="box1"></param>
	/// <param name="box2"></param>
	/// <returns></returns>
	static bool BoxCollision(Box* box1, Box* box2);

	/// <summary>
	/// Calculates if two circles overlap
	/// </summary>
	/// <param name="circle1"></param>
	/// <param name="circle2"></param>
	/// <returns></returns>
	static bool CircleCollision(Circle* circle1, Circle* circle2);

	/// <summary>
	/// Return true if box and circle overlap, otherwise false
	/// </summary>
	/// <param name="circle"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CircleBoxCollision(Circle* circle, Box* box);

	/// <summary>
	/// Return true if two oriented boxes overlap, otherwise false
	/// </summary>
	/// <param name="obb1"></param>
	/// <param name="obb2"></param>
	/// <returns></returns>
	static bool OBBCollision(OBB* obb1, OBB* obb2);

	// I know this is terrible, but all of these functions assume the 1st item is the player, so the 2nd one will not move as a result of the collision!
	// I don't dare change this until I know exactly what we want to do with these functions.
	// If you need this changed in a specific way right now, tell me!

	/// <summary>
	/// If two boxes overlap, move them apart
	/// </summary>
	/// <param name="player"></param>
	/// <param name="object"></param>
	/// <returns></returns>
	static bool BoxCollisionAndResponse(Box* player, Box* object);

	/// <summary>
	/// If circle and box overlap, move circle
	/// </summary>
	/// <param name="circle"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CircleBoxCollisionAndResponse(Circle* circle, Box* box);

	/// <summary>
	/// If two circles overlap, move them apart
	/// </summary>
	/// <param name="circle1"></param>
	/// <param name="circle2"></param>
	/// <returns></returns>
	static bool CircleCollisionAndResponse(Circle* circle1, Circle* circle2);

	static void UpdateOBBs(std::vector<GameObject*> gameObjectList);

	/// <summary>
	/// Check for collisions between all objects in scene
	/// </summary>
	/// <param name="gameObjectMap"></param>
	static void Update(vector<GameObject*> gameObjectMap);
};