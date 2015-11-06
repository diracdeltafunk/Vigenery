#include <iostream>
#include "LinkedList.h"

using namespace std;

int gcd (int x, int y) {
  return (y == 0) ? x : gcd(y, x % y);
}

int main() {
  cout << "Totient of 26 is " << totient(26) << endl;
  return 0;
}
