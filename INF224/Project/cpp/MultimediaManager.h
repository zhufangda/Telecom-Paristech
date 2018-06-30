#ifndef MULTIMEDIAMANAGER_H
#define MULTIMEDIAMANAGER_H

#include<map>
#include"Group.h"
#include"Multimedia.h"
#include "Photo.h"
#include "Film.h"
#include "Video.h"
#include <memory>


class MultimediaManager
{
public:
    static const int FILE = 0;
    static const int GROUP = 1;
    static std::shared_ptr<MultimediaManager> getInstance();
    MultimediaManager(){};
    PhotoPtr createPhoto(std::string name,
                         std::string filenamen,
                         float width,
                         float height);
    VideoPtr createVideo(std::string name,
                         std::string filename,
                         int duration);
    FilmPtr createFilm(std::string name,
                       std::string filename,
                       int duration,
                       int table_size,
                       const int* duration_tables);
    GroupPtr createGroup(std::string name);
    MultiPtr searchMultimedia(std::string name);
    GroupPtr searchGroup(std::string name);
    void readMultimedia(std::ostream& os, const std::string& name);
    void readGroup(std::ostream& os, const std::string& name);
    void play(std::string name);
    void addMediaToGroup(std::string mediaName, std::string groupName);
    /**
    * Delete a group or a multimedia in the group
    * @param name name of group or multimedia
    * @param type object type to delete. The type could be FILE or GROUP
    * @return true if object is delete, false if not.
    */
    bool remove(std::string name, int type);

    /**
     * show all the multimedia in this manager
     * @param os the output stream
     */
    void readMultimedias(std::ostream& os, bool showDetail = false);

    /**
     * show all the group in this manager
     * @param os the output stream
     */
    void readGroups(std::ostream& os);



private:
    std::map<std::string, MultiPtr> mFileMap;
    std::map<std::string, GroupPtr> mGroupMap;
};

using MultimediaManagerPtr = std::shared_ptr<MultimediaManager>;
#endif // MULTIMEDIAMANAGER_H
