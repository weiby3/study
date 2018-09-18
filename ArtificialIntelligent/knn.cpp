#include <iostream>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/progress.hpp>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include "one_hot.h"


using namespace boost;
using namespace std;


int main(int argc, char *argv[]) {
    progress_timer pt;
    using namespace boost::program_options;
    int skip_lines;
    options_description desc("All Options");
    desc.add_options()
            ("help,h", "Show this message")
            ("output,o", value<string>()->default_value("a.csv"), "Output filename, csv format")
            ("train,t", value<string>()->required(), "Train data set, csv format")
            ("validation,v", value<string>()->required(), "Validation data set, csv format")
            ("skip-lines,s", value<int>(&skip_lines)->default_value(1), "Skip the `s` lines of the data set");

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

    if (vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    char_separator<char> sep(" ", ",");
    ifstream train(vm["train"].as<string>());
    vector<int> text_id;
    OneHot oh;
    string line;
    int sl = skip_lines;
    while (getline(train, line)&&!line.empty()){
        if (sl-->0){
            continue;
        }
        text_id.push_back(oh.addTextClassification(line, ",", sep));
    }
    cout << "Debug\n";
}
