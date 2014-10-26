#include "GScene.hpp"
#include "GSimpleAccelStruct.hpp"

GScene::GScene()
{
	accelStruct = new GSimpleAccelStruct(objects);
}

GScene::~GScene()
{
	for(int i = 0; i < objects.size(); i++)
		delete objects[i];
	delete accelStruct;
}

void GScene::addItem(GObject* object, bool update)
{
	objects.push_back(object);
	if(update)
	{
		delete accelStruct;
		accelStruct = new GSimpleAccelStruct(objects);
	}
}
