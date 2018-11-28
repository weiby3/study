#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <climits>
#include <random>

using namespace std;

bool DEBUG = true;

const uint16_t N = 8;
const uint16_t N2 = 64;

const uint64_t BLACK_INITIAL = 34628173824UL; // 0x810000000
const uint64_t WHITE_INITIAL = 68853694464UL; // 0x1008000000
const int DIRECTIONS = 8;
const int directions[8][2] = {
        {-1, -1},
        {-1, 0},
        {-1, 1},
        {0,  1},
        {1,  1},
        {1,  0},
        {1,  -1},
        {0,  -1}
};
const int weights[N][N] = {
        {90,  -60, 10, 10, 10, 10, -60, 90},
        {-60, -80, 5,  5,  5,  5,  -80, -60},
        {10,  5,   1,  1,  1,  1,  5,   10},
        {10,  5,   1,  1,  1,  1,  5,   10},
        {10,  5,   1,  1,  1,  1,  5,   10},
        {10,  5,   1,  1,  1,  1,  5,   10},
        {-60, -80, 5,  5,  5,  5,  -60, -80},
        {90,  -60, 10, 10, 10, 10, -60, 90},
};
const char BOARD_STRING[] = "  A B C D E F G H\n"
                            " ┏━┳━┳━┳━┳━┳━┳━┳━┓\n"
                            "1┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃\n"
                            " ┣━╋━╋━╋━╋━╋━╋━╋━┫\n"
                            "2┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃\n"
                            " ┣━╋━╋━╋━╋━╋━╋━╋━┫\n"
                            "3┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃\n"
                            " ┣━╋━╋━╋━╋━╋━╋━╋━┫\n"
                            "4┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃\n"
                            " ┣━╋━╋━╋━╋━╋━╋━╋━┫\n"
                            "5┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃\n"
                            " ┣━╋━╋━╋━╋━╋━╋━╋━┫\n"
                            "6┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃\n"
                            " ┣━╋━╋━╋━╋━╋━╋━╋━┫\n"
                            "7┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃\n"
                            " ┣━╋━╋━╋━╋━╋━╋━╋━┫\n"
                            "8┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ┃\n"
                            " ┗━┻━┻━┻━┻━┻━┻━┻━┛\n";

enum Color {
    BLANK = 0,
    BLACK = 1,
    WHITE = 2,
    UNKNOWN = 3,
    CURRENT_COLOR = 4
};

struct Point {

    uint16_t x;
    uint16_t y;

    Point() : x(0), y(0) {}

    Point(uint16_t x, uint16_t y) : x(x), y(y) {}

    Point(const Point &p) = default;

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

bool valid_point(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N);
}

uint64_t get_stable_board(uint64_t board) {
    uint64_t board_l, board_r, board_u, board_d, board_ul, board_ur, board_dl, board_dr;

    board_l = board;
    board_l &= board_l >> 1UL | 0x8080808080808080UL;
    board_l &= board_l >> 2UL | 0xc0c0c0c0c0c0c0c0UL;
    board_l &= board_l >> 4UL | 0xf0f0f0f0f0f0f0f0UL;

    board_r = board;
    board_r &= board_r << 1UL | 0x0101010101010101UL;
    board_r &= board_r << 2UL | 0x0303030303030303UL;
    board_r &= board_r << 4UL | 0x0f0f0f0f0f0f0f0fUL;

    board_u = board;
    board_u &= board_u >> 8UL | 0xff00000000000000UL;
    board_u &= board_u >> 16UL | 0xffff000000000000UL;
    board_u &= board_u >> 32UL | 0xffffffff00000000UL;

    board_d = board;
    board_d &= board_d << 8UL | 0x00000000000000ffUL;
    board_d &= board_d << 16UL | 0x000000000000ffffUL;
    board_d &= board_d << 32UL | 0x00000000ffffffffUL;

    board_ul = board;
    board_ul &= board_ul >> 9UL | 0xff80808080808080UL;
    board_ul &= board_ul >> 18UL | 0xffffc0c0c0c0c0c0UL;
    board_ul &= board_ul >> 36UL | 0xfffffffff0f0f0f0UL;

    board_dl = board;
    board_dl &= board_dl << 7UL | 0x80808080808080ffUL;
    board_dl &= board_dl << 14UL | 0xc0c0c0c0c0c0ffffUL;
    board_dl &= board_dl << 28UL | 0xf0f0f0f0ffffffffUL;

    board_ur = board;
    board_ur &= board_ur >> 7UL | 0xff01010101010101UL;
    board_ur &= board_ur >> 14UL | 0xffff030303030303UL;
    board_ur &= board_ur >> 28UL | 0xffffffff0f0f0f0fUL;

    board_dr = board;
    board_dr &= board_dr << 9UL | 0x01010101010101ffUL;
    board_dr &= board_dr << 18UL | 0x030303030303ffffUL;
    board_dr &= board_dr << 36UL | 0x0f0f0f0fffffffffUL;

    return (board_l | board_r) & (board_u | board_d) & (board_ul | board_dr) & (board_ur | board_dl);
}

struct Board {
    uint64_t black;
    uint64_t white;
    Color current_player;

    Board() : black(BLACK_INITIAL), white(WHITE_INITIAL),
              current_player(BLACK) {}
    //Board() = delete;

    Board(uint64_t _black, uint64_t _white, Color _current_player) : black(_black), white(_white),
                                                                     current_player(_current_player) {}

    Board(const Board &b) = default;

    explicit Board(Color _current_player) : black(BLACK_INITIAL), white(WHITE_INITIAL),
                                            current_player(_current_player) {}

    vector<Point> get_points(Color c) const {
        uint64_t chess;
        if (c == BLACK) {
            chess = black;
        } else if (c == WHITE) {
            chess = white;
        } else {
            throw invalid_argument("Expected BLACK(1) or WHITE(2)!");
        }
        vector<Point> points;
        for (uint16_t i = 0; i < N2; i++) {
            uint64_t tmp = 1UL << i;
            if (chess & tmp) {
                points.emplace_back(Point(i / N, i % N));
            }
        }
        return points;
    }

    int get_points_count(Color c) const {
        uint64_t chess;
        if (c == BLACK) {
            chess = black;
        } else if (c == WHITE) {
            chess = white;
        } else {
            throw invalid_argument("Expected BLACK(1) or WHITE(2)!");
        }
        int count = 0;
        for (uint16_t i = 0; i < N2; i++) {
            uint64_t tmp = 1UL << i;
            if (chess & tmp) {
                count++;
            }
        }
        return count;
    }

    bool have_chess(Color c, int position) const {
        uint64_t chess;
        if (c == BLACK) {
            chess = black;
        } else if (c == WHITE) {
            chess = white;
        } else {
            throw invalid_argument("Expected BLACK(1) or WHITE(2)!");
        }
        uint64_t tmp = 1UL << static_cast<uint64_t>(position);
        return static_cast<bool>(chess & tmp);
    }

    Color get_chess(int position) const {
        if (have_chess(BLACK, position)) {
            return BLACK;
        } else if (have_chess(WHITE, position)) {
            return WHITE;
        }
        return BLANK;
    }

    Color get_chess(int x, int y) const {
        if (!valid_point(x, y)) {
            stringstream s;
            s << "Index out of range or unplayable, x: " << x << ", y: " << y;
            throw invalid_argument(s.str());
        }
        auto position = x * N + y;
        if (have_chess(BLACK, position)) {
            return BLACK;
        } else if (have_chess(WHITE, position)) {
            return WHITE;
        }
        return BLANK;
    }

    vector<Point> get_stable_points(Color c = CURRENT_COLOR) const {
        if (c == CURRENT_COLOR) {
            c = current_player;
        }
        uint64_t chess;
        if (c == BLACK) {
            chess = black;
        } else if (c == WHITE) {
            chess = white;
        } else {
            throw invalid_argument("Expected BLACK(1) or WHITE(2)!");
        }
        chess = get_stable_board(chess);
        vector<Point> points;
        for (uint16_t x = 0; x < N; x++) {
            for (uint16_t y = 0; y < N; y++) {
                auto position = x * N + y;
                uint64_t tmp = 1UL << static_cast<uint64_t>(position);
                if (chess & tmp) {
                    points.emplace_back(Point(x, y));
                }
            }
        }
        return points;
    }

    int get_stable_points_count(Color c = CURRENT_COLOR) const {
        if (c == CURRENT_COLOR) {
            c = current_player;
        }
        uint64_t chess;
        if (c == BLACK) {
            chess = black;
        } else if (c == WHITE) {
            chess = white;
        } else {
            throw invalid_argument("Expected BLACK(1) or WHITE(2)!");
        }
        chess = get_stable_board(chess);
        int count = 0;
        for (uint16_t x = 0; x < N; x++) {
            for (uint16_t y = 0; y < N; y++) {
                auto position = x * N + y;
                uint64_t tmp = 1UL << static_cast<uint64_t>(position);
                if (chess & tmp) {
                    count++;
                }
            }
        }
        return count;
    }

    vector<Point> get_playable_chess(Color current_color = CURRENT_COLOR) const {
        if (current_color == CURRENT_COLOR) {
            current_color = current_player;
        }
        vector<Point> points;
        auto next_color = current_color == BLACK ? WHITE : BLACK;
        for (uint16_t x = 0; x < N; x++) {
            for (uint16_t y = 0; y < N; y++) {
                if (get_chess(x, y) != BLANK) {
                    continue;
                }
                for (auto direction : directions) {
                    auto nx = x + direction[0];
                    auto ny = y + direction[1];
                    if (!valid_point(nx, ny)) {
                        continue;
                    }
                    bool breakable = false;
                    while (get_chess(nx, ny) == next_color) {
                        nx += direction[0];
                        ny += direction[1];
                        if (!valid_point(nx, ny)) {
                            break;
                        }
                        if (get_chess(nx, ny) == current_color) {
                            points.emplace_back(Point(x, y));
                            breakable = true;
                            break;
                        }
                    }
                    if (breakable) {
                        break;
                    }
                }
            }
        }
        return points;
    }

    int get_playable_chess_count(Color current_color = CURRENT_COLOR) const {
        if (current_color == CURRENT_COLOR) {
            current_color = current_player;
        }
        int count = 0;
        auto next_color = current_color == BLACK ? WHITE : BLACK;
        for (uint16_t x = 0; x < N; x++) {
            for (uint16_t y = 0; y < N; y++) {
                if (get_chess(x, y) != BLANK) {
                    continue;
                }
                for (auto direction : directions) {
                    auto nx = x + direction[0];
                    auto ny = y + direction[1];
                    if (!valid_point(nx, ny)) {
                        continue;
                    }
                    bool breakable = false;
                    while (get_chess(nx, ny) == next_color) {
                        nx += direction[0];
                        ny += direction[1];
                        if (!valid_point(nx, ny)) {
                            break;
                        }
                        if (get_chess(nx, ny) == current_color) {
                            count++;
                            breakable = true;
                            break;
                        }
                    }
                    if (breakable) {
                        break;
                    }
                }
            }
        }
        return count;
    }

    bool playable(int x, int y, Color c = CURRENT_COLOR) const {
        if (c == CURRENT_COLOR) {
            c = current_player;
        }
        auto points = get_playable_chess(c);
        Point p(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
        for (auto &&point:points) {
            if (p == point) {
                return true;
            }
        }
        return false;
    }

    void set_chess(Color color, int position) {
        uint64_t tmp = 1UL << static_cast<uint64_t>(position);
        auto opposite_color = color == BLACK ? BLACK : WHITE;
        auto &or_color = color == BLACK ? black : white;
        auto &and_color = color == BLACK ? white : black;
        or_color |= tmp;
        and_color &= ~tmp;
    }

    void set_chess(Color color, int x, int y) {
        auto position = x * N + y;
        uint64_t tmp = 1UL << static_cast<uint64_t>(position);
        auto opposite_color = color == BLACK ? BLACK : WHITE;
        auto &or_color = color == BLACK ? black : white;
        auto &and_color = color == BLACK ? white : black;
        or_color |= tmp;
        and_color &= ~tmp;
    }

    void play_chess(int x, int y) {
        if (!valid_point(x, y) || !playable(x, y)) {
            stringstream s;
            s << "Index out of range or unplayable, x: " << x << ", y: " << y;
            throw invalid_argument(s.str());
        }
        auto next_player = current_player == BLACK ? WHITE : BLACK;
        for (auto direction : directions) {
            auto nx = x + direction[0];
            auto ny = y + direction[1];
            if (!valid_point(nx, ny)) {
                continue;
            }
            while (get_chess(nx, ny) == next_player) {
                nx += direction[0];
                ny += direction[1];
                if (!valid_point(nx, ny)) {
                    break;
                }
                if (get_chess(nx, ny) == current_player) {
                    auto tx = x + direction[0];
                    auto ty = y + direction[1];
                    while (!(tx == nx && ty == ny)) {
                        set_chess(current_player, tx, ty);
                        tx += direction[0];
                        ty += direction[1];
                    }
                    break;
                }
            }
        }
        set_chess(current_player, x, y);
        current_player = next_player;
    }

    bool operator==(const Board &other) const {
        return black == other.black && white == other.white;
    }

    int evaluate() const {
        int black_score = 0, white_score = 0;
        int black_count = 100 * get_points_count(BLACK), white_count = 100 * get_points_count(WHITE);
        int black_stable = 200 * get_stable_points_count(BLACK), white_stable = 200 * get_stable_points_count(WHITE);
        int black_playable = 5 * get_playable_chess_count(BLACK), white_playable = 5 * get_playable_chess_count(WHITE);
        for (uint16_t x = 0; x < N; x++) {
            for (uint16_t y = 0; y < N; y++) {
                switch (get_chess(x, y)) {
                    case BLACK:
                        black_score += weights[x][y];
                        break;
                    case WHITE:
                        white_score += weights[x][y];
                        break;
                    default:
                        break;
                }
            }
        }
        return (black_score - white_score) + (black_count - white_count) + (black_stable - white_stable) +
               (black_playable - white_playable);
    }

    bool terminate() const {
        return get_playable_chess_count() == 0;
    }

    Color who_win() const {
        auto black_count = get_points_count(BLACK);
        auto white_count = get_points_count(WHITE);
        return black_count == white_count ? BLANK :
               black_count > white_count ? BLACK : WHITE;
    }

    string as_string() const {
        stringstream o;
        o << "=====================\n";
        o << "Current Score: " << evaluate() << endl;
        o << "Next Player: " << (current_player == BLACK ? "Black" : "White") << endl;
        int i = 0;
        for (auto c:BOARD_STRING) {
            if (c == ' ') {
                switch (get_chess(i++)) {
                    case BLACK:
                        o << "●";
                        break;
                    case WHITE:
                        o << "○";
                        break;
                    default:
                        o << ' ';
                        break;
                }
            } else {
                o << c;
            }
        }
        o << "=====================\n";
        return o.str();
    }
};

ostream &operator<<(ostream &o, const Board &b) {
    o << "=====================\n";
    //o << "Current Score: " << (b.current_player == BLACK ? b.evaluate() : -b.evaluate()) << endl;
    o << "Current Score: " << b.evaluate() << endl;
    o << "Next Player: " << (b.current_player == BLACK ? "Black" : "White") << endl;
    int i = 0;
    for (auto c:BOARD_STRING) {
        if (c == ' ') {
            switch (b.get_chess(i++)) {
                case BLACK:
                    o << "●";
                    break;
                case WHITE:
                    o << "○";
                    break;
                default:
                    o << ' ';
                    break;
            }
        } else {
            o << c;
        }
    }
    o << "=====================\n";
    return o;
}

ostream &operator<<(ostream &o, const Point &p) {
    o << '(' << p.x << ", " << p.y << ')';
    return o;
}

int alpha_beta(Board &b, bool maximizingPlayer = true, int depth = 10, int alpha = INT_MIN, int beta = INT_MAX) {
    if (depth == 0 || b.terminate()) {
        return maximizingPlayer ? b.evaluate() : -b.evaluate();
    }
    if (maximizingPlayer) {
        int v = INT_MIN;
        auto points = b.get_playable_chess();
        for (auto &&point:points) {
            auto next_board = b;
            next_board.play_chess(point.x, point.y);
            v = max(v, alpha_beta(next_board, false, depth - 1, alpha, beta));
            alpha = max(alpha, v);
            if (beta <= alpha) {
                break;
            }
        }
        return v;
    } else {
        int v = INT_MAX;
        auto points = b.get_playable_chess();
        for (auto &&point:points) {
            auto next_board = b;
            next_board.play_chess(point.x, point.y);
            v = min(v, alpha_beta(next_board, true, depth - 1, alpha, beta));
            beta = min(beta, v);
            if (beta <= alpha) {
                break;
            }
        }
        return v;
    }
}

void human_play(Board &b) {
    cout << "Please input x(0-7) and y(0-1):\n";

}

void random_play(Board &b, unsigned long seed = 0) {
    if (seed == 0) {
        random_device r;
        seed = r();
    }
    default_random_engine el(seed);
    auto points = b.get_playable_chess();
    uniform_int_distribution<int> uniform_dist(0, static_cast<int>(points.size() - 1));
    auto position = uniform_dist(el);
    auto &&point = points[position];
    if (DEBUG) {
        cout << "Random Player Set (" << point.x << ", " << point.y << ")\n";
    }
    b.play_chess(point.x, point.y);
}

void ai_play(Board &b, bool maximizingPlayer = true, int depth = 3) {
    auto points = b.get_playable_chess();
    Point best_point;
    int best_score = maximizingPlayer ? INT_MIN : INT_MAX;
    for (auto &&point:points) {
        auto next_board = b;
        next_board.play_chess(point.x, point.y);
        auto current_score = alpha_beta(next_board, maximizingPlayer, depth);
        if (maximizingPlayer) {
            if (current_score > best_score) {
                best_score = current_score;
                best_point = point;
            }
        } else {
            if (current_score < best_score) {
                best_score = current_score;
                best_point = point;
            }
        }
    }
    if (DEBUG) {
        cout << "AI Player Set (" << best_point.x << ", " << best_point.y << ")\n";
    }
    b.play_chess(best_point.x, best_point.y);
}

int main(int argc, char *argv[]) {
    /*Point p{};
    p.x = 1;
    p.y = 2;
    cout << p.xy << endl;
    vector<uint16_t> black_vector(N2), white_vector(N2);
    cout << black_vector.capacity() << endl << white_vector.capacity() << endl;
    Board b;
    cout << b;
    auto black_points = b.get_points(BLACK);
    for (auto &&point:black_points) {
        cout << point << endl;
    }
    cout << "DEBUG" << endl;
    b.play_chess(3, 2);
    cout << b;
    b.play_chess(2, 2);
    cout << b;
    b.play_chess(2, 3);
    cout << b;
    b.play_chess(2, 4);
    cout << b;*/
    Board b;
    cout << b.as_string();
    int count = 0;
    bool who_play = true;
    unsigned int seed = 0;
    while (!b.terminate()) {
        if (who_play) {
            ai_play(b, true, 4);
        } else {
            random_play(b, seed);
        }
        cout << b;
        count++;
        who_play = !who_play;
        cout << "Count: " << count << endl;
    }
    cout << (b.who_win() == BLANK ? "Equal.\n" : b.who_win() == BLACK ? "Black Win.\n" : "White Win.\n");
    /*DEBUG = false;
    for (int depth = 1; depth < 7; depth++) {
        int black_win = 0, white_win = 0, equal = 0;
        for (int i = 0; i < 10000; i++) {
            Board b;
            bool who_play = true;
            while (!b.terminate()) {
                if (who_play) {
                    ai_play(b, true, depth);
                } else {
                    random_play(b, 0);
                }
                who_play = !who_play;
            }
            //cout << b;
            auto c = b.who_win();
            if (c == BLANK) {
                equal++;
            } else if (c == BLACK) {
                black_win++;
            } else {
                white_win++;
            }
        }
        cout << "====================\n";
        cout << "Depth: " << depth << endl;
        cout << "Black win: " << black_win << endl;
        cout << "White win: " << white_win << endl;
        cout << "Equal: " << equal << endl;
        cout << "====================\n";
    }*/
}