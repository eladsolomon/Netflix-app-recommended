#include <iostream>
#include <cstring>
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"

using namespace std;

//***********BaseAction*********************
BaseAction::BaseAction() : errorMsg(""), status(PENDING) {
   // this->setStatus(PENDING);
}
BaseAction::~BaseAction() {}
BaseAction* BaseAction::clone() {
    return nullptr;
}
void BaseAction::setStatus(ActionStatus a) {
    this->status=a;
}
void BaseAction::complete() {
    setStatus(COMPLETED);
}
void BaseAction::setErrorMsg(std::string w) {
    this->errorMsg=w;
}
void BaseAction::error(const std::string &errorMsg) {
    setErrorMsg(errorMsg);
    setStatus(ERROR);
//cout << "ERROR";
}
ActionStatus BaseAction::getStatus() const {
    return this->status;
}
std::string BaseAction::getErrorMsg() const {
    return this->errorMsg;
}
/*std::string BaseAction::print(Session & s) {
    return s.printMe(*this);
}*/
std::string BaseAction::getmsg() const {
    return this->errorMsg;
}


//CreateUser
CreateUser::CreateUser() {}
void CreateUser::act(Session &sess) {

    vector<string>tmp = sess.getTmp();

    if(sess.getMap().find(sess.getTmp()[1])==sess.getMap().end())
    {
        if ((tmp.size()==3)) {
            if ((tmp[2].compare("len") == 0)) {
                LengthRecommenderUser *userlen = new LengthRecommenderUser(tmp[1]);
                sess.getMap().insert({tmp[1], userlen});
                this->complete();
                sess.addActionToVector(this);
            } else if ((tmp[2].compare("rer") == 0)) {
                RerunRecommenderUser *userrer = new RerunRecommenderUser(tmp[1]);
                sess.getMap().insert({tmp[1], userrer});
                this->complete();
                sess.addActionToVector(this);
            } else if ((tmp[2].compare("gen") == 0)) {
                GenreRecommenderUser *usergen = new GenreRecommenderUser(tmp[1]);
                sess.getMap().insert({tmp[1], usergen});
                this->complete();
                sess.addActionToVector(this);
            } else {
                this->error("ERROR: recommedation algorithem is invalid");
                cout<<"ERROR: recommedation algorithem is invalid";
                sess.addActionToVector(this);
            }
        } else {
            this->error("ERROR: invalid input");
            cout<<"ERROR: invalid input";
            sess.addActionToVector(this);
        }
    }
    else {
        this->error("User Name Exist");
        cout<<"ERROR: User Name Exist";
        sess.addActionToVector(this);
    }
}
std::string CreateUser::toString() const {
    string s = "CreateUser";
    return s;
}
BaseAction* CreateUser::clone() {
    return new CreateUser(*this);
}

//ChangeActiveUser
ChangeActiveUser::ChangeActiveUser() {}
void ChangeActiveUser::act(Session &sess) {
    vector<string>tmp = sess.getTmp();
    if(sess.getMap().find(sess.getTmp()[1])!=sess.getMap().end())
    {
        sess.setActiveUser(*sess.getMap().at(sess.getTmp()[1]));
        this->complete();
        sess.addActionToVector(this);
    } else{
        this->error("User Name Doesnt Exist");
        cout<<"ERROR: User Name Doesnt Exist";
        sess.addActionToVector(this);
    }
}
std::string ChangeActiveUser::toString() const {
    string s = "ChangeActiveUser";
    return s;
}
BaseAction* ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}

//DeleteUser
DeleteUser::DeleteUser() {}
void DeleteUser::act(Session &sess) {
    vector<string> tmp = sess.getTmp();
    if (sess.getMap().find(sess.getTmp()[1]) != sess.getMap().end()) {
        string s = sess.getTmp()[1];
        sess.deleteFromMap(s);
        this->complete();
        sess.addActionToVector(this);
    } else {
        this->error("User Name Doesnt Exist");
        cout<<"ERROR: User Name Doesnt Exist";
        sess.addActionToVector(this);
    }
}
std::string DeleteUser::toString() const {
    string s = "DeleteUser";
    return s;
}
BaseAction* DeleteUser::clone() {
    return new DeleteUser(*this);
}
//DuplicateUser
DuplicateUser::DuplicateUser(){}
std::string DuplicateUser::toString() const {
    string s = "DuplicateUser";
    return s;
}
void DuplicateUser::act(Session &sess) {
    vector<string>tmp = sess.getTmp();
    if(sess.getMap().find(sess.getTmp()[1])!=sess.getMap().end())//if the original exise
    {
       if (sess.getMap().find(sess.getTmp()[2])==sess.getMap().end())// if the name of the dup user exist
       {
           sess.getMap().at(sess.getTmp()[1])->dupUser(sess.getTmp()[2],sess);
           this->complete();
           sess.addActionToVector(this);
       }
       else
          {
            this->error("The User name is allready exist");
            cout<<"ERROR: The User name is allready exist";
            sess.addActionToVector(this);
          }
       }
    else{
        this->error("The Original User Name Doesnt Exist");
        cout<<"The Original User Name Doesnt Exist";
        sess.addActionToVector(this);
        }
}
BaseAction* DuplicateUser::clone() {
    return new DuplicateUser(*this);
}

//PrintContentList
PrintContentList::PrintContentList() {}
void PrintContentList::act(Session &sess){

    sess.printContentList();
    this->complete();
    sess.addActionToVector(this);
}
std::string PrintContentList::toString() const {
    string s = "PrintContentList";
    return s;
}
BaseAction* PrintContentList::clone() {
    return new PrintContentList(*this);
}
//PrintWatchHistory
PrintWatchHistory::PrintWatchHistory() {}
std::string PrintWatchHistory::toString() const {
    string s = "PrintWatchHistory";
    return s;
}
void PrintWatchHistory::act(Session &sess) {
    sess.printUserWatchHistory();
    this->complete();
    sess.addActionToVector(this);
}
BaseAction* PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}
//Watch
Watch::Watch() {}
std::string Watch::toString() const {
    string s = "Watch";
    return s;
}
void Watch::act(Session &sess) {
    try {

        vector<string> tmp = sess.getTmp();
        const string str = tmp[1];
        if ((unsigned) stoi(str) <= sess.getContent().size()) {
            printf("watching : ");
            int t = stoi(str) - 1;
            cout << sess.getContent()[t]->toString();
            sess.getActiveUser().insertToWatch(sess.getContent()[t]);
            this->complete();
            sess.addActionToVector(this);
            cout << "\n";
            printf(" We recommend watching ");
            Watchable *w = sess.getActiveUser().getRecommendation(sess);
            if (w != nullptr) {

                cout << w->toString();
                printf(", continue watching? [y/n]");
                char ans;
                scanf("%c", &ans);
                while (getchar() != '\n');

                if (ans == 'y') {
                    int nextEpsiode = w->getId() + 1;
                    sess.setTmpNextEpisode(nextEpsiode);
                    Watch *watch = new Watch;
                    watch->act(sess);
                }
            } else {
                this->error("Couldnt find recommedation");
                cout << "ERROR: Couldnt find recommedation";
                sess.addActionToVector(this);
            }
        } else {
            this->error("couldnt find the episode/movie");
            cout << "ERROR: couldnt find the episode/movie";
            sess.addActionToVector(this);
        }
    }catch (exception e)
    {
        this->error("input unvalid");
        cout << "input unvalid";
        sess.addActionToVector(this);
    }
}
BaseAction* Watch::clone() {
    return new Watch(*this);
}
//PrintActionLog
PrintActionsLog::PrintActionsLog() {}
void PrintActionsLog::act(Session &sess) {
    sess.printActionLog();
    this->complete();
    sess.addActionToVector(this);
}
std::string PrintActionsLog::toString() const {
    string s = "PrintActionsLog";
    return s;
}
BaseAction* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}
//Exit
Exit::Exit() {}
std::string Exit::toString() const {
    string s = "Exit";
    return s;
}
void Exit::act(Session &sess) {
    this->complete();
    sess.addActionToVector(this);
}
BaseAction* Exit::clone() {
    return new Exit(*this);
}