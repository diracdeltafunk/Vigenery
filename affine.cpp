#include "affine.h"

#include <iostream>
#include <sstream>

crypto::AffineCipher::AffineCipher(const std::vector<char>& alphabet, const int& a, const int& b) : alphabet(alphabet), a(a), b(b) {
  std::ostringstream s;
  s << "Affine Cipher, a = " << a << ", b = " << b;
  metaInfo = s.str();
}

int crypto::AffineCipher::getMultiplicand() const {
  return a;
}

int crypto::AffineCipher::getSummand() const {
  return b;
}

std::string crypto::AffineCipher::encrypt(const std::string& message) const {
  std::string result;
  std::string formatted = filterIn(message, alphabet);
  int index;
  for (const auto& i : formatted) {
    index = std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), i));
    result.push_back(alphabet[(a*index + b) % alphabet.size()]);
  }
  return result;
}

std::string crypto::AffineCipher::decrypt(const std::string& message) const {
  int inv;
  try {
    inv = modular_inverse(a, alphabet.size());
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return "";
  }
  std::string result;
  std::string formatted = filterIn(message, alphabet);
  int index;
  for (const auto& i : formatted) {
    index = std::distance(alphabet.begin(), std::find(alphabet.begin(), alphabet.end(), i));
    result.push_back(alphabet[(inv * (index - b)) % alphabet.size()]);
  }
  return result;
}

std::vector<crypto::AffineCipher> crypto::AffineCipher::bestGuesses (const std::string& message, const std::vector<char>& alphabet) {
  auto a = crypto::AffineCipher::all(alphabet);
  std::sort(a.begin(), a.end(), [&message](const crypto::AffineCipher& c1, const crypto::AffineCipher& c2) {
    return crypto::frequencyDistance(crypto::frequencyAnalysis(c1.decrypt(message)), crypto::averageEnglishFrequency) < crypto::frequencyDistance(crypto::frequencyAnalysis(c2.decrypt(message)), crypto::averageEnglishFrequency);
  });
  return a;
}

std::vector<crypto::AffineCipher> crypto::AffineCipher::all (const std::vector<char>& alphabet) {
  std::vector<crypto::AffineCipher> result;
  for (int i = 0; i < alphabet.size(); ++i) {
    if (gcd(i, alphabet.size()) == 1) {
      for (int j = 0; j < alphabet.size(); ++j)
        result.push_back(crypto::AffineCipher(alphabet, i, j));
    }
  }
  return result;
}
