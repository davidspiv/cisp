/******************************************************************************
# Author:           David Spivack
# Discussion:       6
# Date:             9/21/24
# Description:      Slideshow that displays different uses for the (*) as well
#                   as general info surrounding pointers.
# Sources:          Discussion 6 specifications
# Comments:         Implementing vector-like behavior without vectors.
#                   Definitely not efficient but shows behavior that would be
#                   impossible without pointers.
#******************************************************************************/

#include <iostream>
#include <string>

using namespace std;

void print(const string &output, bool carriageReturn = 1);
void handleError(string &errorMessage);
int getNewLineTotal(string input);
int getChoice(const string &menu);
char choiceToChar(int choice, string menuString);
size_t createSlideshow(string *&slideShow);
void startSlideShow(const string pages[], int pagesAmount);
void getMoreSlots(string *&slideShow, size_t slideCount);
void addSlide(string *&slideShow, size_t slideCount);

int main()
{
   const int OPTION_START = 1;
   const int OPTION_ADD = 2;
   const int OPTION_QUIT = 3;

   string *slideShow = nullptr;
   size_t slideCount = 0;

   const string mainMenu = R"(
1. Start slideshow
2. Add slide
3. Quit)";

   int choice = 0;

   print(R"(
Welcome!
Learn about the different ways the * operator is used in C++.)");

   slideCount = createSlideshow(slideShow);

   do {
      choice = getChoice(mainMenu);

      switch (choice) {
      case OPTION_START:
         startSlideShow(slideShow, slideCount);
         break;

      case OPTION_ADD:
         addSlide(slideShow, slideCount);
         slideCount += 1;
         break;
      }

   } while (choice != OPTION_QUIT);

   delete[] slideShow;
   slideShow = nullptr;

   print("\nProgram ended.");
   return 0;
}


// print() outputs string to console
// Pre: output is a string, carriageReturn is a bool
// Post: output is displayed and output stream buffer flushed
void print(const string &output, bool carriageReturn)
{
   if (carriageReturn) {
      cout << output << endl;
      return;
   }

   cout << output << flush;
}


// handleError() prints error, if the error is of type "Invalid" then the input
// stream is reset
// Pre: errorMessage as string reference
// Post: error is printed and input stream is reset if needed
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


// getNewLineTotal() gets total of new line characters
// Pre: input is string
// Post: newLineTotal is integer
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


// getChoice() prints menu and returns user input confined to the amount of
// options present in menu
// Pre: menu is string that has the same amount of new Line characters as
// options
// Post: menuOption is integer
int getChoice(const string &menu)
{
   string errorMessage = "";
   int menuOption = 0;
   const int optionsMax = getNewLineTotal(menu);

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

   cin.get();
   return menuOption;
}


// getString() gets valid string input
// Pre: prompt is a string
// Post: non-empty string is returned
string getString(const string &prompt)
{
   string errorMessage = "";
   string input = "";

   do {
      handleError(errorMessage);
      print(prompt, 0);
      getline(cin, input);

      if (input == "") {
         errorMessage = "Error: No characters entered.";
      }
   } while (errorMessage.length());

   return input;
}


// choiceToChar() returns the first alphabetical character that corresponds
// with the user's choice
// Pre: menu is string and the first characters of each choice must be
// formatted as follows: menuNumber, period delimeter, space, choiceAsChar
// Post: choiceAsChar is returned
char choiceToChar(int choice, string menuString)
{
   char choiceAsChar;
   int index = 0;

   while (choice) {
      index = menuString.find('\n');
      menuString = menuString.substr(index + 1);
      choice -= 1;
   }

   choiceAsChar = menuString[3];
   return choiceAsChar;
}


// createSlideshow() allocates a string array to the referenced pointer and
// returns the array's size
// Pre: slideShow as string pointer reference
// Post: pointer in main() points to slideshow and slideCount is returned as
// integer
size_t createSlideshow(string *&slideShow)
{
   const size_t slideCount = 8;

   const string slide1 = R"(
1) A pointer is a variable that is used to store another variable's memory
address. Memory can not be allocated on the heap without the use of a
pointer.)";

   const string slide2 = R"(
2) As it is just a virtual address, a pointer's memory size is fixed
during runtime regardless of what datatype it is referencing.)";

   const string slide3 = R"(
3) A pointer doesn't inherently know what type of data it is referencing.
The programer is telling it via (char *, int *, ect) so that the compiler
knows the size of the data when it is dereferenced in the future.)";

   const string slide4 = R"(
4) The memory address of a pointer can be read with the * operator. The *
operator can also be used to multiply two values.)";

   const string slide5 = R"(
5) In this program, I am creating a pointer on the stack. Then, a secondary
function is called with that pointer as a argument.)";

   const string slide6 = R"(
6) The pointer is passed by reference with the & operator, so changes to it
are reflected back in main(). This secondary function creates an array on the
heap, assigns the pointer to first index that array, and returns its size)";

   const string slide7 = R"(
7) While the pointer referenced in main() now corresponds with the correct
memory address of the start of the string array on the heap, it has no
knowledge of how long the array is. This is why the secondary function has
to return the size to main().)";

   const string slide8 = R"(
8) Until the array that has been allocated on the heap is deleted, a pointer
must be pointing to it. Otherwise, there is no way to delete it from the
heap during runtime.

The end. Feel free to add your own slides!)";

   slideShow = new string[10]{slide1, slide2, slide3, slide4,
                              slide5, slide6, slide7, slide8};

   return slideCount;
}


// startSlideShow() prints each page accounting to user input
// Pre: pages is pointer to string array, pagesAmount is size of array
// Post: Relevant slides are printed until user exits
void startSlideShow(const string pages[], int pagesAmount)
{
   const char OPTION_NEXT = 'N';
   const char OPTION_BACK = 'B';
   const char OPTION_HOME = 'M';

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

   while (choiceAsChar != OPTION_HOME) {
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

      if (choiceAsChar == OPTION_NEXT) {
         pageNumber += 1;
      }
      else if (choiceAsChar == OPTION_BACK) {
         pageNumber -= 1;
      }
   }
}


// getMoreSlots() copys a dynamically allocated array into a new dynamically
// allocated array with 10 more slots
// Pre: pointer reference to string array, slideCount is array size
// Post: pointer now points to array on heap with 10 more elements
// Notes: There has got to be a better way to do this without creating a whole
// separate intermediary array on the stack...
void getMoreSlots(string *&slideShow, size_t slideCount)
{

   string copyArr[slideCount];

   for (size_t i = 0; i < slideCount; i++) {
      copyArr[i] = slideShow[i];
   }

   delete[] slideShow;
   slideShow = new string[slideCount + 10];

   for (size_t i = 0; i < slideCount; i++) {
      slideShow[i] = copyArr[i];
   }
}


// addSlide() includes string input in slideshow and ensures dynamically
// allocated array is large enough for the added slide
// Pre: pointer reference to string array, slideCount is array size
// Post: string array now includes string input
void addSlide(string *&slideShow, size_t slideCount)
{
   string slideText;
   if (slideCount % 10 == 0) {
      getMoreSlots(slideShow, slideCount);
   }

   slideText = getString("Enter text to put on slide: ");
   slideShow[slideCount] = "\n" + to_string(slideCount + 1) + ") " + slideText;
}
