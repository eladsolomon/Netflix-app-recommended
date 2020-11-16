#ifndef USER_H_
#define USER_H_
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

class Watchable;
class Session;
using namespace std;
class User{
public:
    User ();
    User(const std::string& name);
    User (const User&);
    virtual ~User();
    User (User &&);
    User &operator=(User &&);
    virtual User& operator=(const User&);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    void printWatchHistory();
    std::string printMe(Watchable&);
    virtual void insertToWatch(Watchable*);
    virtual Watchable* getRealRec(Session&);
    virtual void dupUser(string,Session& );
    void dupHistory(vector<Watchable*>&);
    virtual User* clone();
    virtual void setName(string s);
    string getActiveNAme();
    void clearHist();

protected:
    std::vector<Watchable*> history;
private:
    std::string name;


};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    LengthRecommenderUser(LengthRecommenderUser &&);
    LengthRecommenderUser (const LengthRecommenderUser&);
    Watchable* getRealRec(Session&);
    virtual void dupUser(string,Session& );
    virtual LengthRecommenderUser* clone();
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual RerunRecommenderUser& operator=(const RerunRecommenderUser&);
    RerunRecommenderUser(RerunRecommenderUser &&);
    RerunRecommenderUser (const RerunRecommenderUser&);
    RerunRecommenderUser &operator=(RerunRecommenderUser&&);
    Watchable* getRealRec(Session&);
    int getRunIndex();
    virtual void dupUser(string,Session& );
    virtual RerunRecommenderUser* clone();
private:
    int runIndex;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    GenreRecommenderUser(GenreRecommenderUser &&);
    ~GenreRecommenderUser();
    GenreRecommenderUser (const GenreRecommenderUser&);
    Watchable* getRealRec(Session&);
    virtual void insertToWatch(Watchable*);
    static bool sorty (const pair<string,int>&,const pair<string,int>&);
    virtual void dupUser(string,Session&);
    virtual GenreRecommenderUser* clone();
private:
    vector<pair<string,int>> tagsVector;
};

#endif
