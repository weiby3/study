#ifndef ARTIFICIALINTELLIGENT_TF_H
#define ARTIFICIALINTELLIGENT_TF_H

#include <vector>
#include <utility>
#include <boost/tokenizer.hpp>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "idf.h"

using namespace std;
using namespace boost;

class TF {
private:
    // word id
    vector<int> word_vector;
    // word id, count
    map<int, int> word_map;
    int all_words;
    IDF &idf;
    const char_separator<char> &sep;
    bool exist(const string &s){
        auto id = idf.getIdByString(s);
        return word_map[id]!=0;
    }
    void addWord(const string &s){
        auto id = idf.getIdByString(s);
        word_map[id]++;
        word_vector.push_back(id);
        all_words++;
    }
public:
    TF() = delete;
    TF(const string &s, const string &skip, IDF &idf, const char_separator<char> &sep):idf(idf),sep(sep),all_words(0){
        tokenizer<char_separator<char>> tok(s, sep);
        bool ready = false;
        for (auto &str:tok){
            if (!ready){
                if(str!=skip){
                    continue;
                } else{
                    ready=true;
                }
            } else{
                if (!exist(str)){
                    idf.addWordCount(str);
                }
                addWord(str);
            }
        }
        idf.addTextCount();
    }
    //TF(const TF&) = delete;
    TF& operator=(const TF&)= delete;
    bool getTFIDF(string &ret, bool with_zero = true){
        stringstream ss;
        ss << setprecision(100);
        int len = idf.getWordCount(), i=0;
        auto sum = (long double)all_words;
        vector<int> word_vector2(word_vector);
        sort(word_vector2.begin(), word_vector2.end());
        for (const auto &j:word_vector2){
            while (i<j){
                if(with_zero){
                    ss<<0<<' ';
                }
                i++;
            }
            long double tmp = idf.getIDFById(j);
            tmp *= ((long double)word_map[j]/sum);
            ss<<tmp<<' ';
            i++;
        }
        while (i<len){
            if(with_zero){
                ss<<0<<' ';
            }
            i++;
        }
        ss<<endl;
        ret = ss.str();
        return true;
    }
};

#endif //ARTIFICIALINTELLIGENT_TF_H
