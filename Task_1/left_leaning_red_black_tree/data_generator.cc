#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

const int optNum = 1000000;
const int P = 1000;

std::vector<int> v;

void insert() {
  puts("1");
  int p = rand() % 100 + 1;
  if (p <= 80 || v.empty()) {
    int x = rand() % P + 1;
    if (std::find(v.begin(), v.end(), x) == v.end()) {
      v.push_back(x);
    }
    printf("%d\n", x);
  } else {
    int x = rand() % (v.size());
    printf("%d\n", v[x]);
  }
}

void erase() {
  puts("2");
  int p = rand() % 100 + 1;
  if (p <= 20 || v.empty()) {
    int x = rand() % P + 1;
    if (std::find(v.begin(), v.end(), x) != v.end()) {
      auto it = std::remove(v.begin(), v.end(), x);
      v.erase(it, v.end());
    }
    printf("%d\n", x);
  } else {
    int x = rand() % v.size();
    printf("%d\n", v[x]);
    v.erase(v.begin() + x);
  }
}

void count() {
  puts("3");
  int p = rand() % 100 + 1;
  if (p <= 20 || v.empty()) {
    int x = rand() % P + 1;
    printf("%d\n", x);
  } else {
    int x = rand() % v.size();
    printf("%d\n", v[x]);
  }
}

int main() {
  srand(time(0));
  for (int i = 1; i <= optNum; ++i) {
    int p = rand() % 100 + 1, opt;
    if (p <= 70) {
      insert();
    } else if (p <= 80) {
      erase();
    } else if (p <= 90) {
      count();
    } else {
      opt = rand() % 3 + 4;
      printf("%d\n", opt);
    }
  }
  printf("7\n");
  return 0;
}