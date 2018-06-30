#ifndef GROUP_H
#define GROUP_H

#include<string>
#include<list>
#include<memory>
#include <iostream>
#include"Multimedia.h"

class Group : public std::list<MultiPtr>
{
public:
    Group(){}
    Group(std::string name);
    virtual std::string getName() const;
    virtual void read(std::ostream& out) const;
private:
    std::string name;
};

using GroupPtr = std::shared_ptr<Group>;

#endif // GROUP_H
