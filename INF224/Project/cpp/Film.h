#ifndef FILM_H
#define FILM_H

#include<string>
#include <iostream>
#include<memory>
#include "Video.h"

class Film: public Video
{
public:
    Film(){};
    Film(std::string name, std::string filename):Video(name, filename){}
    Film(const Film& film);
    Film(const std::string& name,
                    const std::string& filename,
                    int duration,
                    int table_size,
                    const int* durations_table);
    void setDurationsTable(const int* durations, int size);
    const int getNbChaitre() const;
    virtual const int* getDurationsTable() const;
    virtual ~Film();
    virtual void read(std::ostream& out) const override;
    virtual void operator=(const Film& film);
private:
    int* durations_table = nullptr;
    int size = 0;
};
using FilmPtr = std::shared_ptr<Film>;
#endif // FILM_H
