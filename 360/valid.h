#include <iostream>
using namespace std;

// getInteger() prompts for and returns any valid integer
// pre: none
// post: a valid integer is returned
int getInteger(string prompt = "Enter integer: ")
{
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
   cin.get();
   return num;
}


// getFloat() prompts for and returns any valid float
// pre: none
// post: a valid float is returned
float getFloat(string prompt = "Enter number: ")
{
   float num = 0;
   cout << prompt;
   cin >> num;
   while (!cin || cin.peek() != '\n') {
      cin.clear();
      cin.ignore(100, '\n');
      cout << "Invalid number." << endl;
      cout << prompt;
      cin >> num;
   }
   cin.get();
   return num;
}


// getDouble() prompts for and returns any valid double
// pre: none
// post: a valid double is returned
double getDouble(string prompt = "Enter number: ")
{
   double num = 0;
   cout << prompt;
   cin >> num;
   while (!cin || cin.peek() != '\n') {
      cin.clear();
      cin.ignore(100, '\n');
      cout << "Invalid number." << endl;
      cout << prompt;
      cin >> num;
   }
   cin.get();
   return num;
}


// getChar() prompts for and returns any single character
// pre: none
// post: a single character is returned
char getChar(string prompt = "Enter char: ")
{
   char ch = 0;
   cout << prompt;
   cin >> ch;
   while (!cin || cin.peek() != '\n') {
      cin.clear();
      cin.ignore(100, '\n');
      cout << "Invalid char." << endl;
      cout << prompt;
      cin >> ch;
   }
   cin.get();
   return ch;
}


// getString() prompts for and returns a non-empty string
// pre: none
// post: a non-empty string is returned
string getString(string prompt = "Enter string: ")
{
   string st = "";
   cout << prompt;
   getline(cin, st);
   while (st == "") {
      cout << "No characters entered." << endl;
      cout << prompt;
      getline(cin, st);
   }
   return st;
}
