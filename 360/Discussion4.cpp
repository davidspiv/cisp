/******************************************************************************
# Author:           David Spivack
# Discussion:       4
# Date:             9/19/24
# Sources:          Discussion 3 specifications
# Comments:         I struggled with types being implicitly converted.
#                   Sometimes I would get an int when expecting a float.
#******************************************************************************/

#include "valid.h"
#include <iomanip>
#include <iostream>
using namespace std;

// function prototypes
void printMenu();
int getChoice();
void viewBalance(float balance);
float getDeposit();
float getWithdraw(float balance);

const int BALANCE = 1;
const int DEPOSIT = 2;
const int WITHDRAW = 3;
const int QUIT = 4;


int main()
{
   int choice = 0;
   float balance = 0.0;
   float deposit = 0.0;
   float withdraw = 0.0;

   cout << "Welcome to My Credit Union!" << endl;

   printMenu();

   while (choice != QUIT) {

      choice = getChoice();
      if (choice == BALANCE) {
         viewBalance(balance);
      }
      else if (choice == DEPOSIT) {
         deposit = getDeposit();
         balance = balance + deposit;
      }
      else if (choice == WITHDRAW) {
         withdraw = getWithdraw(balance);
         balance = balance - withdraw;
      }
   }

   cout << "\nThank you for using My Credit Union" << endl;

   return 0;
}


void printMenu()
{
   cout << "\n1. View balance" << endl;
   cout << "2. Make deposit" << endl;
   cout << "3. Withdraw funds" << endl;
   cout << "4. Quit" << endl;
}


int getChoice()
{
   int choice = 0;
   choice = getInteger("\nEnter choice: ");

   while (choice <= 0 || choice > 4) {
      cout << "Error: Must match number from menu" << endl;
      printMenu();
      choice = getInteger("\nEnter choice: ");
   }

   return choice;
};


void viewBalance(float balance)
{
   cout << fixed << setprecision(2);
   cout << "Account balance $" << balance << endl;
}


float getDeposit()
{
   float deposit = 0;
   deposit = getFloat("Enter deposit amount $ ");

   while (deposit < 0) {
      cout << "Error: Must be 0 greater than or equal to 0" << endl;
      deposit = getFloat("Enter deposit amount $ ");
   }

   return deposit;
}


float getWithdraw(float balance)
{
   float withdraw = 0;
   withdraw = getFloat("Enter withdraw amount $ ");

   while (withdraw < 0 || withdraw > balance) {
      cout << "Error: Must be between 0 and current account balance inclusive"
           << endl;
      withdraw = getFloat("Enter withdraw amount $ ");
   }

   return withdraw;
}
