#include <bits/stdc++.h>
using namespace std;

#define ln '\n'

int noprod;
vector<string> prod, leftside;
map<string, vector<string>> rules;
map<string, set<char>> first, follow;

bool derivesEps(char ch){
    // string temp = "";
    // temp += ch;
    string temp(1, ch);
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
                if (!rhs.empty()) rules[lhs].push_back(rhs);
                rhs = "";
            }
            else rhs = rhs + s[i];
        }
    }
    if (!rhs.empty()) rules[lhs].push_back(rhs);
    leftside.push_back(lhs);
    return ;
}

void computeFirst(){
    for (int i=0; i<noprod; i++){
        for (auto j : rules){
            for (auto k : j.second){
                bool br = false;
                for (auto l : k){
                    if ((l<='9' && l>='0')||(l<='z' && l>='a')||l=='#'){
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
                            if ((k[m] <= 'z' && k[m] >= 'a')){
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

int main(){
    string productions;
    cout<<"Enter the number of productions: ";
    cin>>noprod;
    cout<<"Enter the productions. Use '#' for epsilon."<<ln;
    getchar();
    for (int i=0; i<noprod; i++){
        getline(cin, productions);
        separateProd(productions);
        // getchar();
    }
    // for (auto i : rules){
    //     cout<<i.first<<" -> ";
    //     for (auto j : i.second){
    //         cout<<j<<" | ";
    //     }
    //     cout<<ln;
    // }

    computeFirst();
    computeFollow();
    display();
}
