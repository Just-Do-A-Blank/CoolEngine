#pragma once
#include <math.h>
#include "Engine/GameObjects/GameObject.h"
#include "Engine/Physics/Line.h"

class Box;
class Circle;

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
	/// Return true if two box and circle overlap, otherwise false
	/// </summary>
	/// <param name="circle"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CircleBoxCollision(Circle* circle, Box* box);

	/// <summary>
	/// Return true if line and box overlap, otherwise false.
	/// Only works for axis aligned lines right now!!! So, lines straight up or down, as with a tile grid.
	/// </summary>
	/// <param name="line"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool LineBoxCollision(Line line, Box* box);

	static bool BoxCollisionAndResponse(Box* player, Box* object);

	static bool CircleBoxCollisionAndResponse(Circle* circle, Box* box);

	static bool CircleCollisionAndResponse(Circle* circle1, Circle* circle2);

	static void Update(unordered_map<string, GameObject*> gameObjectMap);
};