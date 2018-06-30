#include<iostream>
#include<string>
#include "Multimedia.h"
#include "Video.h"
#include "Photo.h"

using namespace std;

Multimedia::Multimedia(std::string name, std::string filename){
	this->name = name;
	this->filename = filename;
}


string Multimedia::getName() const {return this->name;}
string Multimedia::getFilename() const {return this->filename;}
void Multimedia::setName(string name) { this->name = name;}
void Multimedia::setFilename(string filename) {this->filename = filename;}

void Multimedia::read(ostream& out) const{
    out << "Name:" << this->name <<"," <<"\tFilename:" << this->filename <<",";
}




