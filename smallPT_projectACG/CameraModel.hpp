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

	const Vec& getCamDirection() const {
		return camDirection;
	}

	const Vec& getCamLookAt() const {
		return camLookAt;
	}

	const Vec& getCamPosition() const {
		return camPosition;
	}

	const Vec& getCx() const {
		return cx;
	}

	const Vec& getCy() const {
		return cy;
	}

	const Vec& getViewBottomLeft() const {
		return viewBottomLeft;
	}

	Vec colorOnePixel(int x, int y, GScene* scene, int depth, unsigned short *Xi) const;

private:
	void initCamera();
	double generateRandomeNumber(double min, double max) const;
	Ray generateRay(int x, int y) const;

	Vec camPosition, camDirection;
	Vec camLookAt, u, v;
	Vec viewBottomLeft;
	Vec cx, cy;
	Vec apertureX, apertureY;
	double fov, focalDistance, apertureRadius;
	unsigned int width, height;
};

#endif /* CAMERAMODEL_HPP_ */
