#pragma once
#include "Point.hpp"
#include <vector>
#include <cmath>

template<typename T>
class DrainagePolygon
{
private:
	std::vector<Point2<T> > vertices;
public:
	/**
	 * splits the Polygon based on a line defined by two Points, it does just split it in two parts.
	 * If there are other intersections with the line only the nearest intersections to the points are taken
	 * It's kind of a brute force approach, since it's not the subject to do most efficient code
	 *
	 * @return true on success(there is a right and left polygon)
	 */
	bool split(Point2<T> first, Point2<T> secnd, DrainagePolygon<T>& left, DrainagePolygon<T>& right);
	/**
	 * splits the Polygon based on a line defined by a start Point and an angle, for the rest see above
	 */
	bool split(Point2<T> start, T angle, DrainagePolygon<T>& left, DrainagePolygon<T>& right);
	bool isEmpty() { return vertices.size() == 0; }
	void clear() { vertices.clear(); };
};

template<typename T>
bool DrainagePolygon<T>::split(Point2<T> first, Point2<T> secnd, DrainagePolygon<T>& left, DrainagePolygon<T>& right)
{
	if(!isEmpty())
	{
		std::vector<Point2<T> > intersections;
		const Point2<T>& lastP = vertices[0];
		for(size_t i = 1; i < vertices.size(); i++ )
		{
			const Point2<T>& newP = vertices[i];
			T denom = (first.x-secnd.x)*(lastP.y-newP.y) -
					(first.y-secnd.y)*(lastP.x-newP.x);
			if(denom == 0) // parallel
				continue;
			// calculate the intersection of the lines
			T x = ((first.x*secnd.y-first.y*secnd.x)*(lastP.x-newP.x) - (first.x-secnd.x)*(lastP.x*newP.y-lastP.y*newP.x))/denom;
			T y = ((first.x*secnd.y-first.y*secnd.x)*(lastP.y-newP.y) - (first.y-secnd.y)*(lastP.x*newP.y-lastP.y*newP.x))/denom;
			// check if the intersection is in the line segment
			bool xBetw = false, yBetw = false;
			if((x < lastP.x && x > newP.x) || (x > lastP.x && x < newP.x) || (y == lastP. == newP.y))
				xBetw = true;
			if((y < lastP.y && y > newP.y) || (y > lastP.y && y < newP.y) || (y == lastP.y == newP.y))
				yBetw = true;
		}
	}
}

template<typename T>
bool DrainagePolygon<T>::split(Point2<T> start, T angle, DrainagePolygon<T>& left, DrainagePolygon<T>& right)
{
	split(start, Point2(cos(angle), sin(angle)), left, right);
}
