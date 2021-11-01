#include <bits/stdc++.h>

const int optNum = 100000;
const int P = 10000;

std::priority_queue<int> q;

void push() {
  puts("1");
  int x = rand() % P + 1;
  printf("%d\n", x);
  q.push(x);
}

void pop() {
  if (q.empty()) {
    push();
    return;
  }
  puts("2");
  q.pop();
}

int main() {
  srand(time(0));
  for (int i = 1; i <= optNum; ++i) {
    int p = rand() % 100 + 1, opt;
    if (p <= 40) {
      push();
    } else if (p <= 60) {
      pop();
    } else {
      opt = rand() % 3 + 3;
      if (opt == 3 && q.empty()) {
        --i;
        continue;
      }
      printf("%d\n", opt);
    }
  }
  printf("6\n");
  return 0;
}