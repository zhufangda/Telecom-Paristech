#ifndef _PHOTO_H
#define _PHOTO_H
#include <cstdlib>
#include <iostream>
#include <memory>
#include "Multimedia.h"

class Photo: public Multimedia{
private:
	float width = 0.0f;
	float height = 0.0f; 
public:
    Photo(std::string name, std::string filename):Multimedia(name, filename){}
	Photo(std::string name, 
		std::string filename, 
		float width, 
        float height);

    virtual void setWidth(float width);
    virtual int getWidth() const;
    virtual void setHeight(float height);
    virtual float getHeight() const;
    virtual void play() const;
    virtual void read(std::ostream& out)const override;
};

using PhotoPtr = std::shared_ptr<Photo>;

#endif
