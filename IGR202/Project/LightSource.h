#ifndef __LIGHT_SOURCE_H
#define __LIGHT_SOURCE_H
#include "Vec3.h"

class LightSource {
private:
	Vec3f position;
	Vec3f color;
public:
	inline LightSource() {};
	inline LightSource(float x, float y, float z, float r, float g, float b) {
		position.init(x, y, z);
		color.init(r,g,b);
	}

	inline Vec3f getPosition() const {
		return position;
	}

	Vec3f getColor() const {
		return color;
	}

	void setPoisiton(float x, float y, float z) {
		position.init(x, y, z);
	}
	void setColor(float r, float g, float b) {
		color.init(r, g, b);
	}
};

#endif // 