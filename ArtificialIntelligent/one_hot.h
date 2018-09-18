#ifndef ARTIFICIALINTELLIGENT_ONE_HOT_H
#define ARTIFICIALINTELLIGENT_ONE_HOT_H

#include <vector>
#include <utility>
#include <boost/tokenizer.hpp>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <cmath>

using namespace std;
using namespace boost;

#define EMOTION_COUNT 6

enum EMOTION {
    ANGER = 0,
    DISGUST = 1,
    FEAR = 2,
    JOY = 3,
    SAD = 4,
    SURPRISE = 5
};

map<string, EMOTION> emotion{
        {"anger", ANGER},
        {"disgust", DISGUST},
        {"fear", FEAR},
        {"joy", JOY},
        {"sad", SAD},
        {"surprise", SURPRISE},
};

class OneHot{
private:
    // word, id
    map<string, int> unique_word_map;
    vector<string> unique_word_vector;
    // id
    vector<vector<int>> text_data;
    // emotion, probability
    vector<EMOTION> text_classification_emotion;
    vector<long double[EMOTION_COUNT]> text_regression_emotion;
    // id, count/exist
    vector<map<int, int>> text_word_map;
    // text id
    map<int, vector<int>> one_hot_map;
    int one_hot_length;
    int unique_word_count;
public:
    OneHot():one_hot_length(0),unique_word_count(0){

    }
    // return text id
    int addTextClassification(const string &str, const string &skip, const char_separator<char> &sep){
        tokenizer<char_separator<char>> tok(str, sep);
        bool ready = false;
        vector<int> data;
        map<int, int> word;
        for (auto &s: tok){
            if(!ready){
                if (s==skip){
                    ready=true;
                } else{
                    int id=getIdByString(s);
                    word[id]=true;
                    data.push_back(id);
                }
            } else{
                text_classification_emotion.push_back(emotion[s]);
            }
        }
        text_data.push_back(data);
        text_word_map.push_back(word);
        return text_data.size()-1;
    }
    int getIdByString (const string &s){
        auto p = unique_word_map[s];
        if (p==0){
            p = unique_word_vector.size()+1;
            unique_word_vector.push_back(s);
            unique_word_count++;
        }
        return p-1;
    }
    vector<int>& getOneHotById(int id){
        vector<int> word;
        auto &one_hot = one_hot_map[id];
        for (auto i:text_word_map[id]){
            word.push_back(i.first);
        }
        sort(word.begin(),word.end());
        if (one_hot.empty()){
            int i=0;
            one_hot.reserve(static_cast<unsigned int>(unique_word_count));
            for(auto j:word){
                while (i++<j){
                    one_hot.push_back(0);
                }
                one_hot.push_back(1);
            }
            while (i++<unique_word_count){
                one_hot.push_back(0);
            }
        } else{
            int len = one_hot.size();
            if (one_hot.capacity() < unique_word_count){
                one_hot.reserve(static_cast<unsigned int>(unique_word_count));
            }
            while (len++<unique_word_count){
                one_hot.push_back(0);
            }
        }
        return one_hot;
    }
    /*
     * p >= 0: L_p Distance
     * p <  0: Cosine Similarity
     */
    static bool distance(const vector<int> &x, const vector<int> &y, long double &ret, int p = 2){
        ret = 0;
        int xlen = x.size(), ylen=y.size();
        if(xlen!=ylen||!xlen){
            return false;
        }
        if(p<0){
            int xsum = 0, ysum = 0;
            for(int i=0;i<xlen;i++){
                xsum+=x[i];
                ysum+=y[i];
                ret += x[i]*y[i];
            }
            ret /= sqrt(xsum)*sqrt(ysum);
        } else{
            int sum = 0;
            long double pp = (long double)1 / (long double)p;
            for(int i=0;i<xlen;i++){
                sum += abs(x[i]-y[i]);
            }
            ret = pow((long double)sum, pp);
        }
        return true;
    }
};

#endif //ARTIFICIALINTELLIGENT_ONE_HOT_H
