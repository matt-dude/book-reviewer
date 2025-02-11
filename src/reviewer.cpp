#include <iostream>
#include <chrono>
#include <memory>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <fstream>

using std::cerr;
using std::cout;
using std::default_random_engine;
using std::endl;
using std::ifstream;
using std::ios;
using std::string;
using std::stringstream;
using std::uniform_int_distribution;
using std::unique_ptr;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;

vector<string> grabLines(string input) {
  vector<string> result;
  string temp;
  for (int i = 0; i < input.length(); i ++) {
    char c = input[i];
    if (c == '\n' || c == '\r') {
      if (temp.length() > 0) {
        result.push_back(temp);
        temp = string();
      }
    }else {
      temp += c;
    }
  }
  if (temp.length() > 0) {
    result.push_back(temp);
  }
  return result;
   }

vector<string> grabLines(ifstream& input) {
  input.seekg(0, ios::end);
  int fileSize = input.tellg();
  input.seekg(0, ios::beg);
  unique_ptr<char> data(new char[fileSize + 1]);
  input.read(data.get(), fileSize);
  data.get()[fileSize] = '\0';
  return grabLines(string(data.get()));  
}

vector<string> grabWords(string input) {
    vector<string> result;
    string temp;
    for (int i = 0; i < input.length(); i ++) {
      char c = input[i];
      if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
          if (temp.length() > 0) {
            result.push_back(temp);
            temp = string();
          }
      }else {
            temp += c;
          }
    } 
    if (temp.length() > 0) {
      result.push_back(temp);
    }
    return result;
}

vector<string> grabWords(ifstream& input) {
   input.seekg(0, ios::end);
  int fileSize = input.tellg();
  input.seekg(0, ios::beg);
  unique_ptr<char> data(new char[fileSize + 1]);
  input.read(data.get(), fileSize);
  data.get()[fileSize] = '\0';
  return grabWords(string(data.get()));
 }
int main() { 
  ifstream namesStream("names.txt");
  if(!namesStream) { 
    cerr << "Error while opening file handle" << endl;
    return -1;
  }
  vector<string> names = grabWords(namesStream);
  namesStream.close();
  ifstream nounsStream("nouns.txt");
  vector<string> nouns = grabWords(nounsStream);
  nounsStream.close();
  default_random_engine generator;
  generator.seed(
      duration_cast<nanoseconds>(
          high_resolution_clock::now().time_since_epoch())
          .count());
  uniform_int_distribution<int> nameDistribution(0,names.size() - 1);
  string firstName = names[nameDistribution(generator)];
  string lastName = names[nameDistribution(generator)];
  uniform_int_distribution<int> nounDistribution(0, nouns.size() - 1);
  string noun1 = nouns[nounDistribution(generator)];
  string noun2 = nouns[nounDistribution(generator)];
  cout << firstName << " " << lastName << " and the " << noun1 << " of " << noun2 << endl;
  int stars = 5 - nounDistribution(generator) % 5;
  cout << "Rated " << stars << " stars" << endl;
  stringstream reviewFileName;
  reviewFileName << stars << "stars.txt";
  ifstream reviewInput(reviewFileName.str());
  vector<string> reviewTerms = grabLines(reviewInput);
  reviewInput.close();
  uniform_int_distribution<int> reviewTermDistribution(0, reviewTerms.size() - 1);
  string term1 = reviewTerms[reviewTermDistribution(generator)];
  string term2;
  do {
     term2 = reviewTerms[reviewTermDistribution(generator)];
  } while (term1 == term2);
  cout << term1 << " " << term2 << endl;
  return 0;
}
