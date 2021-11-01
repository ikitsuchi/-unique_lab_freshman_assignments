#include <bits/stdc++.h>

int main() {
  system("g++ data_generator.cc -o data_generator");
  system("g++ main.cc -o main -g3");
  system("g++ std.cc -o std");
  for (int i = 1; i <= 100; ++i) {
    system("./data_generator > data.in");
    system("./main < data.in > data.out");
    system("./std < data.in > data.ans");
    if (system("diff data.out data.ans")) break;
  }
  return 0;
}