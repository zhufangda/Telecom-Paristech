#include "Multimedia.h"
#include "Photo.h"

Photo::Photo(std::string name,
    std::string filename,
    float width,
    float height)
    : Multimedia(name, filename)
{
    this->width = width;
    this->height = height;
}

void Photo::setWidth(float width){this->width = width;}
int Photo::getWidth() const { return this->width;}
void Photo::setHeight(float height){this->height = height;}
float Photo::getHeight() const {return this->height;}
void Photo::play() const{
    std::string cmd = "imagej " + Multimedia::getFilename() + " &";
    const char* cmd_str = cmd.c_str();
    std::cout << cmd;
    system(cmd_str);
}

void Photo::read(std::ostream& out)const{
    Multimedia::read(out);
    out << "\tWidth:" << this->width <<","
        << "\tHeight:" << this->height << ",";
}
