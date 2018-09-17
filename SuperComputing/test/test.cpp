#include <set>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <boost/progress.hpp>

#define LENGTH 36

using namespace std;

set<string> repeated;
int chip_size[256], times;
string FILENAME = "output"+to_string(LENGTH)+".txt";

void dfs(string s){
    int l = 0;
    for (auto i: s)
        l += chip_size[i];
    if (l>LENGTH)
        return;
    else if (l<LENGTH){
        dfs(s + 'F');
        dfs(s + 'N');
        dfs(s + 'T');
        dfs(s + 'W');
        dfs(s + 'X');
        dfs(s + 'Y');
        dfs(s + '1');
        dfs(s + '2');
        dfs(s + '3');
        dfs(s + '4');
        dfs(s + '5');
        dfs(s + '6');
        dfs(s + '7');
        dfs(s + '8');
        dfs(s + '9');
    } else {
        sort(s.begin(), s.end());
        if (repeated.count(s) != 0)
            return;
        repeated.insert(s);
        //cout << times++ << endl;
        //cout << s << endl;
    }
}

int main(){
    chip_size['F']=chip_size['N']=chip_size['T']=chip_size['W']=chip_size['X']=chip_size['Y']=5;
    chip_size['1']=chip_size['2']=chip_size['3']=chip_size['4']=chip_size['5']=chip_size['6']=chip_size['7']=chip_size['8']=chip_size['9']=6;
    boost::progress_timer timee;
    dfs("");
    /*ofstream of;
    of.open(FILENAME);
    of << '[';
    for (auto i: repeated){
        of << '\'' << i << '\'' << ',';
    }
    of << ']' << endl;
    of.close();*/
}