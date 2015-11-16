#include "affine.h"
#include "vigenere.h"

#include <iostream>
#include <thread>

int main(int argc, char** argv) {
  std::string message;
  for (char c; std::cin.get(c); message.push_back(c));
  message.pop_back();
  std::vector<crypto::Cipher<std::string>*> affineP;
  std::vector<crypto::Cipher<std::string>*> vigenereP;
  std::thread t1 ([](std::vector<crypto::Cipher<std::string>*>& v, const std::string& m) {
    auto guesses = crypto::AffineCipher::bestGuesses(m, lowerRomanAlphabet);
    for (const auto& i : guesses)
      v.push_back((crypto::Cipher<std::string>*) new crypto::AffineCipher(i));
  }, std::ref(affineP), std::ref(message));
  std::thread t2 ([](std::vector<crypto::Cipher<std::string>*>& v, const std::string& m) {
    auto guesses = crypto::VigenereCipher::bestGuesses(m, lowerRomanAlphabet);
    for (const auto& i : guesses)
      v.push_back((crypto::Cipher<std::string>*) new crypto::VigenereCipher(i));
  }, std::ref(vigenereP), std::ref(message));
  std::vector<crypto::Cipher<std::string>*> combined;
  t1.join();
  t2.join();
  auto i1 = affineP.begin();
  auto i2 = vigenereP.begin();
  while (i1 != affineP.end() || i2 != vigenereP.end()) {
    if (i1 == affineP.end())
      combined.push_back(*(i2++));
    else if (i2 == vigenereP.end())
      combined.push_back(*(i1++));
    else if (crypto::frequencyDistance(crypto::frequencyAnalysis((*i1)->decrypt(tolower(message))), crypto::averageEnglishFrequency) < crypto::frequencyDistance(crypto::frequencyAnalysis((*i2)->decrypt(tolower(message))), crypto::averageEnglishFrequency))
      combined.push_back(*(i1++));
    else
      combined.push_back(*(i2++));
  }
  std::cout << "Results:" << std::endl << std::endl;
  for (unsigned int pick = 0; pick < combined.size(); ++pick) {
    std::cout << "#" << pick + 1 << " match: " << combined[pick]->getMetaInfo() << std::endl;
    std::cout << combined[pick]->decrypt(tolower(message)) << std::endl << std::endl;
  }
  std::cout << "Complete." << std::endl;
  return 0;
}
