#include <iostream>
#include <functional>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <set>

using namespace std;

bool forwardCheck(int N, int row, vector<set<int>> &domains, int *shu, const function<bool(int, int *)> &callback) {
    auto &&domain = domains[row];
    if (domain.empty()) {
        return false;
    }
    for (auto column:domain) {
        if (row == N - 1) {
            shu[column] = row + 1;
            if (callback(N, shu)) {
                //shu[column] = 0;
                return true;
            }
            shu[column] = 0;
        } else {
            shu[column] = row + 1;
            bool erase_column2 = false, erase_column3 = false;
            for (int i = row + 1; i < N; i++) {
                domains[i].erase(column);
                auto column2 = column + (i - row);
                auto column3 = column - (i - row);
                if (column2 < N) {
                    domains[i].erase(column2);
                }
                if (column3 >= 0) {
                    domains[i].erase(column3);
                }
            }
            if (forwardCheck(N, row + 1, domains, shu, callback)) {
                return true;
            }
            for (int i = row + 1; i < N; i++) {
                domains[i].emplace(column);
                auto column2 = column + (i - row);
                auto column3 = column - (i - row);
                if (column2 < N) {
                    domains[i].emplace(column2);
                }
                if (column3 >= 0) {
                    domains[i].emplace(column3);
                }
            }
            shu[column] = 0;
        }
    }
    return false;
}

bool backtrack(int N, int row, int *flag[3], const function<bool(int, int *)> &callback) {
    auto shu = flag[0];
    auto pie = flag[1];
    auto na = flag[2];
    for (int column = 0; column < N; column++) {
        auto j = row + column;
        auto k = N - 1 - row + column;
        if (shu[column] || pie[j] || na[k]) {
            continue;
        }
        if (row == N - 1) {
            shu[column] = pie[j] = na[k] = row + 1;
            if (callback(N, shu)) {
                shu[column] = pie[j] = na[k] = 0;
                return true;
            }
            shu[column] = pie[j] = na[k] = 0;
        } else {
            shu[column] = pie[j] = na[k] = row + 1;
            if (backtrack(N, row + 1, flag, callback)) {
                return true;
            }
            shu[column] = pie[j] = na[k] = 0;
        }
    }
    return false;
}

bool **newBoard(int N) {
    auto array = new bool[N * N];
    memset(array, 0, sizeof(bool) * N * N);
    auto board = new bool *[N];
    for (int i = 0; i < N; i++) {
        board[i] = array + i * N;
    }
    return board;
}

void deleteBoard(bool **board) {
    delete[] board[0];
    delete[] board;
}

const char QUEEN[] = "●";
const char BLANK[] = " ";

const char *getBoardString(int position, int N, bool **board) {
    if (board[position / N][position % N]) {
        return const_cast<const char *>(QUEEN);
    }
    return const_cast<const char *>(BLANK);
}

void printBoard(int N, bool **board, ostream &o) {
    if (N < 1) {
        return;
    }
    int position = 0;

    // ┏━┳━┳━┳━┳━┳━┳━┳━┓
    o << "┏━";
    for (int i = 1; i < N; i++) {
        o << "┳━";
    }
    o << "┓\n";

    // ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃
    o << "┃";
    for (int i = 0; i < N; i++) {
        o << getBoardString(position++, N, board) << "┃";
    }
    o << "\n";

    // ┣━╋━╋━╋━╋━╋━╋━╋━┫
    // ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃
    for (int i = 1; i < N; i++) {
        // ┣━╋━╋━╋━╋━╋━╋━╋━┫
        o << "┣━";
        for (int j = 1; j < N; j++) {
            o << "╋━";
        }
        o << "┫\n";

        // ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃
        o << "┃";
        for (int j = 0; j < N; j++) {
            o << getBoardString(position++, N, board) << "┃";
        }
        o << "\n";
    }

    // ┗━┻━┻━┻━┻━┻━┻━┻━┛
    o << "┗━";
    for (int i = 1; i < N; i++) {
        o << "┻━";
    }
    o << "┛\n";
}

int solutionCount = 0;

bool displayOne(int N, const int *column, bool return_value) {
    solutionCount++;
    auto board = newBoard(N);
    for (int j = 0; j < N; j++) {
        board[column[j] - 1][j] = true;
    }
    printBoard(N, board, cout);
    deleteBoard(board);
    return return_value;
}

bool countOnly(int N, int *column, bool return_value) {
    solutionCount++;
    return return_value;
}

int **newFlag(int N) {
    auto flag = new int *[3];
    auto array = new int[5 * N - 2];
    flag[0] = array;
    flag[1] = array + N;
    flag[2] = array + (3 * N - 1);
    return flag;
}

void deleteFlag(int **flag) {
    delete[] flag[0];
    delete[] flag;
}

vector<set<int>> makeDomains(int N) {
    vector<set<int>> domains;
    domains.reserve(static_cast<unsigned long>(N));
    for (int i = 0; i < N; i++) {
        set<int> domain;
        for (int j = 0; j < N; j++) {
            domain.insert(j);
        }
        domains.emplace_back(domain);
    }
    return domains;
}

int main(int argc, char *argv[]) {
    int N = 5;
    if (argc > 1) {
        N = atoi(argv[1]);
    }
    using namespace std::placeholders;
    //function<bool(int, int *)> callback = bind(&displayOne, _1, _2, true);
    function<bool(int, int *)> callback = bind(&displayOne, _1, _2, false);
    //function<bool(int, int *)> callback = bind(&countOnly, _1, _2, false);

    // backtrack
    /*auto flag = newFlag(N);
    backtrack(N, 0, flag, callback);
    deleteFlag(flag);*/

    // forward check
    auto shu = new int[N];
    memset(shu, 0, sizeof(int) * N);
    auto domains = makeDomains(N);
    forwardCheck(N, 0, domains, shu, callback);
    delete[] shu;

    cout << solutionCount << endl;
}