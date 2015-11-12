#include "util.h"

int gcd (int x, int y) {
  return (y == 0) ? x : gcd(y, x % y);
}

template <typename L>
std::map<L,crypto::prob_t> crypto::frequencyAnalysis(const std::vector<L>& message) {
  std::map<L,crypto::prob_t> result;
  for (auto i = message.begin(); i != message.end(); ++i)
    result[*i]++;
  for (auto i = result.begin(); i != result.end(); ++i)
    i->second /= message.size();
  return result;
}

template <typename L, typename M>
crypto::Cipher<L,M>::Cipher(const std::vector<L>& a) : alphabet(a) {}

template <typename K, typename L, typename M>
crypto::CryptoSystem<K,L,M>::CryptoSystem(const std::vector<L>& a) : alphabet(a) {}

crypto::CryptoSystem<std::pair<int,int>, char, std::string> affine (lowerRomanAlphabet);
