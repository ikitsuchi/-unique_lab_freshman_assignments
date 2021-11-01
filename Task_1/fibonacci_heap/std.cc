#include <bits/stdc++.h>

std::priority_queue<int> q;

int main() {
  while (true) {
    int opt, x;
    scanf("%d", &opt);
    if (opt == 1) {  // push
      scanf("%d", &x);
      q.push(x);
    } else if (opt == 2) {  // pop
      q.pop();
    } else if (opt == 3) {  // top
      printf("%d\n", q.top());
    } else if (opt == 4) {  // size
      printf("%d\n", q.size());
    } else if (opt == 5) {  // empty
      printf("%d\n", q.empty());
    } else if (opt == 6) {
      break;
    }
  }
  return 0;
}