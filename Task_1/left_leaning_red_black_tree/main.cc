#include <cstdio>

#include "../include/llrb_set_advance.h"

Set<int> s;

int main() {
  while (true) {
    int opt, x;
    scanf("%d", &opt);
    if (opt == 1) {  // insert
      scanf("%d", &x);
      s.insert(x);
    } else if (opt == 2) {  // erase
      scanf("%d", &x);
      s.erase(x);
    } else if (opt == 3) {  // count
      scanf("%d", &x);
      printf("%d\n", s.count(x));
    } else if (opt == 4) {  // size
      printf("%d\n", s.size());
    } else if (opt == 5) {  // empty
      printf("%d\n", s.empty());
    } else if (opt == 6) {  // clear
      s.clear();
    } else if (opt == 7) {
      break;
    }
  }
  return 0;
}