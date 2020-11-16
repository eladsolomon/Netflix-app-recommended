#include <iostream>
#include "../include/Watchable.h"
#include "../include/Session.h"
#include <string>

using namespace std;

//**************Watchable**************************
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id) , length(length),tags(tags) {}
Watchable:: ~Watchable() {}
std::string Watchable::toString() const {return nullptr;}
Watchable* Watchable::clone() {
return nullptr;
}
int Watchable::getLength() const{
    return this->length;
}
std::vector<std::string>& Watchable::getTags() {
    return this->tags;
}
long Watchable::getId() const {
    return this->id;
}
Watchable* Watchable::getNextWatchable(Session &) const {return nullptr;}
string Watchable::getName() {return nullptr;}
bool Watchable::isTagExist(string tag) {
    for (int i = 0;(unsigned) i < this->tags.size(); ++i) {
        if(this->tags[i].compare(tag)==0)
            return true;
    }
    return false;
}


//****************MOVIE*******************************
Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags):Watchable(id,length,tags) ,name(name) {}
Movie::~Movie(){}

Watchable* Movie::getNextWatchable(Session & s) const {
return s.getActiveUser().getRealRec(s);
}
Watchable* Movie::clone() {
    return new Movie((*this));
}
string Movie::getName() {
    return this->name;
}
std::string Movie::toString() const {
    string s =  this->name ;
    return s;
}


//***************EPISODE********************************
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags):Watchable(id,length,tags),seriesName(seriesName),season(season),episode(episode),nextEpisodeId(id++) {}
Episode::~Episode() {}
Watchable* Episode::clone() {
    return new Episode(*this);
}
Watchable* Episode::getNextWatchable(Session & s) const {
    Watchable* w = s.getActiveUser().get_history()[s.getActiveUser().get_history().size()-1];
    long id = w->getId();
    int size= s.getLengthContent();
    if(id < size-1 && (s.getContent()[id+1]->getName().compare(w->getName()) == 0))
        return s.getContent()[id+1];
    else
       return s.getActiveUser().getRealRec(s);

}
std::string Episode::toString() const {
    string s = this->seriesName + " S" + to_string(this->season) + "E" + to_string(this->episode) + " ";
    return s;
}
string Episode::getName() {
    return this->seriesName;
}








