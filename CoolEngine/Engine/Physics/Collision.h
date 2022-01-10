#pragma once
#include "Box.h"
#include "Line.h"
#include "Circle.h"
#include <math.h>

class Collision
{
public:
	/// <summary>
	/// Return true if two boxes overlap, otherwise false
	/// </summary>
	/// <param name="box1"></param>
	/// <param name="box2"></param>
	/// <returns></returns>
	static bool BoxCollision(Box box1, Box box2);

	/// <summary>
	/// Calculates if two circles overlap
	/// </summary>
	/// <param name="circle1"></param>
	/// <param name="circle2"></param>
	/// <returns></returns>
	static bool CircleCollision(Circle circle1, Circle circle2);

	/// <summary>
	/// Return true if line and box overlap, otherwise false.
	/// Only works for axis aligned lines right now!!!
	/// </summary>
	/// <param name="line"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool LineBoxCollision(Line line, Box box);
};