#ifndef ARTIFICIALINTELLIGENT_IDF_H
#define ARTIFICIALINTELLIGENT_IDF_H

#include <vector>
#include <map>
#include <utility>
#include <string>
#include <cmath>

using namespace std;

class IDF{
private:
    // id, count
    map<string, pair<int, int>> unique_word_map;
    vector<string> unique_word_vector;
    vector<long double> unique_word_idf;
    int count;
    bool calculated;
    long double idf(const string &s){
        return log((long double)count/(long double)(1+unique_word_map[s].second));
    }
public:
    IDF() : count(0), calculated(false) {
        unique_word_idf.clear();
        unique_word_map.clear();
        unique_word_vector.clear();
    }
    IDF(const IDF&) = delete;
    IDF& operator=(const IDF&) = delete;
    bool exist(const string &s){
        auto &p = unique_word_map[s];
        /*auto len = unique_word_vector.size();
        if (p.second == 0){
            if (p.first>=len){
                p.first=len;
                unique_word_vector.push_back(s);
            }
            return false;
        }
        return true;*/
        return p.second!=0;
    }
    const string& getStringById(int id){
        return unique_word_vector[id];
    }
    int getIdByString(const string &s){
        auto &p = unique_word_map[s];
        int len = unique_word_vector.size();
        if(p.first==0){
            p.first = len+1;
            unique_word_vector.push_back(s);
        }
        return p.first-1;
        //return p.second==0?-1:p.first;
    }
    int getTextCount(){
        return count;
    }
    void addTextCount(){
        count++;
    }
    void addWordCount(const string &s){
        auto &p = unique_word_map[s];
        int len = unique_word_vector.size();
        if(p.first==0){
            p.first = len+1;
            unique_word_vector.push_back(s);
        }
        p.second++;
    }
    void calculateIDF(){
        unique_word_idf.clear();
        for (auto &s:unique_word_vector){
            unique_word_idf.push_back(idf(s));
        }
        calculated=true;
    }
    long double getIDFById(int id){
        return calculated?unique_word_idf[id]:0;
    }
    int getWordCount(){
        return unique_word_vector.size();
    }
};

#endif //ARTIFICIALINTELLIGENT_IDF_H
