#include <bits/stdc++.h>
using namespace std;

#define ln '\n'

int noprod;
vector<string> prod, leftside;
map<string, vector<string>> rules;
map<string, set<char>> first, follow;
vector<pair<string, string>> p;
map<string, set<pair<char, string>>> table;
vector<char> stk;

bool derivesEps(char ch){
    string temp = "";
    temp += ch;
    for (auto i : first[temp]){
        if (i == '#') return true;
    }
    return false;
}

void separateProd(string s){
    string lhs="", rhs="";
    bool l = true;
    for (int i=0; i<s.size(); i++){
        // cout<<rhs<<ln;
        if (l){
            if (i< s.size() - 1 && s[i]=='-' && s[i+1] == '>'){
                l = false;
                i++;
            }
            else if (s[i] == ' ');
            else if (s[i] == '=') l = false;
            else lhs = lhs + s[i];
        }
        else {
            if (s[i] == ' ' || s[i] == '|'){
                if (!rhs.empty()) {
                    rules[lhs].push_back(rhs);
                    p.push_back({lhs, rhs});
                }
                rhs = "";
            }
            else rhs = rhs + s[i];
        }
    }
    if (!rhs.empty()) {
        rules[lhs].push_back(rhs);
        p.push_back({lhs, rhs});
    }
    leftside.push_back(lhs);
    return ;
}

void computeFirst(){
    for (int i=0; i<noprod; i++){
        for (auto j : rules){
            for (auto k : j.second){
                bool br = false;
                for (auto l : k){
                    if ((l<='9' && l>='0')||(l<='z' && l>='a')||l=='#'||l==')'||l=='('||l=='+'||l=='*'){
                        first[j.first].insert(l);
                        br = true;
                        break;
                    }
                    else if (derivesEps(l)){
                        string temp = "";
                        temp += l;
                        for (auto m : first[temp]){
                            if (m != '#') first[j.first].insert(m);
                        }
                    }
                    else {
                        string temp = "";
                        temp += l;
                        for (auto m : first[temp]){
                            first[j.first].insert(m);
                        }
                        br = true;
                        break;
                    }
                }
                if (!br) first[j.first].insert('#');
            }
        }
    }
    // for (auto i : rules){
    //     cout<<i.first<<":\t";
    //     cout<<"{";
    //     for (auto j : first[i.first]) cout<<j<<",";
    //     cout<<"}";
    //     cout<<ln;
    // }
}

void computeFollow(){
    follow[leftside[0]].insert('$');
    for (int i=0; i<noprod; i++){
        for (auto j : rules){
            for (auto k : j.second){
                for (int l = 0; l<k.size(); l++){
                    if ((k[l] <= 'Z' && k[l] >= 'A')){
                        string temp = "";
                        temp += k[l];
                        bool br = false;
                        for (int m = l+1; m<k.size(); m++){
                            if ((k[m] <= 'z' && k[m] >= 'a')||k[m]==')'||k[m]=='('||k[m]=='*'||k[m]=='+'){
                                follow[temp].insert(k[m]);
                                br = true;
                                break;
                            }
                            else if ((k[m] <= 'Z' && k[m] >= 'A')){
                                string temp2 = "";
                                temp2 += k[m];
                                if (derivesEps(k[m])){
                                    for (auto n : first[temp2]){
                                        if (n!='#') follow[temp].insert(n);
                                    }
                                }
                                else {
                                    for (auto n : first[temp2]){
                                        follow[temp].insert(n);
                                    }
                                    br = true;
                                    break;
                                }                  
                            }
                        }
                        if (!br) {
                            for (auto n : follow[j.first]){
                                follow[temp].insert(n);
                            }
                        }
                    }
                }
            }
        }
    }
    // for (auto i : rules){
    //     cout<<i.first<<":\t";
    //     cout<<"{";
    //     for (auto j : follow[i.first]) cout<<j<<",";
    //     cout<<"}";
    //     cout<<ln;
    // }
}

void display(){
    cout<<"LHS\t\tFIRST\t\tFOLLOW"<<ln;
    for (auto i : rules){
        cout<<i.first<<":\t\t";
        cout<<"{";
        for (auto j : first[i.first]) cout<<j<<",";
        cout<<"}\t\t";
        cout<<"{";
        for (auto j : follow[i.first]) cout<<j<<",";
        cout<<"}"<<ln;
    }
}

void createTable(){
    for (auto i : p){
        for (auto j : i.second){
            if (j == '#'){
                for (auto k : follow[i.first]){
                    table[i.first].insert({k, i.second});
                }
                break;
            }
            else if (j >= 'A' && j <= 'Z'){
                string temp = "";
                temp += j;
                for (auto k : first[temp]){
                    table[i.first].insert({k, i.second});
                }
                break;
            }
            else {
                table[i.first].insert({j, i.second});
                break;
            }
        }
    }

    //Display table: 
    for (auto i : table){
        cout<<i.first<<":\t\t";
        for (auto j : i.second){
            cout<<j.first<<"->"<<j.second<<"\t";
        }
        cout<<ln;
    }
}

bool parseCheck(char c){
    bool found = false;
    string temp = "";
    temp += stk[stk.size() -1];
    for (auto i : table){
        if (temp == i.first){
            for (auto j : i.second){
                if (j.first == c){
                    found = true;
                    stk.pop_back();
                    for (int k = j.second.size()-1; k>=0; k--){
                        if (j.second[k] != '#') stk.push_back(j.second[k]);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

void parser(string ip){
    ip = ip + '$';
    stk.push_back('$');
    for (auto i : leftside[0]) stk.push_back(i);
    for (int i = 0; i<ip.size(); i++){
        for (auto j : stk){
            cout<<j;
        }
        cout<<"\t\t";
        for (int j = i; j<ip.size(); j++){
            cout<<ip[j];
        }
        cout<<ln;
        if (stk[stk.size()-1] == ip[i]){
            stk.pop_back();
            continue;
        }
        while (parseCheck(ip[i])){
            for (auto j : stk){
                cout<<j;
            }
            cout<<"\t\t";
            for (int j = i; j<ip.size(); j++){
                cout<<ip[j];
            }
            cout<<ln;
        }
        if (stk[stk.size()-1] == ip[i]){
            stk.pop_back();
            continue;
        }
        else {
            cout<<"String failed to be parsed."<<ln;
            return ;
        }
    }
    if (stk.size() < 2) {
        cout<<"String parsed successfully."<<ln;
    }
    else {
        cout<<"String failed to be parsed."<<ln;
    }
}

int main(){
    string productions, ip;
    cout<<"Enter the number of productions: ";
    cin>>noprod;
    cout<<"Enter the productions. Use '#' for epsilon."<<ln;
    getchar();
    for (int i=0; i<noprod; i++){
        getline(cin, productions);
        separateProd(productions);
        // getchar();
    }
    for (auto i : rules){
        cout<<i.first<<" -> ";
        for (auto j : i.second){
            cout<<j<<" | ";
        }
        cout<<ln;
    }

    computeFirst();
    computeFollow();
    display();
    cout<<"_________________________"<<ln;
    createTable();
    cout<<"Enter the string to parse: ";
    cin>>ip;
    parser(ip);
}
