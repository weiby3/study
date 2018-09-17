#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <boost/process.hpp>

#define LENGTH 36

using namespace std;
using namespace boost::process;

set<string> repeated;
int chip_size[256], times;
map<string, string> combinations;
string layout[256];

const string CNAME = "C:name=";
const string DBEGIN = "D:xDim=6:yDim=6:zDim=1\n";
const string DBEGIN2 = "D:xDim=8:yDim=6:zDim=1\n"
                       "L:stationary=*\n"
                       "* * . * * . * *\n"
                       "* . . . . . . *\n"
                       ". . . . . . . .\n"
                       ". . . . . . . .\n"
                       "* . . . . . . *\n"
                       "* * . * * . * *\n"
                       "~L\n";
const string DBEGIN3 = "D:xDim=8:yDim=8:zDim=1\n"
                       "L:stationary=*\n"
                       "* * . . * * * *\n"
                       "* . . . . * * *\n"
                       ". . . . . . * *\n"
                       ". . . . . . . *\n"
                       "* . . . . . . .\n"
                       "* * . . . . . .\n"
                       "* * * . . . . *\n"
                       "* * * * . . * *\n"
                       "~L\n";
const string DEND = "~D\n";

void replaceAll(std::string &str, const std::string &from, const std::string &to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

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

void initial(){
    chip_size['F']=chip_size['N']=chip_size['T']=chip_size['W']=chip_size['X']=chip_size['Y']=5;
    chip_size['1']=chip_size['2']=chip_size['3']=chip_size['4']=chip_size['5']=chip_size['6']=chip_size['7']=chip_size['8']=chip_size['9']=6;

    layout['F'] = ":layout=0 2 0, 0 1 0, 1 1 0, 2 1 0, 1 0 0\n";
    layout['N'] = ":layout=0 0 0, 1 0 0, 2 0 0, 2 1 0, 3 1 0\n";
    layout['T'] = ":layout=0 0 0, 1 0 0, 2 0 0, 1 1 0, 1 2 0\n";
    layout['W'] = ":layout=0 2 0, 0 1 0, 1 1 0, 1 0 0, 2 0 0\n";
    layout['X'] = ":layout=0 1 0, 1 0 0, 1 1 0, 1 2 0, 2 1 0\n";
    layout['Y'] = ":layout=0 0 0, 1 0 0, 2 0 0, 3 0 0, 2 1 0\n";
    layout['1'] = ":layout=0 0 0, 0 1 0, 0 2 0, 1 0 0, 1 1 0, 1 2 0\n";
    layout['2'] = ":layout=0 0 0, 0 1 0, 0 2 0, 1 0 0, 1 1 0, 2 0 0\n";
    layout['3'] = ":layout=0 0 0, 0 1 0, 0 2 0, 0 3 0, 1 1 0, 1 2 0\n";
    layout['4'] = ":layout=0 0 0, 0 1 0, 0 2 0, 1 1 0, 1 2 0, 1 3 0\n";
    layout['5'] = ":layout=0 0 0, 0 1 0, 1 1 0, 1 2 0, 2 0 0, 2 1 0\n";
    layout['6'] = ":layout=0 2 0, 1 0 0, 1 1 0, 1 2 0, 1 3 0, 2 2 0\n";
    layout['7'] = ":layout=0 0 0, 0 1 0, 0 2 0, 0 3 0, 0 4 0, 0 5 0\n";
    layout['8'] = ":layout=0 0 0, 0 1 0, 0 2 0, 0 3 0, 1 0 0, 1 3 0\n";
    layout['9'] = ":layout=0 0 0, 0 1 0, 1 0 0, 1 1 0, 1 2 0, 0 1 0\n";
}

void solve(const string &dbegin){
    ipstream ips;
    opstream ops;
    child c(R"(D:\\code\polycube_64.exe -p -V)",
            std_out > ips,
            std_in < ops);
    string in;
    for(auto &i: repeated){
        in = dbegin;
        int k=0;
        for (auto &j: i){
            in += CNAME;
            in += j;
            in += to_string(k++);
            in += layout[j];
        }
        in+=DEND;

        if (!c.running()){
            c.terminate();
            c = child(R"(D:\\code\polycube_64.exe -p -V)",
                      std_out > ips,
                      std_in < ops);
        }
        ops << in << endl;
        /*if (i=="111119"){
            cerr << i << "ERROR!" << endl;
            cerr << in << endl;
        }
        if (!c.running()){
            cerr << i << "ERROR!" << endl;
            cerr << in << endl;
        }*/
        string line, result;
        while (getline(ips, line)) {
            if (line == "SOLUTION OUTPUT END\r")
                break;
            result += line;
        }
        if (!result.empty()){
            replaceAll(result, "\r", "\n");
            result.erase(0, 1);
            combinations[i] = result;
        }
    }
    c.terminate();
}

int main(){
    initial();
    dfs("");

    solve(DBEGIN);

    for (auto &i: combinations){
        cout << i.first << endl;
    }
}