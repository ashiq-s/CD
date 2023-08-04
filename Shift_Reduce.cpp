#include <bits/stdc++.h>
using namespace std;

#define ln '\n'

int noprod, maxlen, top = 0;
vector<string> prod, leftside;
map<string, vector<string>> rules;
vector<char> stk;

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
                if (rhs.size() > maxlen) maxlen = rhs.size();
                rhs = "";
            }
            else rhs = rhs + s[i];
        }
    }
    if (!rhs.empty()) rules[lhs].push_back(rhs);
    if (rhs.size() > maxlen) maxlen = rhs.size();
    leftside.push_back(lhs);
    return ;
}

void shift(char c){
    stk.push_back(c);
    return ;
}

bool checkSuffix(){
    string temp = "";
    top = stk.size();
    for (int i = top-1; (i>=top - maxlen && i>=0); i--){
        temp = stk[i] + temp;
        for (auto k : rules){
            for (auto j : k.second){
                if (j == temp){
                    for (auto n : j){
                        stk.pop_back();
                    }
                    for (auto n : k.first){
                        stk.push_back(n);
                    }
                    return true;
                }
            }
            
        }
    } 
    return false;  
}

void parser(string ip){
    bool correct = true;
    ip = ip + '$';
    stk.push_back('$');
    cout<<ln<<"STACK\t\t\tINPUT\t\t\tACTION"<<ln;
    for (int i = 0; i<ip.size(); i++){
        if (i < ip.size() - 1){
            shift(ip[i]);
            // cout<<1<<ln;
            for (auto j : stk) cout<<j;
            cout<<"\t\t";
            for (int j = i+1; j<ip.size(); j++){
                cout<<ip[j];
            }
            cout<<"\t\t"<<"Shift"<<ln;
        }
        while (checkSuffix()){
            for (auto j : stk) cout<<j;
            cout<<"\t\t";
            for (int j = i+1; j<ip.size(); j++){
                cout<<ip[j];
            }
            cout<<"\t\t"<<"Reduce"<<ln;
        }
    }
    if (stk.size() < 3) {
        cout<<"String parsed successfully."<<ln;
    }
    else cout<<"String failed to be parsed."<<ln;
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

    string test;
    cout<<"Enter the string to parse: ";
    cin>>test;
    parser(test);
}
