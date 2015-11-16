#ifndef AFFINE_H
#define AFFINE_H

#include "util.h"

namespace crypto {
  class AffineCipher : public Cipher<std::string> {
  public:
    //Constructor
    AffineCipher(const std::vector<char>& alphabet, const int& a, const int& b);
    //Retuns the number we multiply by
    int getMultiplicand() const;
    //Retuns the number we add
    int getSummand() const;
    //Encrypts a string
    std::string encrypt(const std::string& message) const;
    //Decrypts a string
    std::string decrypt(const std::string& message) const;
    //Returns a vector of all possible valid affine ciphers, ordered by frequency-similarity to random english
    static std::vector<AffineCipher> bestGuesses (const std::string& message, const std::vector<char>& alphabet);
  private:
    std::vector<char> alphabet;
    int a;
    int b;
    //Returns a vector of all possible valid affine ciphers on a given alphabet
    static std::vector<AffineCipher> all (const std::vector<char>& alphabet);
  };
}

#endif //AFFINE_H
