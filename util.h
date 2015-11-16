#ifndef UTIL_H
#define UTIL_H

#include <map>
#include <algorithm>

//Custom exception class for error handling
class NIException : public std::exception {
  const char* what() const throw () {
    return "Error: no modular inverse exists!";
  }
};

//Calculates the inverse of a in Z/n, or throws an NIException if it doesn't exist
//e.g. modular_inverse(11,120) returns 11
int modular_inverse(int a, int n);

//Calculates the GCD of two integers
//e.g. gcd(14,7) returns 2
int gcd(int x, int y);

//Calculates the larger of the two arguments
template <typename T>
T max(const T& a, const T& b);

//Calcuates the average of a vector
template <typename L>
L average(const std::vector<L>& v);

//Returns a vector of all vectors whose length is the second parameter and whose elements are in the first parameter
template <typename L>
std::vector<std::vector<L>> allVectors(const std::vector<L>& from, const unsigned int& length);

//Retuns the first argument with all characters not in the second removed
//e.g. filterIn("apples or peas", " pqr") returns "alesoeas"
template <typename L>
std::vector<L> filterIn(const std::vector<L>& a, const std::vector<L>& b);
std::vector<char> filterIn(const std::vector<char>& a, const std::string& b);
std::string filterIn(const std::string& a, const std::vector<char>& b);
std::string filterIn(const std::string& a, const std::string& b);

//Splits the argument into blocks of equal length and returns vector of groupings of the first, second, etc. characters in the blocks
template <typename L>
std::vector<std::vector<L>> deinterleave (const int& blockSize, const std::vector<L>& message);
std::vector<std::string> deinterleave (const int& blockSize, const std::vector<char>& message);
std::vector<std::string> deinterleave (const int& blockSize, const std::string& message);

//Converts the argument to lowercase
std::string tolower(std::string str);
std::vector<char> tolower(std::vector<char> v);

//A vector of the lowercase roman characters
const auto lowerRomanAlphabet = ([](){
  std::vector<char> result;
  for (auto i = 'a'; i <= 'z'; i++)
    result.push_back(i);
  return result;
})();

namespace crypto {
  //We store our probabilities as doubles
  typedef double prob_t;

  //Returns the frequency of each letter in the message
  template <typename L>
  std::map<L,prob_t> frequencyAnalysis(const std::vector<L>& message);
  std::map<char,prob_t> frequencyAnalysis(const std::string& message);

  //The frequency of roman letters (case-insensitive) in random english text
  const auto averageEnglishFrequency = ([](){
    std::map<char,prob_t> result;
    result['a'] = 0.08167;
    result['b'] = 0.01492;
    result['c'] = 0.02782;
    result['d'] = 0.12702;
    result['f'] = 0.02228;
    result['g'] = 0.02015;
    result['h'] = 0.06094;
    result['i'] = 0.06966;
    result['j'] = 0.00153;
    result['k'] = 0.00772;
    result['l'] = 0.04025;
    result['m'] = 0.02406;
    result['n'] = 0.06749;
    result['o'] = 0.07507;
    result['p'] = 0.01929;
    result['q'] = 0.00095;
    result['r'] = 0.05987;
    result['s'] = 0.06327;
    result['t'] = 0.09056;
    result['u'] = 0.02758;
    result['v'] = 0.00978;
    result['w'] = 0.02361;
    result['x'] = 0.00150;
    result['y'] = 0.01974;
    result['z'] = 0.00074;
    return result;
  })();

  //Returns the square of the distance between the intersection of the two frequency distributions, as vectors under the euclidean norm
  template <typename L>
  prob_t frequencyDistance(const std::map<L,prob_t>& a, const std::map<L,prob_t>& b);

  //Returns the probability that two letters from the sampled text are the same
  template <typename L>
  prob_t sameLetterProbability(const std::map<L,prob_t>& frequencies);

  //The probability that two letters in random english text are the same
  const prob_t averageEnglishSLP = sameLetterProbability(averageEnglishFrequency);

  //Ciphers are pairing of an encryption function and its inverse
  //M is the type of the message it can encrypt (e.g. string)
  template <typename M>
  class Cipher {
  public:
    //Encrypts a message
    virtual M encrypt(const M& message) const = 0;
    //Decrypts a message
    virtual M decrypt(const M& message) const = 0;
    //Gives a string of information about the cipher
    std::string getMetaInfo() const;
  protected:
    std::string metaInfo;
  };
}

void here();

void here(int i);

#endif //UTIL_H
