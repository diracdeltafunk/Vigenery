#include "vigenere.h"
#include "affine.h"

#include <cmath>
#include <sstream>

crypto::VigenereCipher::VigenereCipher (const std::vector<char>& alphabet, const std::string& key) : alphabet(alphabet), key(filterIn(key, alphabet)) {
  std::ostringstream s;
  s << "Vigenere Cipher, key = " << key;
  metaInfo = s.str();
}

std::string crypto::VigenereCipher::getKey() const {
  return key;
}

std::string crypto::VigenereCipher::encrypt(const std::string& message) const {
  std::string result, formatted = filterIn(message, alphabet);
  char block [key.size()];
  int index1, index2, blockFill = 0;
  for (auto i = formatted.begin(); i != formatted.end(); ++i, ++blockFill) {
    if (blockFill == key.size()) {
      blockFill = 0;
      for (int i = 0; i < key.size(); ++i) {
        index1 = std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), block[i]));
        index2 = std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), key[i]));
        result.push_back(alphabet[(index1 + index2) % alphabet.size()]);
      }
    }
    block[blockFill] = *i;
  }
  for (int i = 0; i < blockFill; i++) {
    index1 = std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), block[i]));
    index2 = std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), key[i]));
    result.push_back(alphabet[(index1 + index2) % alphabet.size()]);
  }
  return result;
}

std::string crypto::VigenereCipher::decrypt(const std::string& message) const {
  std::string result, formatted = filterIn(message, alphabet);
  char block [key.size()];
  int index1, index2, blockFill = 0;
  for (auto i = formatted.begin(); i != formatted.end(); ++i, ++blockFill) {
    if (blockFill == key.size()) {
      blockFill = 0;
      for (int i = 0; i < key.size(); ++i) {
        index1 = std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), block[i]));
        index2 = std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), key[i]));
        result.push_back(alphabet[(index1 - index2) % alphabet.size()]);
      }
    }
    block[blockFill] = *i;
  }
  for (int i = 0; i < blockFill; i++) {
    index1 = std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), block[i]));
    index2 = std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), key[i]));
    result.push_back(alphabet[(index1 - index2) % alphabet.size()]);
  }
  return result;
}

std::vector<crypto::VigenereCipher> crypto::VigenereCipher::bestGuesses(const std::string& message, const std::vector<char>& alphabet) {
  std::vector<crypto::VigenereCipher> result;
  std::vector<unsigned int> allShifts (alphabet.size());
  std::iota(allShifts.begin(), allShifts.end(), 0);
  auto keyLengths = crypto::VigenereCipher::keyLengthGuesses(message);
  while (keyLengths.size() > 3)
    keyLengths.pop_back();
  const unsigned int maxBadness = 2;
  std::vector<unsigned int> validIndices (maxBadness+1);
  std::iota(validIndices.begin(), validIndices.end(), 0);
  std::vector<std::string> deinterleaved;
  std::vector<std::vector<unsigned int>>* shiftGuesses;
  std::vector<std::vector<unsigned int>> choiceIndices;
  std::string* key;
  for (auto i : keyLengths) {
    deinterleaved = deinterleave(i, message);
    shiftGuesses = new std::vector<std::vector<unsigned int>>;
    for (auto j : deinterleaved) {
      std::sort(allShifts.begin(), allShifts.end(), [&alphabet,&j](const unsigned int& s1, const unsigned int& s2) {
        return crypto::frequencyDistance(crypto::frequencyAnalysis(crypto::AffineCipher(alphabet, 1, s1).decrypt(j)), crypto::averageEnglishFrequency) < crypto::frequencyDistance(crypto::frequencyAnalysis(crypto::AffineCipher(alphabet, 1, s2).decrypt(j)), crypto::averageEnglishFrequency);
      });
      shiftGuesses->push_back(allShifts);
    }
    choiceIndices = allVectors(validIndices, i);
    std::sort(choiceIndices.begin(), choiceIndices.end(), [](const std::vector<unsigned int>& v1, const std::vector<unsigned int>& v2) {
      auto totalBad1 = v1.size() - std::count(v1.begin(), v1.end(), 0);
      auto totalBad2 = v2.size() - std::count(v2.begin(), v2.end(), 0);
      if (totalBad1 != totalBad2)
        return totalBad1 < totalBad2;
      auto totalBadness1 = std::accumulate(v1.begin(), v1.end(), 0);
      auto totalBadness2 = std::accumulate(v2.begin(), v2.end(), 0);
      if (totalBadness1 != totalBadness2)
        return totalBadness1 < totalBadness2;
      auto maxBadness1 = *std::max_element(v1.begin(), v1.end());
      auto maxBadness2 = *std::max_element(v2.begin(), v2.end());
      return maxBadness1 < maxBadness2;
    });
    for (auto j : choiceIndices) {
      key = new std::string;
      for (unsigned int k = 0; k < i; ++k)
        key->push_back(alphabet[(*shiftGuesses)[k][j[k]]]);
      result.push_back(crypto::VigenereCipher(alphabet, *key));
      delete key;
    }
    delete shiftGuesses;
  }
  return result;
}

std::vector<unsigned int> crypto::VigenereCipher::keyLengthGuesses(const std::string& message) {
  std::vector<unsigned int> result (message.size());
  std::iota(result.begin(), result.end(), 1);
  std::sort(result.begin(), result.end(), [&message](const unsigned int& x, const unsigned int& y) {
    auto xd = deinterleave(x, message);
    auto yd = deinterleave(y, message);
    crypto::prob_t xSLP = 0, ySLP = 0;
    for (const auto& i : xd)
      xSLP += crypto::sameLetterProbability(crypto::frequencyAnalysis(i));
    xSLP /= x;
    for (const auto& i : yd)
      ySLP += crypto::sameLetterProbability(crypto::frequencyAnalysis(i));
    ySLP /= y;
    return std::abs(xSLP - averageEnglishSLP) < std::abs(ySLP - averageEnglishSLP);
  });
  return result;
}
