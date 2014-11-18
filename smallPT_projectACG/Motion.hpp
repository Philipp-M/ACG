#pragma once
#include <vector>
#include <map>
#include "MotionType.hpp"
#include "GScene.hpp"
#include "GPolygonObject.hpp"
#include "GTexturedTriangle.hpp"

typedef std::map<int, MotionType> Map;

class Motion {
private:
	std::vector<GPolygonObject<GTexturedTriangle>*> scene;
	Map assigned_motion;
	int timeSteps;
public:

	Motion(std::vector<GPolygonObject<GTexturedTriangle>*> scene, int timeSteps);
	~Motion();
	bool assign_motion(uint32_t id, Vec3 goal);			// translation to a goal
	bool assign_motion(uint32_t id, float rad,Dir d);	//rotation
	bool assign_motion(uint32_t id, Vec3 t, double acc);	//accelerated translation
	std::vector<GScene*> get_scenes();



};

