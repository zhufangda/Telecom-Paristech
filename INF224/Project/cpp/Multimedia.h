#ifndef _MULTIMEDIA_H
#define _MULTIMEDIA_H

#include<memory>
#include<iostream>
#include<string>

class Multimedia{
private:
	/** Le nom de l'object multimédia**/
	std::string name;
	/** chemin complet de permettant d'accéder à ce fichier **/
	std::string filename;
public:
	inline Multimedia(){}
	Multimedia(std::string name, std::string filename);

    virtual inline ~Multimedia(){
        std::cout << this->name << " will be destroied!" << std::endl;
    }

	/**
	* Returns the multimedia name
	*@return the multimedia name
	**/
	std::string getName() const;
	/**
	* Returns the chemin complet de permettant d'accéder à ce fichier
	* @return the chemin complet de permettant d'accéder à ce fichier
	**/
	std::string getFilename() const;
	void setName(std::string name);
	void setFilename(std::string name);
	/**
	* Print the multimedia info in a ostream object
	* @param out the ostream object used to print the object
	**/
	virtual void read(std::ostream& out) const;

	/**
	* Show the multimedia
	*/
	virtual void play() const = 0;
};

using MultiPtr = std::shared_ptr<Multimedia>;

#endif
