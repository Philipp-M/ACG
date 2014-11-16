#pragma once
#include <vector>
#include <map>
#include "MotionType.h"
#include "GScene.hpp"
#include "GPolygonObject.hpp"

typedef std::map<int, MotionType> Map;

class Motion {
private:
	std::vector<GPolygonObject*> scene;
	Map assigned_motion;
	int timeSteps;
public:

	Motion(std::vector<GPolygonObject*> scene, int timeSteps);
	~Motion();
	bool assign_motion(int id, Vec goal);			// translation to a goal
	bool assign_motion(int id, float rad,Dir d);	//rotation
	bool assign_motion(int id, Vec t, double acc);	//accelerated translation
	std::vector<GScene*> get_scenes();



};

