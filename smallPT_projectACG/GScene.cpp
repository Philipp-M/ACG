#include "GScene.hpp"
#include "GSimpleAccelStruct.hpp"
#include "GBVHAccelStruct.hpp"

GScene::GScene()
{
	accelStruct = new GSimpleAccelStruct(objects);
}

GScene::~GScene()
{
	for(size_t i = 0; i < objects.size(); i++)
		delete objects[i];
	delete accelStruct;
}

void GScene::addItem(GObject* object, bool update)
{
	objects.push_back(object);
	if(update)
	{
		delete accelStruct;
		accelStruct = new GBVHAccelStruct(objects);
	}
}
