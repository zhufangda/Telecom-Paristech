#include "MultimediaManager.h"
#include "Group.h"
#include "Video.h"
#include "Film.h"
#include "Photo.h"
#include <string>
#include <memory>
#include <exception>
#include <sstream>

PhotoPtr MultimediaManager::createPhoto(std::string name,
                                        std::string filename,
                                        float width,
                                        float height){
    mFileMap[name] = std::make_shared<Photo>(name, filename, width, height);
    return std::dynamic_pointer_cast<Photo>(mFileMap[name]);
}

VideoPtr MultimediaManager::createVideo(std::string name, std::string filename, int duration){
    mFileMap[name] = std::make_shared<Video>(name, filename, duration);
    return std::dynamic_pointer_cast<Video>(mFileMap[name]);
}

FilmPtr MultimediaManager::createFilm(std::string name,
                                      std::string filename,
                                      int duration,
                                      int table_size,
                                      const int* duration_tables){
    mFileMap[name] = std::make_shared<Film>(name, filename,duration, table_size,duration_tables);
    return std::dynamic_pointer_cast<Film>(mFileMap[name]);
}

GroupPtr MultimediaManager::createGroup(std::string name){
    mGroupMap[name] = std::make_shared<Group>(name);
    return mGroupMap[name];
}

void MultimediaManager::addMediaToGroup(std::string mediaName, std::string groupName){
    auto media = mFileMap.find(mediaName);
    auto group = mGroupMap.find(groupName);

    if(media == mFileMap.end()){
        throw std::invalid_argument("Media " + mediaName + " does not exist!");
    }
    if(group == mGroupMap.end()){
        this->createGroup(groupName);
    }
    mGroupMap[groupName]->push_back(media->second);

}

MultiPtr MultimediaManager::searchMultimedia(std::string name){
    auto ite = mFileMap.find(name);
    if(ite != mFileMap.end()){
        return mFileMap.find(name)->second;
    }else{
        throw std::runtime_error("Can not find the multimedia " + name + "!");
    }
}

GroupPtr MultimediaManager::searchGroup(std::string name){
    auto ite = mGroupMap.find(name);
    if(ite != mGroupMap.end()){
        return mGroupMap.find(name)->second;
    }else{
        throw std::runtime_error("Can not find the group " + name + "!");
    }
}

void MultimediaManager::readMultimedia(std::ostream& os,const std::string& name){
        searchMultimedia(name)->read(os);
}

void MultimediaManager::readGroup(std::ostream& os,const std::string& name){
        searchMultimedia(name)->read(os);
}

void MultimediaManager::play(std::string name){
    searchMultimedia(name)->play();
}

bool MultimediaManager::remove(std::string name, int type){
    if(type == FILE){
        if(mFileMap.erase(name)==0){
            std::stringstream msg;
            msg << "Warning:Multimedia " << name <<" don't exist!"
                << "Can not delete it!";
            throw std::runtime_error(msg.str());
            return false;
        }else{
            if(mGroupMap.erase(name)==0){
            std::stringstream msg;
            msg << "Warning:Group " << name <<" don't exist!"
                << "Can not delete it!";
            throw std::runtime_error(msg.str());
            return false;
            }
        }
    }
    return true;
}

void MultimediaManager::readMultimedias(std::ostream& os, bool showDetail){
    for(auto media: this->mFileMap){
        if(showDetail) media.second->read(os);
        else os << media.second->getName() << ",";
    }
}

void MultimediaManager::readGroups(std::ostream& os){
    for(auto group: this->mGroupMap){
        os << "-----Group Name:" << group.first << "------,";
        group.second->read(os);
    }


}
