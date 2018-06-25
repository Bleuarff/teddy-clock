#include <iostream>

struct Time {
  int h;
  int m;
  int s;
};

Time getTime(){
  Time t;
  std::cout << t.h;
  t.h = 12;
  t.m = 5;
  t.s = 55;

  return t;
}

int main(){
  std::cout << "Hello world\n";

  Time t = getTime();

  std::cout << t.h;
  std::cout << ":";
  std::cout << t.m;
  std::cout << ":";
  std::cout << t.s;
  std::cout << "\n";
  return 0;
}
