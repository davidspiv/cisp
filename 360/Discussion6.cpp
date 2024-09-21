
// define a pointer variable
// indirection operator
// multiplication operator

#include <iostream>
#include <string>

using namespace std;

void print(const string &output, bool carriageReturn = 1)
{
   if (carriageReturn) {
      cout << output << endl;
      return;
   }

   cout << output << flush;
}

void handleError(string &errorMessage)
{
   if (errorMessage.find("Invalid") != string::npos) {
      cin.clear();
      cin.ignore(1000, '\n');
   }

   if (errorMessage.length()) {
      print(errorMessage);
      errorMessage = "";
   }
}

int getNewLineTotal(string input)
{
   int newLineTotal = 0;

   for (size_t i = 0; i < input.length(); i += 1) {
      if (input[i] == '\n') {
         newLineTotal += 1;
      }
   }

   return newLineTotal;
}

// between 1 and maxChoice inclusive
int getChoice(const string &menu)
{
   const int optionsMax = getNewLineTotal(menu);
   string errorMessage = "";
   int menuOption = 0;

   do {
      handleError(errorMessage);

      print(menu);
      print("Choose an option: ", 0);
      cin >> menuOption;

      if (!cin || cin.peek() != '\n') {
         errorMessage = "\nError: Invalid integer";
      }
      else if (menuOption < 1 || menuOption > optionsMax) {
         errorMessage = "\nError: Number not associated with a menu option";
      }

   } while (errorMessage.length());

   return menuOption;
}

char choiceToChar(int choice, string menuChunk)
{
   char choiceAsChar;
   int index = 0;

   while (choice) {
      index = menuChunk.find('\n');
      menuChunk = menuChunk.substr(index + 1);
      choice -= 1;
   }

   choiceAsChar = menuChunk[3];
   return choiceAsChar;
}

void handleSlideShow(const string pages[], int pagesAmount)
{
   const char optionNext = 'N';
   const char optionBack = 'B';
   const char optionMainMenu = 'M';

   const string menuFirst = (R"(
1. Next
2. Main menu)");

   const string menuMiddle = (R"(
1. Next
2. Back
3. Main menu)");

   const string menuLast = (R"(
1. Main menu
2. Back)");

   string menu;
   int pageNumber = 0;
   int choice = 0;
   char choiceAsChar = ' ';

   while (choiceAsChar != optionMainMenu) {
      print(pages[pageNumber]);

      if (pageNumber == 0) {
         menu = menuFirst;
      }
      else if (pageNumber == pagesAmount - 1) {
         menu = menuLast;
      }
      else {
         menu = menuMiddle;
      }

      choice = getChoice(menu);
      choiceAsChar = choiceToChar(choice, menu);

      if (choiceAsChar == optionNext) {
         pageNumber += 1;
      }
      else if (choiceAsChar == optionBack) {
         pageNumber -= 1;
      }
   }
}

size_t handleOption1(string *&ref)
{

   const string menuOptions1 = R"(
A) A pointer is a variable that is used to store another variable's memory
address. The memory address can be located on the stack or on the heap.)";

   const string menuOptions2 = R"(
B) As it is just a virtual address, a pointers memory size is fixed
regardless of what datatype it is referencing.)";

   const string menuOptions3 = R"(
C) A pointer doesn't inherently know what type of data it is referencing,
the programer is telling it via (char *, int *, ect) so that the pointer
can be dereferenced in the future.)";

   ref = new string[3]{menuOptions1, menuOptions2, menuOptions3};

   return 3;
}

int main()
{
   const int option1 = 1;
   const int option2 = 2;
   const int option3 = 3;
   const int optionQuit = 4;

   string *ref;
   size_t arrSize = 0;

   const string mainMenu = R"(
1. Defining a pointer variable
2. As an indirection operator
3. As a multiplication operator
4. Quit)";

   int choice = 0;

   print(R"(
Welcome!
Learn about the three different ways the * operator is used in C++.
Be careful, there is a quiz at the end.)");

   do {
      choice = getChoice(mainMenu);

      switch (choice) {
      case option1:
         arrSize = handleOption1(ref);
         handleSlideShow(ref, arrSize);
         break;

      case option2:
         print("\nIndirection");
         break;

      case option3:
         print("\nMultiplication");
         break;
      }

   } while (choice != optionQuit);

   delete ref;
   ref = nullptr;

   print("\nProgram ended.");
   return 0;
}
