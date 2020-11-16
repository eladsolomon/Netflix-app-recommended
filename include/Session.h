#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "Watchable.h"
using  namespace std;

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    Session(Session &&);
    Session(const Session &);
    Session&operator=(Session&&);
    Session& operator=(const Session&);
    std::unordered_map<std::string,User*>& getMap();
    std::vector<std::string>& getTmp();
    std::vector<BaseAction*>& getLog();
    void addActionToVector(BaseAction*);
    void setActiveUser(User&);
    void deleteFromMap(std::string&);
    void printContentList();
    void printUserWatchHistory();
    void printActionLog();
    std::vector<Watchable*>& getContent();
    User& getActiveUser() ;
    int getLengthContent();
    void setTmpNextEpisode(int);
    Watchable* getPointerContent(int );

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::vector<std::string> tmp;
};
#endif
