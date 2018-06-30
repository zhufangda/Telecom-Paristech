#pragma once
#include<vector>
#include "Vec3.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ray.h"

class AxisAlignedBoundingBox
{
private:
	float x[3];
	float y[3];
	float z[3];
	std::vector<Vec3f> m_positions;
	unsigned int m_indices[24] = {
		0, 1, 2, 3, 4, 5, 6, 7,
		2, 6, 3, 7, 1, 5, 0, 4,
		2, 0, 6, 4, 7, 5, 3, 1};

public:
	AxisAlignedBoundingBox();
	void init(const float & xMax, const float & xMin, const float & yMax, const float & yMin, const float & zMax, const float & zMin);
	std::vector<Vec3f> positions();
	unsigned int* indices();
	float getZmin() const;
	void drawBox(const float width) const ;
	bool contain(const Vec3f& point) const;
	bool contain(const Ray& ray) const;
	bool intersection(const Ray & r) const;
	~AxisAlignedBoundingBox();
	friend std::ostream & operator<<(std::ostream & output, const AxisAlignedBoundingBox& aabb) ;
};

