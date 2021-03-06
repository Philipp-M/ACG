#ifndef MOTION_HPP
#define MOTION_HPP
#include <vector>
#include <map>
#include "MotionType.hpp"
#include "GScene.hpp"
#include "GPolygonObject.hpp"
#include "GTexturedTriangle.hpp"

typedef std::map<int, MotionType> Map;

/**
*	This class is used to construct the discrete time steps needed for the MotionBlur. 
*/
class Motion {
private:
	std::vector<GPolygonObject<GTexturedTriangle>*> scene;
	Map assigned_motion;	//maps object to MotionType
	int timeSteps;			//number of discrete time steps

public:

	Motion(std::vector<GPolygonObject<GTexturedTriangle>*> scene, int timeSteps);
	~Motion();
	bool assign_motion(uint32_t id, Vec3 goal);				// translation to a goal
	bool assign_motion(uint32_t id, float rad,Dir d);		//rotation
	bool assign_motion(uint32_t id, Vec3 t, double acc);	//accelerated translation
	std::vector<GScene*> get_scenes();						//compute final time steps and return all scenes



};
#endif // MOTION_HPP
