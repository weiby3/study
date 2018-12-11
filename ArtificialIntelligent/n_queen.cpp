#include <iostream>
#include <functional>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

bool deprecatedForwardCheck(int N, int row, vector<map<int, int>> &domains, int *shu,
                            const function<bool(int, int *)> &callback) {
    auto &&domain = domains[row];
    if (domain.empty()) {
        return false;
    }
    for (auto column:domain) {
        if (column.second != 1) {
            continue;
        }
        if (row == N - 1) {
            shu[column.first] = row + 1;
            if (callback(N, shu)) {
                //shu[column] = 0;
                return true;
            }
            shu[column.first] = 0;
        } else {
            shu[column.first] = row + 1;
            for (int i = row + 1; i < N; i++) {
                //domains[i].erase(column);
                domains[i][column.first]--;
                auto column2 = column.first + (i - row);
                auto column3 = column.first - (i - row);
                if (column2 < N) {
                    domains[i][column2]--;
                }
                if (column3 >= 0) {
                    domains[i][column3]--;
                }
            }
            if (deprecatedForwardCheck(N, row + 1, domains, shu, callback)) {
                return true;
            }
            for (int i = row + 1; i < N; i++) {
                //domains[i].emplace(column);
                domains[i][column.first]++;
                auto column2 = column.first + (i - row);
                auto column3 = column.first - (i - row);
                if (column2 < N) {
                    domains[i][column2]++;
                }
                if (column3 >= 0) {
                    domains[i][column3]++;
                }
            }
            shu[column.first] = 0;
        }
    }
    return false;
}

bool validBoard(int N, int x, int y, bool **board) {
    for (int i = x - 1; i >= 0; i--) {
        if (board[i][y]) {
            return false;
        }
    }
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j]) {
            return false;
        }
    }
    for (int i = x - 1, j = y + 1; i >= 0 && j < N; i--, j++) {
        if (board[i][j]) {
            return false;
        }
    }
    return true;
}


bool backtrack(int N, int row, int *shu, bool **board, const function<bool(int, int *)> &callback) {
    for (int column = 0; column < N; column++) {
        if (validBoard(N, row, column, board)) {
            shu[row] = column + 1;
            board[row][column] = true;
            if (row == N - 1) {
                if (callback(N, shu)) {
                    return true;
                }
            } else {
                if (backtrack(N, row + 1, shu, board, callback)) {
                    return true;
                }
            }
            shu[row] = 0;
            board[row][column] = false;
        }
    }
    return false;
}

bool forwardCheck(int N, int row, int **flag, const function<bool(int, int *)> &callback) {
    auto shu = flag[0];
    auto pie = flag[1];
    auto na = flag[2];
    auto nm1 = N - 1;
    auto rp1 = row + 1;
    for (int column = 0; column < N; column++) {
        if (shu[column]) {
            continue;
        }
        auto j = row + column;
        if (pie[j]) {
            continue;
        }
        auto k = nm1 - row + column;
        if (na[k]) {
            continue;
        }
        if (row == nm1) {
            shu[column] = pie[j] = na[k] = rp1;
            if (callback(N, shu)) {
                shu[column] = pie[j] = na[k] = 0;
                return true;
            }
            shu[column] = pie[j] = na[k] = 0;
        } else {
            shu[column] = pie[j] = na[k] = rp1;
            if (forwardCheck(N, rp1, flag, callback)) {
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
    memset(array, 0, sizeof(int) * (5 * N - 2));
    flag[0] = array;
    flag[1] = array + N;
    flag[2] = array + (3 * N - 1);
    return flag;
}

void deleteFlag(int **flag) {
    delete[] flag[0];
    delete[] flag;
}

vector<map<int, int>> makeDomains(int N) {
    vector<map<int, int>> domains;
    for (int i = 0; i < N; i++) {
        map<int, int> domain;
        for (int j = 0; j < N; j++) {
            domain[j] = 1;
        }
        domains.emplace_back(domain);
    }
    return domains;
}

int main(int argc, char *argv[]) {
    int N = 32;
    int choice = 1;
    if (argc == 2) {
        N = atoi(argv[1]);
    } else if (argc == 3) {
        choice = atoi(argv[1]);
        N = atoi(argv[2]);
    }
    using namespace std::placeholders;
    function<bool(int, int *)> callback = bind(&displayOne, _1, _2, true);
    //function<bool(int, int *)> callback = bind(&displayOne, _1, _2, false);
    //function<bool(int, int *)> callback = bind(&countOnly, _1, _2, false);

    // forwardCheck
    /*auto flag = newFlag(N);
    forwardCheck(N, 0, flag, callback);
    deleteFlag(flag);*/

    // forward check
    /*auto shu = new int[N];
    memset(shu, 0, sizeof(int) * N);
    auto domains = makeDomains(N);
    deprecatedForwardCheck(N, 0, domains, shu, callback);
    delete[] shu;*/

    if (choice == 1) {
        auto flag = newFlag(N);
        forwardCheck(N, 0, flag, callback);
        deleteFlag(flag);
    } else if (choice == 2) {
        auto shu = new int[N];
        memset(shu, 0, sizeof(int) * N);
        auto board = newBoard(N);
        backtrack(N, 0, shu, board, callback);
        deleteBoard(board);
        delete[] shu;
    } else {
        cerr << "Not Implemented!\n";
    }

    cout << solutionCount << endl;
}