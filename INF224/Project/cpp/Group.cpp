#include<string>
#include<list>
#include<memory>
#include"Multimedia.h"
#include "Group.h"
Group::Group(std::string name)
{
    this->name = name;
}

std::string Group::getName() const{
    return this->name;
}

void Group::read(std::ostream& out) const{
    out << "Group name:" << this->name <<",";
    for(auto element: *this){
        element->read(out);
    }
}



