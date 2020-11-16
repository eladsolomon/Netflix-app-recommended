#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
#include "User.h"
using namespace std;
class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    std::string print(Session&);
    std::string print(User&);
    int getLength() const;
    std::vector<std::string>& getTags() ;
    long getId() const;
    virtual string getName();
    bool isTagExist(string);
    virtual Watchable* clone();
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual ~Movie();
    virtual std::string toString()const;
    virtual Watchable* getNextWatchable(Session&) const;
    string getName();
    virtual Watchable* clone();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString()const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual ~Episode();
    string getName();
    virtual Watchable* clone();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
