/*
 * CameraModel.hpp
 *
 *  Created on: Nov 10, 2014
 *      Author: steve
 */

#ifndef CAMERAMODEL_HPP_
#define CAMERAMODEL_HPP_

#include "DataTypes.hpp"
#include "GScene.hpp"

class CameraModel {
public:
	CameraModel(Vec _camPosition, Vec _camDirection, double _fov, double _focalDistance, double _apertureRadius, unsigned int _width, unsigned int _height);

	virtual ~CameraModel();

	Ray generateRay(int x, int y, int sx, int sy, double dx, double dy, unsigned short *Xi) const;

private:
	void initCamera();
	double generateRandomNumber() const;

	Vec camPosition, camDirection;
	Vec camLookAt, u, v;
	Vec viewBottomLeft;
	Vec cx, cy;
	Vec apertureX, apertureY;
	double fov, focalDistance, apertureRadius;
	unsigned int width, height;
};

#endif /* CAMERAMODEL_HPP_ */
