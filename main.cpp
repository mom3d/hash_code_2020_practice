#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
class Result {
 public:
  int avail_;
  std::vector<int> result_;
};
class Order {
 public:
  void fillFromFile(std::string file_name) {
    std::string line;
    std::ifstream file_input(file_name);

    if (file_input.is_open()) {
      std::getline(file_input, line);
      std::stringstream first_ss(line);
      std::string m_token, n_token;
      std::getline(first_ss, m_token, ' ');
      std::getline(first_ss, n_token, ' ');
      m_ = std::stoi(m_token);
      n_ = std::stoi(n_token);

      while (std::getline(file_input, line)) {
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ' ')) {
          slices_.emplace_back(std::stoi(token));
        }
      }
    }
  }
  void greedy() {
    int size = slices_.size() - 1;
    int max = m_;

    Result res;
    res.avail_ = 0;
    for (int i = size; i >= 0; --i) {
      if ((max - slices_[i]) >= 0) {
        max -= slices_[i];
        res.result_.emplace_back(i);
        res.avail_ += slices_[i];
      }
    }
    results = res;
  }
  void testFast() { results = recurcive(slices_, m_); }
  Result recurcive(std::vector<int> slices, int avail) {
    Result res;
    static std::map<std::vector<int>, Result> memo;
    if (memo.find({(int)slices.size(), avail}) != memo.end()) {
      res = memo[{(int)slices.size(), avail}];
    } else if ((slices.size() == 0) || (avail == 0)) {
      res.avail_ = 0;
      res.result_ = {};

    } else if (slices[0] > avail) {
      slices.erase(slices.begin());
      res = recurcive(slices, avail);
    } else {
      int nextItem = slices[0];
      std::vector<int> copy1{slices.begin() + 1, slices.end()};
      std::vector<int> copy2{slices.begin() + 1, slices.end()};

      auto toTake = recurcive(copy1, avail - nextItem);
      int withVal = toTake.avail_;
      std::vector<int> withToTake = toTake.result_;
      withVal += nextItem;

      auto notToTake = recurcive(copy2, avail);
      int withoutVal = notToTake.avail_;
      std::vector<int> withoutToTake = notToTake.result_;

      if (withVal > withoutVal) {
        withToTake.emplace_back(nextItem);
        res.avail_ = withVal;
        res.result_ = withToTake;

      } else {
        res.avail_ = withoutVal;
        res.result_ = withoutToTake;
      }
      memo[{(int)slices.size(), avail}] = res;
    }
    return res;
  }

  int m_;
  int n_;

  std::vector<int> slices_;
  Result results;
};

int main(int argc, char** argv) {
  Order o;

  o.fillFromFile(argv[1]);

  o.testFast();
  std::cout << o.results.avail_ << std::endl;

  for (auto x : o.results.result_) {
    std::cout << x << std::endl;
  }
  return 0;
}
