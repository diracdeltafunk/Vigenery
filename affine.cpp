#include <iostream>

using namespace std;

struct node {
   int val;
   node* next;
};

int gcd (int x, int y) {
  if (y == 0)
    return x;
  return gcd(y, x % y);
}

int totient (int x) {
  int result = 0;
  for (int i = 1; i < x; i++) {
    if (gcd(i,x) == 1)
      result++;
  }
  return result;
}

int main() {
   cout << "Totient of 26 is " << totient(26) << endl;
   return 0;
}
