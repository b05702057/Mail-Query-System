#include<iostream>
#include<unordered_set>
#include<set>
#include<string>
#include<map>
#include<vector>
#include <algorithm> //intersection
using namespace std;


class email{
public:
    int id;
    string from;
    string to;
    string date;
    set<string> content_set;
    //unordered_set<string> content_uset;

    email(const int& i, const string& f, const string& t, const string& d, string& content):id(i),from(f),to(t),date(d){
        content_init(&content[0]);
    }
    void content_init(char* content);
    bool find(string& keyword);

};
void email::content_init(char* content){
    char* p = content;
    //cout<<p;
    while(*p != '\0'){
        char* n = p;
        while(isalnum(*n)){
            ++n;
        }
        string s(p, n);
        content_set.emplace(s);
        //content_uset.emplace(s);

        while(!(isalnum(*n)) && *n != '\0'){
            ++n;
        }
        p = n;
    }
}
bool email::find(string& keyword){
    return !(content_set.find(keyword) == content_set.end());
}

map<string, vector<email> > mail_from;
map<string, vector<email> > mail_to;
map<string, vector<email> > mail_date;

void add(const int & id, const string & from, const string & to, const string & date, string & content){

    email mail(id,from,to,date,content);

    map<string, vector<email> >::iterator it = mail_from.find(from);
    if(it == mail_from.end()){
        vector<email> e;
        it = mail_from.insert(mail_from.begin(), pair<string, vector<email> >(from,e));
    }
    it->second.push_back( mail );

    it = mail_to.find(to);
    if(it == mail_to.end()){
        vector<email> e;
        it = mail_to.insert(mail_to.begin(), pair<string, vector<email> >(to,e));
    }
    it->second.push_back( mail );

    it = mail_date.find(date);
    if(it == mail_date.end()){
        vector<email> e;
        it = mail_date.insert(mail_date.begin(), pair<string, vector<email> >(date,e));
    }
    it->second.push_back( mail );

}

set<int> find_by_from(string from, string keyword){
    //把找到所有from 特定sender，且包含 keyword 的 mail，塞進一個set中。
    set<int> result;
    map<string, vector<email> >::iterator it = mail_from.find(from); //找mail_from有沒有這位sender
    if(it != mail_from.end()){
        for(int i = 0; i<it->second.size(); ++i){ //有的話，把vector中所有mail的content抓出來檢查是否有keyword
            if( it->second[i].find(keyword) ){
                //cout<< it->second[i].id <<endl;
                result.insert(it->second[i].id);
            }
        }
    }
    return result;
}

set<int> find_by_to(string to, string keyword){
    //把找到所有to 特定sender，且包含 keyword 的 mail，塞進一個set中。
    set<int> result;
    map<string, vector<email> >::iterator it = mail_to.find(to); //找mail_to有沒有這位receiver
    if(it != mail_to.end()){
        for(int i = 0; i<it->second.size(); ++i){ //有的話，把vector中所有mail的content抓出來檢查是否有keyword
            if( it->second[i].find(keyword) ){
                //cout<< it->second[i].id <<endl;
                result.insert(it->second[i].id);
            }
        }
    }
    return result;
}

void query_ft(const string& from, const string& to, const string& keyword){
    set<int> f = find_by_from(from, keyword);
    set<int> t = find_by_to(to, keyword);

    set<int> intersect;
    set_intersection(f.begin(), f.end(), t.begin(), t.end(), inserter(intersect, intersect.begin()) );
    for(set<int>::iterator it = intersect.begin(); it!=intersect.end(); it++ ){
        cout<< *it <<" ";
    }
    cout<<endl;
}

void query_f(const string& from, const string& keyword){
    set<int> f = find_by_from(from, keyword);

   for(set<int>::iterator it = f.begin(); it!=f.end(); it++ ){
        cout<< *it <<" ";
    }
    cout<<endl;
}

void query_t(const string& to, const string& keyword){
    set<int> t = find_by_to(to, keyword);

    for(set<int>::iterator it = t.begin(); it != t.end(); it++ ){
        cout<< *it << " ";
    }
    cout<< endl;
}

int main(){
    string s = "hello test content valid! >< hey";
    //ps: 要避免內文傳 const string，不然之後要存set很麻煩
    add(1,"Erin","anna","202006121240",s) ;

    s = "hi test today! >< #ok";
    add(2,"David","May","202006121248",s) ;

    s = "epic content like! >< hello reveal";
    add(3,"Mike","Tom","202006120900",s) ;

    s = "epic content like! >< hello reveal";
    add(4,"Erin","Tom","202006120900",s) ;

    s = "epic today like! >< ok reveal";
    add(5,"Erin","Tommy","202006120900",s) ;

    s = "epic today like! hello >< ok reveal";
    add(6,"Erin","Tommy","202006120900",s) ;

    cout<<"debug"<<endl;

    query_ft("Erin","Tommy","hello");

    cout<<"debug"<<endl;

    query_ft("Erin","Tom","hello");

    cout<<"debug"<<endl;

    query_ft("Erin","Tommy","like");

    cout<<"debug"<<endl;

}


