#ifndef _VEDIO_H
#define _VEDIO_H
#include <cstdlib>
#include "Multimedia.h"
#include <iostream>
#include <memory>
class Video: public Multimedia{
private:
	int duration = 0;
public:
    Video(){};
    Video(const std::string& name, const std::string& filename):Multimedia(name, filename){};
    Video(const std::string& name, const std::string& filename, int duration);
    virtual int getDuration() const;
    virtual void setDuration();
    virtual void play()const override;
    virtual void read(std::ostream& out)const override;
};

using VideoPtr = std::shared_ptr<Video>;
#endif
