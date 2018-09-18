#include <iostream>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/progress.hpp>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <cstdio>
#include <iomanip>
#include "one_hot.h"


using namespace boost;
using namespace std;


int main(int argc, char *argv[]) {
    progress_timer pt;
    bool verbose=false, verbose2=false, classification = true;
    using namespace boost::program_options;
    int skip_lines;
    int pmax, kmax;
    int poutput, koutput;
    options_description desc("All Options");
    desc.add_options()
            ("help,h", "Show this message")
            ("output,o", value<string>()->default_value("a.csv"), "Output filename, csv format")
            ("train,t", value<string>()->required(), "Train data set, csv format")
            ("validation,v", value<string>()->required(), "Validation data set, csv format")
            ("skip-lines,s", value<int>(&skip_lines)->default_value(1), "Skip the `s` lines of the data set")
            ("pmax", value<int>(&pmax)->default_value(3), "-1 <= p <= pmax")
            ("kmax", value<int>(&kmax)->default_value(25), "1 <= k <= kmax")
            ("poutput", value<int>(&poutput)->default_value(-2), "when p == poutput display detail information")
            ("koutput", value<int>(&koutput)->default_value(0), "when k == koutput display detail information")
            ("regression", "Change to regression mode")
            ("verbose,V", "Output detailed information");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    try {
        notify(vm);
    }
    catch (required_option &a){
        //cerr << "Required --input options!" << endl;
        //cerr << a.get_option_name() << endl;
        cerr << a.what() << endl;
        return 1;
    }
    catch (invalid_command_line_syntax &a){
        cerr << a.what() << endl;
        return 1;
    }
    if (vm.count("verbose") == 2){
        verbose = true;
        verbose2 = true;
    }
    if (vm.count("verbose") == 1){
        verbose = true;
    }
    if (vm.count("regression")){
        classification = false;
    }

    if (vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    char_separator<char> sep(" \r\n", ",");
    OneHot oh;
    string line;

    if (classification) {
        ifstream train(vm["train"].as<string>());
        vector<int> train_text_id;
        int sl = skip_lines;
        while (getline(train, line)&&!line.empty()){
            if (sl-->0){
                continue;
            }
            train_text_id.push_back(oh.addTextClassification(line, ",", sep, true));
        }

        ifstream validation(vm["validation"].as<string>());
        vector<int> validation_text_id;
        sl = skip_lines;
        //while (getline(validation, line)&&!line.empty()){
        while (getline(validation, line)){
            if (sl-->0){
                continue;
            }
            validation_text_id.push_back(oh.addTextClassification(line, ",", sep, false));
        }

        vector<EMOTION> validation_actual_emotion_vector;
        validation_actual_emotion_vector.reserve(validation_text_id.size());
        for (auto i:validation_text_id){
            validation_actual_emotion_vector.emplace_back(oh.getEmotionByTextId(i));
        }

        int len = validation_actual_emotion_vector.size();
        struct pkc{
            int p;
            int k;
            int correct;
            long double correct_rate;
            pkc(int p, int k, int correct, long double correct_rate):p(p), k(k), correct(correct), correct_rate(correct_rate) {}
        };
        vector<pkc> pkc_vector;
        int k=1, p=-1;
        int kpcount = 0;
        pkc_vector.reserve(static_cast<unsigned int>((pmax - p + 1) * (kmax - k + 1)));
        for (p=-1;p<=pmax;p++){
            for(k=1;k<=kmax;k++){
                int correct = 0;
                for(int i=0;i<len;i++){
                    EMOTION actual, predict;
                    actual = validation_actual_emotion_vector[i];
                    predict = oh.getEmotionByTextIdClassification(validation_text_id[i], k, p);
                    correct += (actual==predict);
                    if (verbose2||(p==poutput && k==koutput)){
                        printf("%10s:%10s\n", emotion_reverse[actual].c_str(), emotion_reverse[predict].c_str());
                    }
                }
                long double correct_rate = (long double)(correct * 100) / (long double)len;
                pkc_vector.emplace_back(pkc(p, k, correct, correct_rate));
                if (verbose){
                    cout << "k: " << k << endl;
                    cout << "p: " << p << endl;
                    cout << "All tags: " << len << endl;
                    cout << "Predict correctly: " << correct << endl;
                    cout << "Correct Rate: " << correct_rate << "%\n";
                }
                kpcount++;
            }
        }

        sort(pkc_vector.begin(), pkc_vector.end(), [](const pkc &a, const pkc &b){
            if (a.correct == b.correct) {
                if (a.k == b.k) {
                    return a.p < b.p;
                }
                return a.k < b.k;
            }
            return a.correct_rate > b.correct_rate;
        });
        cout << "k: " << pkc_vector[0].k << endl;
        cout << "p: " << pkc_vector[0].p << endl;
        cout << "All tags: " << len << endl;
        cout << "Predict correctly: " << pkc_vector[0].correct << endl;
        cout << "Correct Rate: " << setprecision(100) << pkc_vector[0].correct_rate << "%\n";
        cout << "kpcount: " << kpcount << endl;
        cout << "expected kpcount: " << (pmax - -1 + 1) * (kmax - 1 + 1) << endl;
        cout << "k & p that has the same result as above: " << endl;
        int correct = pkc_vector[0].correct;
        auto pkcptr = pkc_vector.begin() + 1;
        while (true) {
            if (pkcptr == pkc_vector.end() || pkcptr->correct != correct){
                break;
            }
            cout << pkcptr->k << ' ' << pkcptr->p << endl;
            pkcptr++;
        }
    } else {
        // train data
        ifstream train(vm["train"].as<string>());
        vector<int> train_text_id;
        int sl = skip_lines;
        while (getline(train, line)&&!line.empty()){
            if (sl-->0){
                continue;
            }
            train_text_id.push_back(oh.addTextRegression(line, ",", sep, true));
        }

        // validation data
        ifstream validation(vm["validation"].as<string>());
        vector<int> validation_text_id;
        sl = skip_lines;
        while (getline(validation, line)){
            if (sl-->0){
                continue;
            }
            validation_text_id.push_back(oh.addTextRegression(line, ",", sep, false));
        }

        // get actual validation emotion data
        vector<emotion_array> validation_actual_emotion_vector;
        validation_actual_emotion_vector.reserve(validation_text_id.size());
        for (auto i:validation_text_id){
            validation_actual_emotion_vector.emplace_back(oh.getEmotionByTextIdRegresionOrigin(i));
        }

        int len = validation_actual_emotion_vector.size();
        struct pkc{
            int p;
            int k;
            long double correlation_rate;
            pkc(int p, int k, long double correlation_rate):p(p), k(k), correlation_rate(correlation_rate) {}
        };
        vector<pkc> pkc_vector;
        int k=1, p=-1;
        int kpcount = 0;
        pkc_vector.reserve(static_cast<unsigned int>((pmax - p + 1) * (kmax - k + 1)));
        for (p=-1;p<=pmax;p++){
            for(k=1;k<=kmax;k++){
                long double correlation_rate = 0;
                // predict, actual
                long double xsum[EMOTION_COUNT], ysum[EMOTION_COUNT];
                long double x2sum[EMOTION_COUNT], y2sum[EMOTION_COUNT];
                long double xavg[EMOTION_COUNT], yavg[EMOTION_COUNT];
                long double xysum[EMOTION_COUNT];
                memset(xysum, 0 ,sizeof(long double)*EMOTION_COUNT);
                memset(xsum, 0 ,sizeof(long double)*EMOTION_COUNT);
                memset(ysum, 0 ,sizeof(long double)*EMOTION_COUNT);
                memset(x2sum, 0 ,sizeof(long double)*EMOTION_COUNT);
                memset(y2sum, 0 ,sizeof(long double)*EMOTION_COUNT);
                for (int i=0;i<len;i++){
                    auto predict = oh.getEmotionByTextIdRegresion(validation_text_id[i], k, p);
                    auto actual = validation_actual_emotion_vector[i];
                    if (true) {
                        cout << "p: ";
                        for (int j=0;j<EMOTION_COUNT;j++) {
                            cout << predict[i] << ',';
                        }
                        cout << endl;
                        cout << "a: ";
                        for (int j=0;j<EMOTION_COUNT;j++) {
                            cout << actual[i] << ',';
                        }
                        cout << endl;
                    }
                    for (int j=0;j<EMOTION_COUNT;j++){
                        x2sum[j] += predict[j] * predict[j];
                        y2sum[j] += actual[j] * actual[j];
                        xysum[j] += predict[j] * actual[j];
                        xsum[j] += predict[j];
                        ysum[j] += actual[j];
                    }
                }
                for (int j=0;j<EMOTION_COUNT;j++){
                    xavg[j] = xsum[j] / len;
                    yavg[j] = ysum[j] / len;
                    correlation_rate += (xysum[j] - xavg[j]*yavg[j]*(long double)len) / (sqrt(x2sum[j]-xavg[j]*xavg[j]*(long double)len)*sqrt(y2sum[j]-yavg[j]*yavg[j]*(long double)len));
                }
                correlation_rate /= (long double)EMOTION_COUNT;

                pkc_vector.emplace_back(pkc(p, k, correlation_rate));
                if (verbose){
                    cout << "k: " << k << endl;
                    cout << "p: " << p << endl;
                    //cout << "All tags: " << len << endl;
                    cout << "Predict Correlation: " << correlation_rate << endl;
                }
                kpcount++;
            }
        }

        sort(pkc_vector.begin(), pkc_vector.end(), [](const pkc &a, const pkc &b){
            if (a.correlation_rate == b.correlation_rate) {
                if (a.k == b.k) {
                    return a.p < b.p;
                }
                return a.k < b.k;
            }
            return a.correlation_rate > b.correlation_rate;
        });
        cout << "k: " << pkc_vector[0].k << endl;
        cout << "p: " << pkc_vector[0].p << endl;
        cout << "All tags: " << len << endl;
        cout << "Predict Correlation: " << pkc_vector[0].correlation_rate << endl;
        cout << "kpcount: " << kpcount << endl;
        cout << "expected kpcount: " << (pmax - -1 + 1) * (kmax - 1 + 1) << endl;
        cout << "k & p that has the same result as above: " << endl;
        auto correct = pkc_vector[0].correlation_rate;
        auto pkcptr = pkc_vector.begin() + 1;
        while (true) {
            if (pkcptr == pkc_vector.end() || pkcptr->correlation_rate != correct){
                break;
            }
            cout << pkcptr->k << ' ' << pkcptr->p << endl;
            pkcptr++;
        }


    }
}
