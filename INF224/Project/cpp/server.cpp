//
//  server.cpp
//  TP C++
//  Eric Lecolinet - Telecom ParisTech - 2016.
//

#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "MultimediaManager.h"
#include "tcpserver.h"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

using namespace std;
using namespace cppu;

const int PORT = 3331;

class MyBase {
public:
    /* Cette méthode est appelée chaque fois qu'il y a une requête à traiter.
   * Ca doit etre une methode de la classe qui gere les données, afin qu'elle
   * puisse y accéder.
   *
   * Arguments:
   * - 'request' contient la requête
   * - 'response' sert à indiquer la réponse qui sera renvoyée au client
   * - si la fonction renvoie false la connexion est close.
   *
   * Cette fonction peut etre appelée en parallele par plusieurs threads (il y a
   * un thread par client).
   *
   * Pour eviter les problemes de concurrence on peut créer un verrou en creant
   * une variable Lock **dans la pile** qui doit etre en mode WRITE (2e argument = true)
   * si la fonction modifie les donnees.
   */
    bool processRequest(TCPConnection& cnx, const string& request, string& response)
    {
        cerr << "\nRequest: '" << request << "'" << endl;

        // 1) pour decouper la requête:
        // on peut par exemple utiliser stringstream et getline()
        vector<string> reqSequence;
        splitString(request, reqSequence, " ");
        for(auto str: reqSequence){
            cout << str << std::endl;
        }
        // 2) faire le traitement:
        // - si le traitement modifie les donnees inclure: TCPLock lock(cnx, true);
        // - sinon juste: TCPLock lock(cnx);
        stringstream repStream;
        shared_ptr<TCPLock> lock;
        repStream << "Server:,";
        try{
            if(reqSequence.size() == 0){
                cerr << "\n Empty request!" << std::endl;
                repStream << "Empty request";
                return true;
            }else if(reqSequence[0] == "add"){
                lock = make_shared<TCPLock>(cnx, true);
                add(reqSequence, repStream);
            }else if(reqSequence[0] == "search"){
                lock = make_shared<TCPLock>(cnx);
                search(reqSequence, repStream);
            }else if(reqSequence[0] == "play"){
                lock = make_shared<TCPLock>(cnx);
                play(reqSequence, repStream);
            }else if(reqSequence[0] == "remove"){
                lock = make_shared<TCPLock>(cnx, true);
                remove(reqSequence, repStream);
            }else if( reqSequence[0] == "show"){
                lock = make_shared<TCPLock>(cnx);
                show(reqSequence, repStream);
            }else if(reqSequence[0] == "quit"){
                cerr << "Close request" << std::endl;
                response = "Close request";
                return false;
            }else{
                response = "IllegaleCMD:" +  reqSequence[0];
                cerr <<	"IllegaleCMD:" << reqSequence[0] << std::endl;
                cerr << "Close request" << std::endl;
                return false;
            }
        }catch(runtime_error e){
            repStream << e.what();
       }catch(invalid_argument e){
            repStream << e.what();
        }




        // 3) retourner la reponse au client:
        // - pour l'instant ca retourne juste OK suivi de la requête
        // - pour retourner quelque chose de plus utile on peut appeler la methode print()
        //   des objets ou des groupes en lui passant en argument un stringstream
        // - attention, la requête NE DOIT PAS contenir les caractères \n ou \r car
        //   ils servent à délimiter les messages entre le serveur et le client

        response = "" + repStream.str();
        cerr << "response: " << response << endl;

        // renvoyer false si on veut clore la connexion avec le client
        return true;
    }

    void show(vector<string>& seq, stringstream& response){
        if(seq.size() == 1){
          managerPtr->readMultimedias(response, false);
        }else if(seq.size() == 2){
            if(seq[1] == "group"){
                managerPtr->readGroups(response);
            }else{
                auto media = managerPtr->searchMultimedia(seq[1]);
                media->read(response);
            }
            return;
        }else if(seq.size() == 3 && seq[1] == "group"){
          auto group = managerPtr->searchGroup(seq[2]);
          group->read(response);
      }
  }

  void add(vector<string>& seq, stringstream& response)
  {
      if(seq.size() < 1){
          response << "Format invalide";
          return;
      }
      if(seq[1] == "video"){
          if(seq.size() == 5){
            managerPtr->createVideo(seq[2], seq[3], stoi(seq[4]));
            response << "Add Video " << seq[2] << " successfully!" ;
          }
          else{
              response << "Format invalide: add video name filename duration";
              return ;
          }
      }else if(seq[1] == "photo"){
          if(seq.size()==6){
              managerPtr->createPhoto(seq[2], seq[3], stof(seq[4]), stof(seq[5]));
              response << "Add Photo " << seq[2] << " successfully!";
              return ;
          }else{
              response << "Format invalide: add photo name filename width height";
              return;
          }
      }else if(seq[1] == "group"){
          if(seq.size() == 3){
              managerPtr->createGroup(seq[2]);
              response << "Add group " << seq[2] << " successfully";
          }else{
            response << "Format invalid: add group name";
            return;
          }
      }
  }

  void search(const vector<string>& seq, stringstream& response) const{
      if(seq.size() !=3){
          throw invalid_argument("Format invalud: searach group|media name");
      }
      if(seq[1] == "group"){
          auto group = managerPtr->searchGroup(seq[2]);
          group->read(response);
      }else{
          auto media = managerPtr->searchMultimedia(seq[2]);
          media->read(response);
      }
  }

  void remove(const vector<string>& seq, stringstream& response) const{
      if(seq.size() != 3){
        throw invalid_argument("Format invalid: remove group|file name");
        return;
      }
    bool res;
    if(seq[1] == "group"){
        res = managerPtr->remove(seq[2], MultimediaManager::GROUP);
    }else{
        res = managerPtr->remove(seq[2], MultimediaManager::FILE);
    }

    if(res){
        response << "Remove " << seq[2] << " successfully!";
    }else{
        response << "Can not remove " << seq[2] << ".";
    }
  }

  void play(const vector<string>& seq, stringstream& response) const{
      if(seq.size() <= 1 ){
        throw runtime_error("Format of cmd is not correct!");
      }

    managerPtr->play(seq[1]);
    response << "Start to playing " << seq[1];

  }

  void splitString(const std::string& s, std::vector<std::string>& v, const std::string& delim)
  {
    std::string::size_type pos1, pos2;
    pos2 = s.find(delim);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
      if(pos1 != pos2){
        v.push_back(s.substr(pos1, pos2-pos1));
      }

      pos1 = pos2 + delim.size();
      pos2 = s.find(delim, pos1);
    }
    if(pos1 != s.length())
      v.push_back(s.substr(pos1));
  }

  void setManager(MultimediaManagerPtr mangerPtr){
      this->managerPtr = mangerPtr;
  }

 private:
    MultimediaManagerPtr managerPtr = managerPtr;
};


int main(int argc, char* argv[])
{

    boost::filesystem::path full_path(boost::filesystem::current_path());
    std::cout << "Current path is : " << full_path << std::endl;
    //Initialisation de base de donnée
    string path = full_path.string() + "/testset";
    MultimediaManagerPtr manager(new MultimediaManager());
	
    if(boost::filesystem::exists(path + "/video")){
        auto videoGroup = manager->createGroup("Video");
        for(auto x: boost::filesystem::directory_iterator(path + "/video")){
            auto video = manager->createVideo(x.path().filename().string(), x.path().string(), 25);
            manager->addMediaToGroup(video->getName(), videoGroup->getName());
        }
    }else{
        std::cout << "Vedio set do not existe, can not load the video list in database!"
                  << std::endl;
    }

    if(boost::filesystem::exists(path + "/photo")){
        auto photoGroup = manager->createGroup("Photo");
        for(auto x: boost::filesystem::directory_iterator(path + "/photo")){
            auto photo = manager->createPhoto(x.path().filename().string(), x.path().string(),100,200);
            manager->addMediaToGroup(photo->getName(), photoGroup->getName());
        }
    }else{
        std::cout << "Video set do not existe, can not load the video list in database!"
                  << std::endl;
    }


  // cree le TCPServer
  shared_ptr<TCPServer> server(new TCPServer());
  
  // cree l'objet qui gère les données
  shared_ptr<MyBase> base(new MyBase());


  base->setManager(manager);

  // le serveur appelera cette méthode chaque fois qu'il y a une requête
  server->setCallback(*base, &MyBase::processRequest);
  
  // lance la boucle infinie du serveur
  cout << "Starting Server on port " << PORT << endl;
  int status = server->run(PORT);
  
  // en cas d'erreur
  if (status < 0) {
    cerr << "Could not start Server on port " << PORT << endl;
    return 1;
  }
  
  return 0;
}

