/*
 * GeometricScene.cpp
 *
 *  Created on: 22.10.2014
 *      Author: Philipp Mildenberger
 */

#include "GScene.hpp"

GScene::GScene()
{
}

GScene::~GScene()
{
	for(int i = 0; i < objects.size(); i++)
		delete objects[i];
}

void GScene::addItem(GObject* object)
{
	objects.push_back(object);
}
