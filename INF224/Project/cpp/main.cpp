//
//  main.cpp
//

#include <iostream>
#include "Multimedia.h"
#include "Photo.h"
#include "Video.h"
#include "Film.h"
#include<vector>
#include<string>
#include<fstream>
#include<memory>
#include "Group.h"
#include "MultimediaManager.h"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

using namespace std;
using namespace boost::filesystem;
const string filepath = "testset/photo";

void testExo10();

int main(int argc, char* argv[]) {

    std::cout << "<---------Question 9--------->" << std::endl;

    testExo10();


}

void testExo10(){
    std::cout << "<-----------Test exo 10:---------------->" << std::endl;
    std::cout << "Create Manager..." << std::endl;

    MultimediaManager manager;
    boost::filesystem::path p(filepath);

    std::cerr << "Create Group..." << std::endl;
    manager.createGroup("Photo");
    manager.createGroup("Film");
    manager.createGroup("Video");
    manager.readGroups(std::cout);

    std::cerr << "Create photos...." << std::endl;

    if(boost::filesystem::exists(p)){
        for(auto x: boost::filesystem::directory_iterator(p)){
            manager.createPhoto(x.path().filename().string(), x.path().string(),0,0);
        }
    }

    manager.readMultimedias(std::cout);


}

    //string cmd = " for %a in (\"./testset/mv/*.*\") do ffprobe -v info \"./testset/mv/%a\"  -show_entries format=filename,duration:format_tags=title -print_format csv>>output.cvs";
    //string input = getOutputCmd(cmd);
    //std::cout << inpus:wt << std::endl;

//    /**Get all video**/
//    Group<VideoPtr> videoGroup;
//    getGroupFromFile("./testset/mv", videoGroup);
//    Group<PhotoPtr> photoGroup;
//    getGroupFromFile("./testset/photo", photoGroup);
//    Group<FilmPtr> filmGroup;
//    getGroupFromFile("./testset/film", filmGroup);

//    Group<MultiPtr> multiGroup;
//    multiGroup.insert( multiGroup.end(),videoGroup.begin(), videoGroup.end());
//    multiGroup.insert( multiGroup.end(), photoGroup.begin(),photoGroup.end());
//    multiGroup.insert( multiGroup.end(),filmGroup.begin(),filmGroup.end());

//    std::cout << filmGroup.size() << std::endl;
//    std::cout << videoGroup.size() << std::endl;
//    std::cout << photoGroup.size() << std::endl;
//    std::cout << multiGroup.size() << std::endl;


//    //VideoPtr* vPtrPtr = &filmGroup[2];
//    std::cout<< "multiMedia" << std::endl;
//    multiGroup.clear();
//    std::cout<< "Film" << std::endl;
//    filmGroup.clear();
//    std::cout<< "Video" << std::endl;
//    videoGroup.clear();
//    std::cout<< "Photo" << std::endl;
//    photoGroup.clear();

//    std::cout << "All clear" << std

//    /** Get all Film **/

//	multis.push_back(new Video("Hello", "F:/Downloads/1.mp4",200));
//	multis.push_back(new Photo("Hello2", "C:/Users/zhufa/Desktop/maxresdefault.jpg", 20,20));
//	multis.push_back(new Video("Heloo3","E:/Pictures/320px-Imagej2-icon.png",20));

//	for( auto x: multis){
//		x->read(cout);
//		x->play();
//	}

//    /*** Test Film class ****/
//    cout<< "Test film.... " << std::endl;
//    int durations[5] = {1,5,8,9,7};
//    Film film("Love story", "E:/", 100, 5, durations);
//    film.read(std::cout);

//    durations[1] = 0;
//    film.read(std::cout);

//    Film film2;
//    film2.read(std::cout);
//    film2 = film;
//    film2.read(std::cout);

//    std::cout <<film.getDurationsTable() << std::endl;
//    std::cout <<film2.getDurationsTable() << std::endl;




//    std::cout << "Start" << std::endl;
//        Film* film2= new Film();
//    std::cout << "End" << std::endl;


//}

//template<typename T>
//void getGroupFromFile(string path, Group<std::shared_ptr<T>>& typeGroup){
//    std::vector<_finddata_t> typeFileList = traverseFiles(path, true);

//    for(auto t_info: typeFileList){
//        std::shared_ptr<T> t_ptr(new T(t_info.name, t_info.name));
//        typeGroup.push_back(t_ptr);
//    }
//}


//vector<_finddata_t> traverseFiles(string path, bool fileOnly = true)
//{
//    vector<_finddata_t> fileList;
//    _finddata_t file_info;
//    string current_path= path+"/*.*";
//    int handle=_findfirst(current_path.c_str(),&file_info);

//    do
//    {
//        if(file_info.attrib==_A_SUBDIR && fileOnly)
//            continue;
//        fileList.push_back(file_info);
//    } while(!_findnext(handle,&file_info));

//    return fileList;
//}


//string getOutputCmd(string cmd){
//    string data;
//    FILE * stream;

//    const int max_buffer = 256;
//    char buffer[max_buffer];

//    stream = popen(cmd.c_str(), "r");
//    if (stream) {
//        while (!feof(stream))
//            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
//        pclose(stream);
//    }
//    return data;
//}
