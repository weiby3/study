#include <iostream>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/progress.hpp>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include "tf.h"
#include "idf.h"


//using namespace Eigen;
using namespace std;


int main(int argc, char *argv[]) {
    progress_timer pt;
    using namespace boost::program_options;
    int skip_lines;
    options_description desc("All Options");
    desc.add_options()
            ("help,h", "Show this message")
            ("output,o", value<string>()->default_value("a.txt"), "Output filename")
            ("input,i", value<string>()->required(), "Input data set, csv format")
            ("skip-lines,s", value<int>(&skip_lines)->default_value(1), "Skip the `s` lines of the data set");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    try {
        notify(vm);
    }
    catch (required_option &a){
        cerr << "Required --input options!" << endl;
        return 1;
    }

    if (vm.count("help")) {
        cout << desc << endl;
        return 1;
    }




    ifstream ifs(vm["input"].as<string>());
    IDF idf;
    vector<TF> tf_vector;
    using namespace boost;
    char_separator<char> sep("\t: 0123456789");
    string line, skip("surprise");
    while (getline(ifs, line)&&!line.empty()){
        TF tf(line, skip, idf, sep);
        tf_vector.push_back(tf);
    }
    ifs.close();
    idf.calculateIDF();

    ofstream ofs(vm["output"].as<string>());
    for (auto &i:tf_vector){
        string tfidf;
        i.getTFIDF(tfidf);
        ofs<<tfidf;
    }
    ofs.close();
}
