#ifndef ARTIFICIALINTELLIGENT_ONE_HOT_H
#define ARTIFICIALINTELLIGENT_ONE_HOT_H

#include <vector>
#include <utility>
#include <boost/tokenizer.hpp>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <cmath>
#include <numeric>
#include <cstring>

using namespace std;
using namespace boost;

#define EMOTION_COUNT 6

enum EMOTION {
    ANGER = 0,
    DISGUST = 1,
    FEAR = 2,
    JOY = 3,
    SAD = 4,
    SURPRISE = 5,
    UNKNOWN = 6
};

map<string, EMOTION> emotion{
        {"anger", ANGER},
        {"disgust", DISGUST},
        {"fear", FEAR},
        {"joy", JOY},
        {"sad", SAD},
        {"surprise", SURPRISE},
        {"?", UNKNOWN}
};

string emotion_reverse[]{
    "anger",
    "disgust",
    "fear",
    "joy",
    "sad",
    "surprise",
    "?"
};

struct emotion_array{
    long double ea[EMOTION_COUNT]{};

    emotion_array() {
        memset(ea, 0 , sizeof(long double) * EMOTION_COUNT);
    }
    explicit emotion_array(const long double e_a[EMOTION_COUNT]) {
        for (int i=0;i<EMOTION_COUNT;i++){
            ea[i] = e_a[i];
        }
    }
    long double& operator[](int i){
        return ea[i];
    }
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
    vector<emotion_array> text_regression_emotion;
    // id, count/exist
    vector<map<int, int>> text_word_map;
    // text id
    map<int, vector<int>> one_hot_map;
    // p, validation text id, train text id, distance
    map<int, map<int, vector<pair<int, long double> > > > distance_map;
    int one_hot_length;
    int unique_word_count;
    int train_length;
public:
    OneHot():one_hot_length(0),unique_word_count(0),train_length(0){
        //emotion["anger"] = ANGER;
        //emotion["disgust"] = DISGUST;
        //emotion["fear"] = FEAR;
        //emotion["joy"] = JOY;
        //emotion["sad"] = SAD;
        //emotion["surprise"] = SURPRISE;
        //emotion["?"] = UNKNOWN;
        //for (auto &i:emotion) {
        //    cout << i.first << ' ' << i.second << endl;
        //}
    }
    // return text id
    // train if train data, true; if validation data, false
    int addTextClassification(const string &str, const string &skip, const char_separator<char> &sep, bool train){
        tokenizer<char_separator<char>> tok(str, sep);
        bool ready = false;
        vector<int> data;
        map<int, int> word;
        for (auto &s: tok){
            //cout << s << endl;
            if(!ready){
                if (s==skip){
                    ready=true;
                } else{
                    int id=getIdByString(s);
                    word[id]=true;
                    data.push_back(id);
                }
            } else{
                if (s.empty()){
                    continue;
                }
                EMOTION emotion_tmp = emotion[s];
                //cout << s << ":" << emotion_tmp << endl;
                text_classification_emotion.push_back(emotion_tmp);
            }
        }
        text_data.push_back(data);
        text_word_map.push_back(word);
        train_length += train;
        return text_data.size()-1;
    }
    // return text id
    // train if train data, true; if validation data, false
    int addTextRegression(const string &str, const string &skip, const char_separator<char> &sep, bool train){
        tokenizer<char_separator<char>> tok(str, sep);
        bool ready = false;
        vector<int> data;
        map<int, int> word;
        int j = 0;
        emotion_array _emotion_array;
        for (auto &s: tok){
            //cout << s << endl;
            if(!ready){
                if (s==skip){
                    ready=true;
                } else{
                    int id=getIdByString(s);
                    word[id]=true;
                    data.push_back(id);
                }
            } else{
                if (s.empty() || s==skip){
                    continue;
                }char *tmpend;
                long double tmpld = strtold(s.c_str(), &tmpend);
                _emotion_array[j++] = tmpld;
            }
        }
        text_regression_emotion.push_back(_emotion_array);
        text_data.push_back(data);
        text_word_map.push_back(word);
        train_length += train;
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
    vector<int>& getOneHotByTextId(int text_id){
        vector<int> word;
        auto &one_hot = one_hot_map[text_id];
        for (auto i:text_word_map[text_id]){
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
    const vector<pair<int, long double>>& getDistanceVector(int p, int validation_text_id){
        auto &dv = distance_map[p][validation_text_id];
        if(dv.empty()){
            dv.reserve(static_cast<unsigned int>(train_length));
            for (int i=0;i<train_length;i++){
                long double r;
                distance(getOneHotByTextId(validation_text_id), getOneHotByTextId(i), r, p);
                dv.emplace_back(pair<int, long double>(i, r));
            }
        }
        return dv;
    }
    /*
     * x one hot matrix
     * y one hot matrix
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
    EMOTION getEmotionByTextId(int text_id){
        return text_classification_emotion[text_id];
    }
    /*
     * k represent kNN's k
     * p >= 0: L_p Distance
     * p <  0: Cosine Similarity
     */
    EMOTION getEmotionByTextIdClassification(int text_id, int k, int p){
        if (k>train_length){
            return UNKNOWN;
        }
        // train text id, distance
        vector<pair<int, long double>> distance_vector(getDistanceVector(p, text_id));
        if (p < 0){
            sort(distance_vector.begin(), distance_vector.end(), [](const pair<int, long double> &a, const pair<int, long double> &b){
                return a.second > b.second;
            });
        }else{
            sort(distance_vector.begin(), distance_vector.end(), [](const pair<int, long double> &a, const pair<int, long double> &b){
                return a.second < b.second;
            });
        }
        // emotion, count
        map<EMOTION, int> emotion_count;
        for (int i=0;i<k;i++){
            emotion_count[getEmotionByTextId(distance_vector[i].first)]++;
        }
        int max_count=0;
        EMOTION ret = UNKNOWN;
        for(auto &i:emotion_count){
            if(i.second>max_count){
                max_count=i.second;
                ret=i.first;
            }
        }
        return ret;
    }
    const emotion_array getEmotionByTextIdRegresionOrigin(int text_id){
        return text_regression_emotion[text_id];
    }
    const emotion_array getEmotionByTextIdRegresion(int text_id, int k, int p){
        long double ret[EMOTION_COUNT];
        if (k>train_length){
            return emotion_array(ret);
        }
        // train text id, distance
        vector<pair<int, long double>> distance_vector(getDistanceVector(p, text_id));
        if (p < 0){
            sort(distance_vector.begin(), distance_vector.end(), [](const pair<int, long double> &a, const pair<int, long double> &b){
                return a.second > b.second;
            });
        }else{
            sort(distance_vector.begin(), distance_vector.end(), [](const pair<int, long double> &a, const pair<int, long double> &b){
                return a.second < b.second;
            });
        }
        for (long double &i : ret) {
            i = 0;
        }
        for (int i=0;i<k;i++){
            int id=distance_vector[i].first;
            for (int j=0;j<EMOTION_COUNT;j++){
                // train probability / distance
                ret[j] += text_regression_emotion[id][j] / distance_vector[i].second;
            }
        }
        long double sum = 0;
        for (long double &i : ret) {
            sum += i;
        }
        for (long double &i : ret) {
            i /= sum;
        }
        return emotion_array(ret);
    }
};

#endif //ARTIFICIALINTELLIGENT_ONE_HOT_H
