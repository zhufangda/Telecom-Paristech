#include "Film.h"

Film::Film(const Film& film): Video(film){
    this->size = film.size;
    this->durations_table = new int[size];

    for(int i =0; i< size; i++){
        this->durations_table[i] = film.durations_table[i];
    }
}

Film::Film(const std::string& name,
                const std::string& filename,
                int duration,
                int table_size,
                const int* durations_tables):
    Video(name, filename, duration){
    this->durations_table = new int[table_size];
    this->size = table_size;

    for(int i=0; i<this->size; i++){
        this->durations_table[i] = durations_tables[i];
    }
}


const int Film::getNbChaitre() const{
    return this->size;
}

const int* Film::getDurationsTable() const{
    return this->durations_table;
}

Film::~Film(){

    if(this->durations_table != nullptr){
        delete durations_table;
    }
}

void Film::setDurationsTable(const int* durations, int size){
    if(this->durations_table!= nullptr){
        delete this->durations_table;
    }

    this->durations_table = new int[size];
    this->size = size;
    for(int i =0; i< size; i++){
        this->durations_table[i] = durations[i];
    }

}

void Film::read(std::ostream& out) const{
    Video::read(out);
    out << "Chapitre:," ;
    for(int i=0; i<this->size; i++){
        out <<"\tChapitre " << i << ":"
           << this->durations_table[i]
           << " Second" << ",";
    }
}

void Film::operator =(const Film& film){
   Video::operator=(film);
   setDurationsTable(film.getDurationsTable(),film.getNbChaitre());
}




