#include "valid.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

// function prototypes
void ratioCalc(ifstream &inFile);

int main()
{
   ifstream inFile;
   string fileName;

   if (!openFile(inFile, "./360/stem.txt")) {
      cout << "file did not open. Program terminating!!!";
      return 0;
   }
   ratioCalc(inFile);
   inFile.close();
}


// write the ratioCalc function below this
void ratioCalc(ifstream &inFile)
{
   string fileText;
   inFile >> fileText;

   while (!inFile.fail()) {
      inFile >> fileText;
      cout << "fileText: " << fileText << endl;
   }

   if (!inFile.eof()) {
      cout << "Input failure before reaching end of file." << endl;
   }

   cout << "Closing file myfile.txt." << endl;

   // Done with file, so close it
   inFile.close();
}
