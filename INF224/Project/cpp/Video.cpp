#include "Video.h"

Video::Video(const std::string& name, const std::string& filename, int duration)
:Multimedia(name, filename)
{
    this->duration = duration;
}

int Video::getDuration() const{
    return this->duration;
}

void Video::setDuration(){
    this->duration = duration;
}



void Video::play() const{
    std::string cmd = "mpv " + Multimedia::getFilename() + " &";
    const char* cmd_str = cmd.c_str();
    system(cmd_str);
}

void Video::read(std::ostream& out) const {
    Multimedia::read(out);
    out << "\tDuration:" << this->duration << ",";
}
