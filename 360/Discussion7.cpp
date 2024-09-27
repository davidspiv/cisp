/******************************************************************************
# Author:           David Spivack
# Discussion:       7
# Date:             9/26/24
# Description:      Takes text file input of majors by men and women and
#                   transforms into text file output of percentages by gender
#                   and max / min values
# Sources:          Discussion 7 specifications
# Comments:         I put unprocessed data into four separate
#                   parallel arrays. My business functions populate a struct
#                   passed by ref instead of returning values. The program can
#                   be shorter by just declaring everything on the stack but I
#                   wanted to assume we didn't know input size at compile time.
#******************************************************************************/

#include "valid1.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

struct StemData {
   string *majors;
   int *men;
   int *women;
   int *salaries;
   size_t lineCount;
};

struct StemResults {
   double *menRatio;
   double *womenRatio;
   size_t indexMaxSalary;
   size_t indexMinSalary;
};

// Utilities
template <typename T> void expandArray(T *&arr, size_t &size, size_t step);
string formatDouble(double input, int decimalPlaces = 0);

// File I/O
StemData getStemData(ifstream &inFile);
void writeFile(ofstream &outFile, StemData const &stemData,
               StemResults const &stemResults);

// Business
void ratioCalc(StemData const &stemData, StemResults &stemResults);
void maxMinCalc(StemData const &stemData, StemResults &stemResults);


int main()
{
   ifstream inFile;
   ofstream outFile;
   StemData stemData;
   StemResults stemResults = {nullptr, nullptr, 0, 0};

   if (!openFile(inFile, "./stem.txt")) {
      cout << "Input File did not open. Program terminating!!!";
      return 1;
   }

   stemData = getStemData(inFile);
   inFile.close();

   if (!stemData.lineCount) {
      cout << "No lines to input. Program terminating!!!" << endl;
      return 1;
   }

   ratioCalc(stemData, stemResults);
   maxMinCalc(stemData, stemResults);

   if (!openFile(outFile, "./stemout.txt")) {
      cout << "Output file did not open. Program terminating!!!" << endl;
      return 1;
   };

   writeFile(outFile, stemData, stemResults);
   outFile.close();
   cout << "Successfully wrote to file!" << endl;

   delete[] stemData.majors;
   delete[] stemData.men;
   delete[] stemData.women;
   delete[] stemData.salaries;
   delete[] stemResults.menRatio;
   delete[] stemResults.womenRatio;
   stemData.majors = nullptr;
   stemData.men = nullptr;
   stemData.women = nullptr;
   stemData.salaries = nullptr;
   stemResults.menRatio = nullptr;
   stemResults.womenRatio = nullptr;

   cout << "end program" << endl;
   return 0;
}


// expandArray() copys dynamically assigned array into larger array
// Pre: pointer to unspecified type array, array size, step as number of
// elements to grow
// Post: pointer is redirected to larger array, original array is deleted
template <typename T> void expandArray(T *&arr, size_t &size, size_t step)
{
   T *newArr = new T[size + step];

   for (size_t i = 0; i < size; i++) {
      newArr[i] = arr[i];
   }

   delete[] arr;
   arr = newArr;
}


// getStemData() formats input stream into separate parallel arrays
// Pre: inFile is input file stream
// Post: parallel arrays along with size are returned within a StemData struct
StemData getStemData(ifstream &inFile)
{
   string fileText = "";
   size_t wordCount = 0;
   const size_t columns = 4;
   size_t size = 40;      // number of elements in parallel arrays
   const size_t step = 5; // number of elements to increase by if needed

   string *majors = new string[size];
   int *men = new int[size];
   int *women = new int[size];
   int *salaries = new int[size];
   size_t lineCount = 0;

   // ignore first line
   getline(inFile, fileText);

   while (inFile >> fileText) {

      switch (wordCount % columns) {
      case 0:
         majors[lineCount] = fileText;
         break;
      case 1:
         men[lineCount] = stoi(fileText);
         break;
      case 2:
         women[lineCount] = stoi(fileText);
         break;
      case 3:
         salaries[lineCount] = stoi(fileText);
         lineCount++;
         break;
      }

      if (lineCount == size) {
         expandArray(majors, size, step);
         expandArray(men, size, step);
         expandArray(women, size, step);
         expandArray(salaries, size, step);
         size += step;
      }

      wordCount++;
   }

   return {majors, men, women, salaries, lineCount};
}


// ratioCalc gets ratio of men and women per major
// Pre: stemData is ref struct
// Pre: stemResults is ref struct
void ratioCalc(StemData const &stemData, StemResults &stemResults)
{
   double total = 0.0;

   stemResults.menRatio = new double[stemData.lineCount];
   stemResults.womenRatio = new double[stemData.lineCount];

   for (size_t i = 0; i < stemData.lineCount; i++) {
      total = (stemData.men[i] + stemData.women[i]) / 100.0;
      stemResults.menRatio[i] = stemData.men[i] / total;
      stemResults.womenRatio[i] = stemData.women[i] / total;
   }
}


// maxMinCalc get index of largest and smallest salary
// Pre: stemData is ref struct
// Pre: stemResults is ref struct
void maxMinCalc(StemData const &stemData, StemResults &stemResults)
{
   size_t indexOfMax = 0;
   size_t indexOfMin = 0;

   for (size_t i = 0; i < stemData.lineCount; i++) {
      if (stemData.salaries[i] > stemData.salaries[indexOfMax]) {
         indexOfMax = i;
      }

      if (stemData.salaries[i] < stemData.salaries[indexOfMin]) {
         indexOfMin = i;
      }
   }

   stemResults.indexMaxSalary = indexOfMax;
   stemResults.indexMinSalary = indexOfMin;
}


// formatDouble() will convert a double to a comma separated value with an
// argument-defined number of decimal places
// Pre: input is a double, decimalPlaces is an int
// Post: the formatted string is returned
string formatDouble(double input, int decimalPlaces)
{
   string inputAsString = to_string(input);

   if (decimalPlaces < 0) {
      cout << "Unable to format: decimal places must be 0 or greater" << endl;
      return inputAsString;
   }

   // remove decimal character when no decimalPlaces
   if (!decimalPlaces) {
      decimalPlaces--;
   }

   inputAsString =
       inputAsString.substr(0, inputAsString.length() + decimalPlaces - 6);

   for (int i = inputAsString.length() - decimalPlaces - 4; i > 0; i = i - 3) {
      inputAsString.insert(i, ",");
   }

   return inputAsString;
}


// writeFile() writes formatted results to output file stream
// Pre: outFile is ref output stream stemData is ref struct
// Post: stemResults is ref struct
void writeFile(ofstream &outFile, StemData const &stemData,
               StemResults const &stemResults)
{
   const string maxSalaryFormatted =
       formatDouble(stemData.salaries[stemResults.indexMaxSalary]);
   const string minSalaryFormatted =
       formatDouble(stemData.salaries[stemResults.indexMinSalary]);

   outFile << "Highest salary: $" << maxSalaryFormatted << " from "
           << stemData.majors[stemResults.indexMaxSalary] << endl;
   outFile << "Lowest salary: $" << minSalaryFormatted << " from "
           << stemData.majors[stemResults.indexMinSalary] << '\n'
           << endl;

   outFile << "Major Percent Men Percent Women" << endl;

   for (size_t i = 0; i < stemData.lineCount; i++) {
      outFile << setprecision(4) << stemData.majors[i] << ' '
              << stemResults.menRatio[i] << ' ' << stemResults.womenRatio[i]
              << endl;
   }
}
