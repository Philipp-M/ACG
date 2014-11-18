#include "Motion.hpp"
#include "MotionType.hpp"
#include <map>


Motion::Motion(std::vector<GPolygonObject *> _scene,int _timeSteps) : scene(_scene), timeSteps(_timeSteps)  {}


Motion::~Motion() {

}

bool Motion::assign_motion(uint32_t id, Vec3 goal) {			// translation to a goal
	if( scene.size() <= id )
		return false;
	else 	
		assigned_motion[id] = MotionType(Translation, goal);
	return true;
}
bool Motion::assign_motion(uint32_t id, float rad, Dir d) {	//rotation
	if( scene.size() <= id )
		return false;
	else
		assigned_motion[id] = MotionType(Rotation, rad, d);
	return true;
}
bool Motion::assign_motion(uint32_t id, Vec3 t, double acc) {	//accelerated translation
	if( scene.size() <= id )
		return false;
	else
		assigned_motion[id] = MotionType(Acc, t, acc);
	return true;
}

std::vector<GScene*> Motion::get_scenes() {


	std::vector<GScene*> scenes;

	for( int j = 0; j < timeSteps; ++j) {
		//make fresh copy
		std::vector<GPolygonObject*> sceneObjT;
		for( GPolygonObject* obj : scene )
			sceneObjT.push_back(new GPolygonObject(*obj));

		//go over all assigned motions
		for( size_t i = 0; i < scene.size(); ++i ) {
			std::map<int, MotionType>::iterator it = assigned_motion.find(i);
			
			if( !( it== assigned_motion.end()) ) {
				MotionType m = it->second;
				switch( m.get_type() ) {
				case Translation:
					sceneObjT[i]->translate((m.get_goal() / timeSteps)*j);
					break;
				case Rotation:
					switch( m.get_dir()) {
					case X:
						
						sceneObjT[i]->rotationX(m.get_rad());
						break;
					case Y:
						sceneObjT[i]->rotationY(m.get_rad());
						break;
					case Z:
						sceneObjT[i]->rotationZ(m.get_rad());
						break;
					}
					break;
				case Acc:
					sceneObjT[i]->translateAcc(m.get_t(), m.get_acc(), j);
					break;
				}
			}


		}
		
		GScene* scene = new GScene();
		for( GPolygonObject* obj : sceneObjT ) {
			scene->addItem(obj);

		}
		scenes.push_back(scene);
	}
	
	return scenes;

}
