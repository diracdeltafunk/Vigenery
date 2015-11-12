#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <map>
#include <numeric>

int gcd (int x, int y);

//A vector of the lowercase characters
const auto upperRomanAlphabet = ([](){
  std::vector<char> result;
  for (auto i = 'A'; i <= 'Z'; i++)
    result.push_back(i);
  return result;
})();

//A vector of the uppercase characters
const auto lowerRomanAlphabet = ([](){
  std::vector<char> result;
  for (auto i = 'a'; i <= 'z'; i++)
    result.push_back(i);
  return result;
})();

namespace crypto {
  //We store our probabilities as doubles
  typedef double prob_t;
  template <typename L>
  std::map<L,prob_t> frequencyAnalysis(const std::vector<L>& message);
  //Ciphers are pairing of an encryption function and its inverse
  //L is the letter type (e.g. char)
  //M is the messsage type (e.g. string)
  template <typename L, typename M>
  class Cipher {
  public:
    Cipher(const std::vector<L>& alphabet);
    virtual M encrypt(const M& message) const = 0;
    virtual M decrypt(const M& message) const = 0;
  protected:
    const std::vector<L> alphabet;
  };
  //A CryptoSystem is a collection of ciphers, indexed to keys
  //L is the letter type (e.g. char)
  //K is the key type (e.g. int)
  //M is the message type (e.g. string)
  template <typename K, typename L, typename M>
  class CryptoSystem {
  public:
    CryptoSystem(const std::vector<L>& alphabet);
    virtual Cipher<L,M> cipher(const K& key) const = 0;
  protected:
    const std::vector<L> alphabet;
  };
}

#endif
