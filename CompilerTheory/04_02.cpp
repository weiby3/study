#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum State { Default = 0, Start, End };

struct DeterminedState {
  State st;
  vector<int> st_vector;
};

struct DeterminedMapping {
  int start, end;
  string s;
};

const static string epsilon("k");

string vi2s(const vector<int> &v) {
  stringstream ss;
  for (auto &i : v) {
    ss << i;
  }
  return ss.str();
}

void classification(vector<int> &v) {
  for (int i = 0; i < v.size(); i++) {
    for (int j = i + 1; j < v.size(); j++) {
      if (v[j] == v[i]) {
        v.erase(v.begin() + j);
        j--;
      }
    }
  }
  sort(v.begin(), v.end());
}

bool is_end(const vector<int> &v, const vector<int> &e) {
  for (const auto &est : e) {
    for (const auto &st : v) {
      if (est == st) {
        return true;
      }
    }
  }
  return false;
}

bool cmp(const DeterminedMapping &a, const DeterminedMapping &b) {
  return a.start < b.start ? true : a.start == b.start ? a.end < b.end : false;
}

int main() {
  int ch_count;
  vector<string> ch_vector;
  int st_count;
  map<int, State> st_map;
  vector<int> ends_vector;
  int mp_count;
  map<int, map<string, vector<int>>> mp_map;
  map<int, vector<int>> emp_map;

  cin >> ch_count;
  for (int i = 0; i < ch_count; i++) {
    string tmp;
    cin >> tmp;
    ch_vector.emplace_back(tmp);
  }

  cin >> st_count;
  for (int i = 0; i < st_count; i++) {
    int n, start, end;
    cin >> n >> start >> end;
    st_map[n] = start == 1 ? Start : end == 1 ? End : Default;
    if (end == 1) {
      ends_vector.emplace_back(n);
    }
  }

  cin >> mp_count;
  for (int i = 0; i < mp_count; i++) {
    int start, end;
    string ch;
    cin >> start >> end >> ch;
    mp_map[start][ch].emplace_back(end);
  }
  for (const auto &st : st_map) {
    if (mp_map[st.first].count(epsilon) != 0) {
      vector<int> ev{st.first};
      for (int i = 0; i < ev.size(); i++) {
        auto n = ev[i];
        if (mp_map[n].count(epsilon) != 0) {
          for (const auto &n2 : mp_map[n][epsilon]) {
            ev.emplace_back(n2);
          }
        }
      }
      ev.erase(ev.begin());
      emp_map[st.first] = ev;
    }
  }

  vector<DeterminedState> ds_vector{DeterminedState{Start, vector<int>{0}}};
  set<string> ds_set;
  map<string, int> ds_map;
  vector<DeterminedMapping> dm_vector;
  cout << "Determine State:\n";
  for (int i = 0; i < ds_vector.size(); i++) {
    auto ds = ds_vector[i];
    cout << i << ' ' << (ds.st == Start) << ' ' << (ds.st == End) << " [";
    for (const auto &n : ds.st_vector) {
      cout << n;
    }
    cout << "]\n";

    for (const auto &ch : ch_vector) {
      vector<int> est_vector;
      for (const auto &st : ds.st_vector) {
        const auto &st_vector = mp_map[st][ch];
        for (const auto &ost : st_vector) {
          est_vector.emplace_back(ost);
          if (emp_map.count(ost) != 0) {
            for (const auto &est : emp_map[ost]) {
              est_vector.emplace_back(est);
            }
          }
        }
      }

      if (!est_vector.empty()) {
        classification(est_vector);
        auto est_str = vi2s(est_vector);
        if (ds_set.count(est_str) == 0) {
          ds_set.insert(est_str);
          ds_vector.emplace_back(DeterminedState{
              is_end(est_vector, ends_vector) ? End : Default, est_vector});
          ds_map[est_str] = ds_vector.size() - 1;
          ds = ds_vector[i];
        }
        dm_vector.emplace_back(DeterminedMapping{i, ds_map[est_str], ch});
      }
    }
  }

  cout << "Determine Mapping:\n";
  sort(dm_vector.begin(), dm_vector.end(), cmp);
  for (int i = 0; i < dm_vector.size(); i++) {
    const auto &dm = dm_vector[i];
    cout << dm.start << ' ' << dm.end << ' ' << dm.s << " (" << i << endl;
  }
  return 0;
}