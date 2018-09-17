#include <boost/process.hpp>
#include <iostream>

using namespace std;
using namespace boost::process;
const char TEST[] = "D:xDim=6:yDim=6:zDim=1\n"
                    "C:name=10:layout=0 0 0, 0 1 0, 0 2 0, 1 0 0, 1 1 0, 1 2 0\n"
                    "C:name=11:layout=0 0 0, 0 1 0, 0 2 0, 1 0 0, 1 1 0, 1 2 0\n"
                    "C:name=12:layout=0 0 0, 0 1 0, 0 2 0, 1 0 0, 1 1 0, 1 2 0\n"
                    "C:name=13:layout=0 0 0, 0 1 0, 0 2 0, 1 0 0, 1 1 0, 1 2 0\n"
                    "C:name=14:layout=0 0 0, 0 1 0, 0 2 0, 1 0 0, 1 1 0, 1 2 0\n"
                    "C:name=95:layout=0 0 0, 0 1 0, 1 0 0, 1 1 0, 1 2 0, 0 1 0\n"
                    "~D\n";

void replaceAll(std::string &str, const std::string &from, const std::string &to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int main() {
    //boost::asio::io_service ios;
    //future<string> data;
    ipstream pipe_stream;
    opstream opipe_stream;
    child c(R"(D:\\code\polycube_64.exe -p -V)",
            std_out > pipe_stream,
            std_in < opipe_stream);
    string line;
    opipe_stream << TEST << endl;

    string result;
    while (getline(pipe_stream, line)) {
        if (line == "SOLUTION OUTPUT END\r")
            break;
        result += line;
    }
    c.terminate();
    replaceAll(result, "\r", "\n");
    result.erase(0, 1);
    cerr << result << endl;
    //line = data.get();
    //cerr << line << endl;
}
