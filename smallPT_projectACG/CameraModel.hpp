/*
 * CameraModel.hpp
 *
 *  Created on: Nov 10, 2014
 *      Author: steve
 */

#ifndef CAMERAMODEL_HPP_
#define CAMERAMODEL_HPP_

#include "DataTypes.hpp"

class CameraModel {
public:
	CameraModel(Vec _camPosition, Vec _camDirection, double _fov, double _focalDistance, double _apertureRadius, unsigned int _width, unsigned int _height);
	Ray generateRay(int x, int y);
	virtual ~CameraModel();
private:
	void initCamera();
	double generateRandomeNumber(double min, double max);

	Vec camPosition, camDirection;
	Vec camLookAt, u, v;
	Vec viewBottomLeft;
	Vec cx, cy;
	Vec apertureX, apertureY;
	double fov, focalDistance, apertureRadius;
	unsigned int width, height;
};

#endif /* CAMERAMODEL_HPP_ */
