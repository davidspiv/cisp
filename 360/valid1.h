#include <fstream>
#include <iostream>
using namespace std;

// validates for integer input
int getInteger(string prompt = "Enter integer: ") {
  int num = 0;
  cout << prompt;
  cin >> num;
  while (!cin || cin.peek() != '\n') {
    cin.clear();
    cin.ignore(100, '\n');
    cout << "Invalid integer." << endl;
    cout << prompt;
    cin >> num;
  }
  return num;
}

// validates for float input
float getFloat(string prompt = "Enter float: ") {
  float num = 0;
  cout << prompt;
  cin >> num;
  while (!cin) {
    cin.clear();
    cin.ignore(100, '\n');
    cout << "Invalid float." << endl;
    cout << prompt;
    cin >> num;
  }
  return num;
}

// open a file for input
bool openFile(ifstream& infile, string fileName) {
  infile.open(fileName);
  if (!infile) {
    return false;
  }
  return true;
}

// open a file for output
bool openFile(ofstream& outfile, string fileName) {
  outfile.open(fileName);
  if (!outfile) {
    return false;
  }
  return true;
}
