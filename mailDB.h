#include <iostream>
#include <unordered_set>
#include <set>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <stack>

using namespace std ;

typedef std::pair<int, unsigned> LENGTH; 

class Mail
{
public: 
    bool searchContent(std::string& keyword);

    // Enable MailDB access private data
    friend class MailDB;
private:
    unsigned id;
    std::string subject;
    std::string from;
    std::string to;
    unordered_map< string, unordered_set<string> > from_dict; // pair 中裝著 id 及 date
    unordered_map< string, unordered_set<string> > to_dict; 
    unordered_map< string, unordered_set<string> > word_dict;

    unordered_map< string, vector<string> > database;

    set < vector<int> > longest_set;

    // TO-Do:
    // A container for content
};

class MailDB
{
public:
    MailDB() {};
    ~MailDB() {};

    // Thers four member funtions is the interface of this class
    // which is designed for the four commands respectively
    string add(std::string& path);
    void remove(unsigned id);
    void longest();
    void query(std::vector<std::string>& args) 
    {
        if (args.size()>1) 
            queryWithCond(args);
        else if (args.size()==1)
            queryOnlyExpr(args[0]);
    }
private:
    // We may need more member data or function hear

    // These two private function is for two query situation
    // 1. query only with expression
    // 2. query with other conditions
    // You can costumize your situation
    // and don't forget to change the interface too!
    void queryOnlyExpr(std::string& expr);
    void queryWithCond(std::vector<std::string>& args);

    // This heap is for the longest() function
    std::priority_queue<LENGTH, std::vector<LENGTH>, std::greater<int> > lengthHeap;
    
    // TO-Do:
    // A container store mails
    // and maybe other containers to store content
};