#include "util.h"

#include <iostream>

int modular_inverse(int a, int n) {
  if (gcd(a,n) != 1)
    throw NIException();
  if (n == 1)
    return 1;
  int n0 = n, y = 1;
  for (int t, q, x = 0; a > 1; y = t) {
    q = a / n;
    t = n;
    n = a % n;
    a = t;
    t = x;
    x = y - q*x;
  }
  if (y < 0)
    y += n0;
  return y;
}

int gcd (int x, int y) {
  return (y == 0) ? x : gcd(y, x % y);
}

template <typename L>
std::vector<std::vector<L>> allVectors(const std::vector<L>& from, const unsigned int& length) {
  if (length == 0)
    return std::vector<std::vector<L>>();
  std::vector<std::vector<L>> result;
  if (length == 1) {
    for (const auto& i : from)
      result.push_back(std::vector<L>(1,i));
    return result;
  }
  auto prev = allVectors(from, length-1);
  for (auto& i : prev) {
    for (auto j : from) {
      i.push_back(j);
      result.push_back(i);
      i.pop_back();
    }
  }
  return result;
}

template std::vector<std::vector<unsigned int>> allVectors(const std::vector<unsigned int>& from, const unsigned int& length);

template <typename L>
std::vector<L> filterIn (const std::vector<L>& a, const std::vector<L>& b) {
  std::vector<L> result;
  for (const auto& i : a) {
    if (std::find(b.begin(), b.end(), i) != b.end())
      result.push_back(i);
  }
  return result;
}

std::vector<char> filterIn (const std::vector<char>& a, const std::string& b) {
  std::vector<char> result;
  for (const auto& i : a) {
    if (std::find(b.begin(), b.end(), i) != b.end())
      result.push_back(i);
  }
  return result;
}

std::string filterIn (const std::string& a, const std::vector<char>& b) {
  std::string result;
  for (const auto& i : a) {
    if (std::find(b.begin(), b.end(), i) != b.end())
      result.push_back(i);
  }
  return result;
}

std::string filterIn (const std::string& a, const std::string& b) {
  std::string result;
  for (const auto& i : a) {
    if (std::find(b.begin(), b.end(), i) != b.end())
      result.push_back(i);
  }
  return result;
}

template <typename L>
std::vector<std::vector<L>> deinterleave (const int& blockSize, const std::vector<L>& message) {
  std::vector<std::vector<L>> result (blockSize, std::vector<L>());
  int blockFill = 0;
  for (const auto& i : message) {
    result[blockFill].push_back(i);
    blockFill = (blockFill + 1) % blockSize;
  }
  return result;
}

std::vector<std::string> deinterleave (const int& blockSize, const std::vector<char>& message) {
  std::vector<std::string> result (blockSize, "");
  int blockFill = 0;
  for (const auto& i : message) {
    result[blockFill].push_back(i);
    blockFill = (blockFill + 1) % blockSize;
  }
  return result;
}

std::vector<std::string> deinterleave (const int& blockSize, const std::string& message) {
  std::vector<std::string> result (blockSize, "");
  int blockFill = 0;
  for (const auto& i : message) {
    result[blockFill].push_back(i);
    blockFill = (blockFill + 1) % blockSize;
  }
  return result;
}

std::string tolower(std::string str) {
  for (auto& i : str)
    i = tolower(i);
  return str;
}

std::vector<char> tolower(std::vector<char> v) {
  for (auto& i : v)
    i = tolower(i);
  return v;
}

template <typename L>
std::map<L,crypto::prob_t> crypto::frequencyAnalysis(const std::vector<L>& message) {
  std::map<L,crypto::prob_t> result;
  for (const auto& i : message)
    result[i]++;
  for (auto& i : result)
    i.second /= message.size();
  return result;
}

std::map<char,crypto::prob_t> crypto::frequencyAnalysis(const std::string& message) {
  std::map<char,crypto::prob_t> result;
  for (const auto& i : message)
    result[i]++;
  for (auto& i : result)
    i.second /= message.size();
  return result;
}

template <typename L>
crypto::prob_t crypto::frequencyDistance(const std::map<L,crypto::prob_t>& a, const std::map<L,crypto::prob_t>& b) {
  crypto::prob_t result = 0;
  typename std::map<L,crypto::prob_t>::const_iterator j;
  for (const auto& i : a) {
    j = b.find(i.first);
    if (j != b.end())
      result += (i.second - j->second) * (i.second - j->second);
  }
  return result;
}

template crypto::prob_t crypto::frequencyDistance<char>(const std::map<char,crypto::prob_t>& a, const std::map<char,crypto::prob_t>& b);

template <typename L>
crypto::prob_t crypto::sameLetterProbability(const std::map<L,prob_t>& frequencies) {
  crypto::prob_t result = 0;
  for (const auto& i : frequencies)
    result += i.second * i.second;
  return result;
}

template crypto::prob_t crypto::sameLetterProbability(const std::map<char,prob_t>& frequencies);

template <typename M>
std::string crypto::Cipher<M>::getMetaInfo() const {
  return metaInfo;
}

template class crypto::Cipher<std::string>;

void here() {
  std::cout << "HERE" << std::endl;
}

void here(int i) {
  std::cout << "HERE " << i << std::endl;
}
