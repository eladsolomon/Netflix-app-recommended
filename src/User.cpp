#include <iostream>
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "algorithm"
#include "unordered_map"
#include <cmath>
#include "algorithm"

using namespace std;
//*************USER*************************
User::User(const std::string &name): history(), name(name) {}//Constructor
User::User(const User &u) : history(), name(u.name) {//copyConstructor
    for (int i = 0; (unsigned)i <u.history.size() ; ++i) {
        this->history.push_back(u.history[i]);
    }
}
User::~User() {
    history.clear();
}
User::User(User &&other) :history(std::move(other.history)), name(other.name){}
User& User::operator=(const User& u) {
    if(this != &u) {
        /*for (int i = 0; i < this->history.size(); ++i) {
            delete this->history[i];
        }*/
        this->history.clear();
        for (int j = 0; (unsigned)j < u.history.size(); ++j) {
            this->history.push_back(u.history[j]->clone());
        }
    }
    return *this;
}
User& User::operator=(User &&other ) {
    if(this != &other) {
//        for (int i = 0; i < this->history.size(); ++i) {
//            delete this->history[i];
//        }
        this->history.clear();
        for (int j = 0; (unsigned)j < other.history.size(); ++j) {
            this->history.push_back(other.history[j]->clone());
        }
        for (int k = 0; (unsigned)k < other.history.size(); ++k) {
            delete other.history[k];
        }
        other.history.clear();
    }
    return *this;
}
Watchable* User::getRecommendation(Session &s) {
return nullptr;
}
Watchable* User::getRealRec(Session &) {return nullptr;}
std::string User::getName() const {return this->name;}
std::vector<Watchable*> User::get_history() const {
    return  this->history;
}
void User::printWatchHistory() {
    cout<< "Watch history for: "<< this->name << endl;
    int j = this->history.size();
    for (int i = 0; i < j; ++i) {
        string s = to_string(i+1);
        cout << s+ ". ";
        cout << this->history[i]->toString()<<endl;
    }
}
void User::insertToWatch(Watchable* w) {

    this->history.push_back(w);
}
void User::dupHistory(vector<Watchable *> &r) {
    for (int i = 0; (unsigned)i <r.size() ; ++i) {
        this->history.push_back(r[i]);
    }
}
void User::dupUser(string, Session &) {}
User* User::clone() {return nullptr;}
void User::setName(string s) {
    this->name=s;
}
string User::getActiveNAme() {
    return this->name;
}
void User::clearHist() {

    history.clear();
}
//***********LengthRecommenderUser*********************
LengthRecommenderUser::LengthRecommenderUser(const std::string &name):User(name) {}
LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &u) : User(u){}
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    Watchable *W =history[0]->getNextWatchable(s);
    return W;
    }
Watchable* LengthRecommenderUser::getRealRec(Session & sess) {
    Watchable *W = nullptr;
    int size = this->history.size();
    int length = 0;
    for (int i = 0; i < size; ++i) {
        length = length + this->history[i]->getLength();
    }
    int ans = length / size;
    int hefresh = 1000000;

    int j = sess.getLengthContent();
    for (int i = 0; i < j; i++) {
        if (hefresh > (abs(ans - sess.getContent()[i]->getLength()))) {
            if (find(history.begin(), history.end(), sess.getContent()[i]) == history.end()) {
                hefresh =abs( ans - sess.getContent()[i]->getLength());
                W = sess.getContent()[i];
            }
        }
    }
    return W;
}
void LengthRecommenderUser::dupUser(string s, Session &sess) {
    LengthRecommenderUser* u = new LengthRecommenderUser(s);
    sess.getMap().insert({s,u});
    u->dupHistory(this->history);
}
LengthRecommenderUser* LengthRecommenderUser::clone() {
    return new LengthRecommenderUser(*this);
}

//***********Rerun*********************
RerunRecommenderUser& RerunRecommenderUser::operator=(const RerunRecommenderUser & u){
    if(this != &u) {
        this->history.clear();
        for (int j = 0; (unsigned)j < u.history.size(); ++j) {
            this->history.push_back(u.history[j]);
        }
        this->runIndex = u.runIndex;
        return *this;
    }
    else return *this;
}
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) :User(name), runIndex(0) {}
RerunRecommenderUser::RerunRecommenderUser(RerunRecommenderUser &&other) :User(other),runIndex(other.runIndex){}
RerunRecommenderUser& RerunRecommenderUser::operator=(RerunRecommenderUser &&other) {

        if(this != &other) {
            this->history.clear();
            for (int j = 0; (unsigned)j < other.history.size(); ++j) {
                this->history.push_back(other.history[j]);
            }
            this->runIndex=other.runIndex;
            other.history.clear();
        }
        return *this;
    }
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    Watchable *W =history[history.size()-1]->getNextWatchable(s);
    return W;
}
Watchable* RerunRecommenderUser::getRealRec(Session & s) {
Watchable* w = this->history[runIndex%this->history.size()];
runIndex++;
return w;
}
int RerunRecommenderUser::getRunIndex() {
    return this->runIndex;
}
void RerunRecommenderUser::dupUser(string s, Session &sess) {
    RerunRecommenderUser* u = new RerunRecommenderUser(s);
    sess.getMap().insert({s,u});
    u->dupHistory(this->history);
}
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser &e):User(e),runIndex(e.runIndex) {

}
RerunRecommenderUser* RerunRecommenderUser::clone() {
    return new RerunRecommenderUser(*this);
}


//***********GenereRecommenderUser*********************
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) :User(name), tagsVector() {}
GenreRecommenderUser::~GenreRecommenderUser() {
    tagsVector.clear();
}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    Watchable *W =history[history.size()-1]->getNextWatchable(s);
    return W;
}
Watchable* GenreRecommenderUser::getRealRec(Session & s) {
    Watchable* W= nullptr;
  sort(tagsVector.begin(),tagsVector.end(),sorty);
  bool found= false;
    for (int i = 0; ((unsigned)i <tagsVector.size())&(!found) ; ++i) {
        for (int j = 0; ((unsigned)j < (unsigned)s.getLengthContent())&(!found); ++j) {
            if(s.getContent()[j]->isTagExist(tagsVector[i].first)) // if this Watchable have this tag.
            {
                if (find(history.begin(), history.end(), s.getContent()[j]) == history.end()) //and we didnt watched him already.
                {
                     W= s.getContent()[j];
                    found = true;
                }
            }
        }
    }
    return W;
}
GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser &u) :User(u),tagsVector(u.tagsVector) {}
bool GenreRecommenderUser::sorty(const pair<string, int> &A, const pair<string, int> &B) {
    if(A.second!=B.second) return A.second>B.second;
    return A.first<B.first;
}
void GenreRecommenderUser::dupUser(string s, Session &sess) {
    GenreRecommenderUser* u = new GenreRecommenderUser(s);
    sess.getMap().insert({s,u});
    u->dupHistory(this->history);
}
GenreRecommenderUser* GenreRecommenderUser::clone() {
    return new GenreRecommenderUser(*this);
}
void GenreRecommenderUser::insertToWatch(Watchable * w) {

    this->history.push_back(w);
   vector<string> s = w->getTags();
    for (int i = 0; (unsigned)i <s.size() ; ++i) {
        bool found=false;
        for (int k = 0;((unsigned) k <(unsigned)tagsVector.size())&(!found) ; ++k) {
            if(tagsVector[k].first.compare(s[i])==0)
            {
                found=true;
                tagsVector[k].second++;
            }
        }
        if(!found)
        {
            pair<string,int> p;
            p.first=s[i];
            p.second=1;
            tagsVector.push_back(p);
        }


    }
}
