#include <iostream>
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/json.hpp"
#include "../include/Watchable.h"
#include <fstream>
#include <vector>
#include "unordered_map"
#include <string>
#include "sstream"

using namespace std;
using namespace nlohmann;



Session::Session(const std::string &configFilePath) :  content(), actionsLog(), userMap(),activeUser(), tmp(){
    this->activeUser= new LengthRecommenderUser("default");
    string c= "default";
    this->userMap.insert({c,this->activeUser});

    //Read the json file

    
    ifstream myFile(configFilePath, ifstream::binary);
    json j;
    myFile >> j;

    //***************MOVIES************************************************
    int i = 0;
    while (j["movies"][i] != nullptr) {
        int index = 0;
        vector<string> temp;
        while (j["movies"][i]["tags"][index] != nullptr) {
            temp.push_back(j["movies"][i]["tags"][index]);
            index++;
        }
        this->content.push_back(new Movie(i, j["movies"][i]["name"], j["movies"][i]["length"], temp));
        i++;
    }

    //******************TV-SERIES******************************************
    int k = 0;
    int ID=i;
    while (j["tv_series"][k] != nullptr) {//go throw all the series that available
        int index = 0;
        vector<string> tags;
        while (j["tv_series"][k]["tags"][index] != nullptr) {//get the tags vector
            tags.push_back(j["tv_series"][k]["tags"][index]);
            index++;
        }
        int sizeOfSeason = 0;
        while (j["tv_series"][k]["seasons"][sizeOfSeason] != nullptr)//get the number of the seasons
        {
            sizeOfSeason++;
        }
        for (int e= 0; e<=sizeOfSeason; e++)
        {
            int numberOfEpisode=1;
            for(int r=0; r<  j["tv_series"][k]["seasons"][e];r++)
            {
                this->content.push_back(new Episode(ID,j["tv_series"][k]["name"], j["tv_series"][k]["episode_length"],e+1,numberOfEpisode, tags));
                 numberOfEpisode++;
                ID++;
            }
        }
        k++;
    }

}

Session::~Session(){
    //Delete all the Users
    for (auto it=userMap.begin(); it!=userMap.end(); it++)
        delete it->second;
userMap.clear();


    for(int i=0 ;(unsigned) i < content.size();i++) {
        delete content[i];
    }
    content.clear();
    for(int i=0 ; (unsigned)i < actionsLog.size();i++) {
      delete actionsLog[i]; }
actionsLog.clear();

}
Session::Session(const Session &sess) : content(), actionsLog(), userMap(),activeUser(), tmp(){
    for (int i = 0;(unsigned) i <sess.content.size() ; ++i) {
        this->content.push_back(sess.content[i]->clone());
    }
    for (int j = 0; (unsigned)j <sess.actionsLog.size() ; ++j) {
        this->actionsLog.push_back((sess.actionsLog[j]->clone()));
    }
    for(auto it=sess.userMap.begin();it!=sess.userMap.end();it++){
        auto *user = it->second->clone();
        user->clearHist();

        for (int i = 0; (unsigned)i <it->second->get_history().size() ; ++i) {
            int tryy=it->second->get_history()[i]->getId();
            user->insertToWatch(this->content[tryy]);
        }
        this->userMap.insert({user->getName(),user});
    }
    string activeName = sess.activeUser->getActiveNAme();
    this->activeUser=this->userMap.at(activeName);

}//copy constructor
Session& Session::operator=(const Session& other) {
    if(this != &other){

        for(auto it=userMap.begin();it!=userMap.end();it++) {
            it->second->clearHist();
            delete it->second;

        }
        userMap.clear();
        for (int i = 0; (unsigned)i < this->actionsLog.size(); ++i) {
            delete this->actionsLog[i];
        }
        actionsLog.clear();
        for (int i = 0; (unsigned)i < this->content.size(); ++i) {
            delete this->content[i];
        }
        content.clear();
        //////////////////////////////////////////////////

        for (int j = 0;(unsigned) j < other.content.size(); ++j) {
            this->content.push_back(other.content[j]->clone());
        }

        for (int j = 0; (unsigned)j < other.actionsLog.size(); ++j) {
            this->actionsLog.push_back(other.actionsLog[j]->clone());
        }
        for(auto it=other.userMap.begin();it!=other.userMap.end();it++){
            auto *user = it->second->clone();
            user->clearHist();

            for (int i = 0; (unsigned)i <it->second->get_history().size() ; ++i) {
                int index=it->second->get_history()[i]->getId();
                user->insertToWatch(this->content[index]);
            }
            this->userMap.insert({user->getName(),user});
        }
        string activeName = other.activeUser->getActiveNAme();
        this->activeUser=this->userMap.at(activeName);


    }

    return *this;
}//copy assignment operator
Session::Session(Session &&sess) :  content(), actionsLog(), userMap(),activeUser(), tmp(){
    for (int i = 0; (unsigned)i <sess.content.size() ; ++i) {
        this->content.push_back(sess.content[i]);

    }
    sess.content.clear();
    for (int j = 0; (unsigned)j <sess.actionsLog.size() ; ++j) {
        this->actionsLog.push_back((sess.actionsLog[j]));

    }
    sess.actionsLog.clear();
    for(std::pair<string,User*> user:sess.userMap){
        this->userMap.insert(make_pair(user.first,user.second));

    }
    this->activeUser=sess.activeUser;
    sess.activeUser=nullptr ;
    sess.userMap.clear();



}//move constructor
Session& Session::operator=(Session &&other){//move assignment operator
    if(this != &other) {
        //delete from this
        for (int i = 0; (unsigned)i < this->content.size(); ++i) {
            delete this->content[i];
        }
        this->content.clear();
        for (int j = 0; (unsigned)j < this->actionsLog.size(); ++j) {
            delete this->actionsLog[j];
        }
        this->actionsLog.clear();
        for(auto it=userMap.begin();it!=userMap.end();it++) {
            it->second->clearHist();
            delete it->second;

        }
        this->userMap.clear();

        //copy to this
        this->activeUser = other.activeUser;
        for(auto it=other.userMap.begin();it!=other.userMap.end();it++) {
            auto *user = it->second;
            this->userMap.insert({user->getName(), user});
        }

        for (int k = 0;(unsigned) k < other.content.size(); ++k) {
            this->content.push_back((other.content[k]));
        }
        for (int l = 0; (unsigned)l < other.actionsLog.size(); ++l) {
            this->actionsLog.push_back(other.actionsLog[l]);
        }

        //delete from other
        other.content.clear();
        other.actionsLog.clear();
        other.userMap.clear();
        other.activeUser=nullptr;


    }

    return *this;
}//move assignment
 void Session::start() {
     printf("SPLFLIX is now on!");
     this->tmp.clear();
 this->tmp.push_back("");

     while ( tmp[0].compare("exit") != 0) {
         std::string input;
         getline(cin,input);
         std::istringstream iss(input);
         std::vector<std::string> ans((std::istream_iterator<std::string>(iss)),
                                      std::istream_iterator<std::string>());
         this->tmp=ans;
         if ((tmp[0].compare("createuser") == 0)) {
             BaseAction *createuser = new CreateUser();
             createuser->act(*this);
         } else if ((tmp[0].compare("changeuser") == 0) ){
             BaseAction *changeuser = new ChangeActiveUser();
             changeuser->act(*this);
         } else if ((tmp[0].compare("deleteuser") == 0)) {
             BaseAction *deleteuser = new DeleteUser();
             deleteuser->act(*this);
         } else if ((tmp[0].compare("dupuser") == 0)) {
             BaseAction *duplicateuser = new DuplicateUser();
             duplicateuser->act(*this);
         } else if ((tmp[0].compare("content"))==0){
             BaseAction *printcontentlist = new PrintContentList();
             printcontentlist->act(*this);
         } else if ((tmp[0].compare("watchhist") == 0)) {
             BaseAction *printhistory = new PrintWatchHistory();
             printhistory->act(*this);
         } else if ((tmp[0].compare("watch") == 0)){
             BaseAction *watch = new Watch();
             watch->act(*this);
         } else if ((tmp[0].compare("log") == 0)) {
             BaseAction *printactionlog = new PrintActionsLog();
             printactionlog->act(*this);
         } else if ((tmp[0].compare("exit") == 0)) {
             BaseAction *exit = new Exit();
             exit->act(*this);
         } else {
             printf("input not valid ! please write again");
         }
     }
 }
     std::unordered_map<std::string, User *> &Session::getMap() {
         return this->userMap;
     }
     std::vector<std::string>& Session::getTmp() {
         return this->tmp;
     }
     std::vector<BaseAction *> &Session::getLog() {
         return this->actionsLog;
     }

     void Session::addActionToVector(BaseAction *b) {
         this->actionsLog.push_back(b);
     }
     void Session::setActiveUser(User &u) {
         this->activeUser = &u;
     }
     void Session::deleteFromMap(std::string &s) {
         auto f = userMap.find(s);
         if (f != userMap.end()){
             delete (f->second);
         userMap.erase(f);
     }

     }
     void Session::printContentList() {
         int j = this->content.size();
         for (int i = 0; i < j; i++) {
             string s = to_string(content[i]->getId()+1);
             std::cout <<s + ". ";
             cout << this->content[i]->toString();
             cout << " " << content[i]->getLength() << " minutes" << " [";
             int m =content[i]->getTags().size();
             for (int n = 0; n < m-1; n++) {
                 std::cout << content[i]->getTags()[n] << ",";
             }
             std::cout << content[i]->getTags()[m-1];
             std::cout << "]" << endl;
         }
     }
     /*std::string Session::printMe(Watchable & w) {
         return w.toString();
     }*/
     void Session::printUserWatchHistory() {
        this->activeUser->printWatchHistory();
    }
    void Session::printActionLog() {
        int j = this->actionsLog.size();
        for (int i = 0; i < j; i++) {
             cout << this->actionsLog[i]->toString();
            if(actionsLog[i]->getStatus() == COMPLETED)
                cout <<  " COMPLETED" << endl;
            else if(actionsLog[i]->getStatus() == ERROR)
                cout << " ERROR: " +  actionsLog[i]->getmsg() << endl;
            else if(actionsLog[i]->getStatus() == PENDING)
                cout << " PENDING" << endl;

        }
    }
   /* std::string Session::printMe(BaseAction & b) {
        return b.toString();
    }*/
   std::vector<Watchable*>& Session::getContent() {
       return this->content;
   }
   User& Session::getActiveUser() {
       return *activeUser;
   }
   int Session::getLengthContent() {
       return this->content.size();
   }
   void Session::setTmpNextEpisode(int i) {
       this->tmp[1] = to_string(i);
   }
Watchable* Session::getPointerContent(int i) {
       bool found=false;
    for (int j = 0; ((unsigned)j <content.size())&(!found) ; ++j) {
        if(content[j]->getId()==i)
            return content[j];
    }
    return nullptr;
   }

