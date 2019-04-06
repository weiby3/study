#include <cctype>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
  //关键词
  vector<string> keywords{"int", "main", "return"};
  //运算符
  vector<string> operators{"+", "*", "=", "+=", "*="};
  //分界符
  vector<string> boundaries{"(", ")", "{", "}", ",", ";"};
  //标识符
  vector<string> identifiers;
  map<string, int> idmap;
  //无符号整数
  vector<string> unsigned_integers;
  map<string, int> uimap;

  bool flag = false;
  while (true) {
    char c, t;
    if (flag) {
      c = t;
      flag = false;
    } else {
      c = cin.get();
    }

    if (c == '#') {
      break;
    } else if (c == '\n' || c == ' ') {
      continue;
      // operators
    } else if (c == '+' || c == '*') {
      t = cin.get();
      if (t == '=') {
        printf("<2,%d>", c == '+' ? 3 : 4);
      } else {
        printf("<2,%d>", c == '+' ? 0 : 1);
        flag = true;
      }
      continue;
    } else if (c == '=') {
      printf("<2,2>");
      continue;
      // unsigned integers
    } else if (isdigit(c)) {
      string s(1, c);
      while (true) {
        t = cin.get();
        if (isdigit(t)) {
          s.append(1, t);
        } else {
          flag = true;
          break;
        }
      }
      if (uimap.count(s) == 0) {
        uimap[s] = unsigned_integers.size();
        printf("<5,%d>", unsigned_integers.size());
        unsigned_integers.emplace_back(s);
      } else {
        printf("<5,%d>", uimap[s]);
      }
      continue;
      // key words or identifiers
    } else if (isalpha(c)) {
      string s(1, c);
      while (true) {
        t = cin.get();
        if (isalnum(t)) {
          s.append(1, t);
        } else {
          flag = true;
          break;
        }
      }
      bool isIdentifier = true;
      for (int i = 0; i < keywords.size(); i++) {
        if (s == keywords[i]) {
          printf("<1,%d>", i);
          isIdentifier = false;
          break;
        }
      }
      if (isIdentifier) {
        if (idmap.count(s) == 0) {
          idmap[s] = identifiers.size();
          printf("<4,%d>", identifiers.size());
          identifiers.emplace_back(s);
        } else {
          printf("<4,%d>", idmap[s]);
        }
      }
      continue;
    }

    // boundaries
    string cs(1, c);
    for (int i = 0; i < boundaries.size(); i++) {
      if (cs == boundaries[i]) {
        printf("<3,%d>", i);
        break;
      }
    }
  }

  printf("\nidentifieres:");
  bool first = true;
  for (const auto &id : identifiers) {
    if (first) {
      printf("%s", id.c_str());
      first = false;
    } else {
      printf(" %s", id.c_str());
    }
  }
  printf("\nUnsigned_integer:");
  first = true;
  for (const auto &ui : unsigned_integers) {
    if (first) {
      printf("%s", ui.c_str());
      first = false;
    } else {
      printf(" %s", ui.c_str());
    }
  }
  printf("\n");
  return 0;
}