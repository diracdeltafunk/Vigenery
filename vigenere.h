#ifndef VIGENERE_H
#define VIGENERE_H

#include "util.h"

namespace crypto {
  class VigenereCipher : public Cipher<std::string> {
  public:
    //Constructor
    VigenereCipher(const std::vector<char>& alphabet, const std::string& key);
    //Returns the key
    std::string getKey() const;
    //Encrypts a string
    std::string encrypt(const std::string& message) const;
    //Decrypts a string
    std::string decrypt(const std::string& message) const;
    //Return a vector of good guesses for Vigenere ciphers to decrypt the argument
    static std::vector<VigenereCipher> bestGuesses(const std::string& message, const std::vector<char>& alphabet);
  private:
    std::vector<char> alphabet;
    std::string key;
    //Return a vector of possible key lengths for the argument, ordered by same-letter similarlity to english
    static std::vector<unsigned int> keyLengthGuesses(const std::string& message);
  };
}

#endif //VIGENERE_H
