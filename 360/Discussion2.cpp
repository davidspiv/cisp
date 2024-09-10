/******************************************************************************
# Author:           David Spivack (refactored existing code)
# Discussion:       2
# Date:             9/9/24
# Description:      Print a receipt with total price from movie name and ticket
#                   number inputs.
# Sources:          Existing Movie Ticket program
# Comments:         I struggled to choose whether or not to keep the input as
#                   one function or separate it into two. Even though one
#                   function seems simpler I assume you would have to end
#                   up returning a non primitive value like a struct (barring
#                   global variables).
#******************************************************************************/

#include <iomanip>
#include <iostream>

using namespace std;

const float TICKET_PRICE = 13.50;


// getInputAsMovie() outputs a message to console dictating the type of input
// wanted from user and stores input to string variable.
// Pre: none
// Post: the input is returned and is not an empty string.
string getInputAsMovie()
{
   string input = "";
   cout << "\nEnter movie name: ";
   getline(cin, input);
   return input;
}


// getInputAsTickets() outputs a message to console dictating the type of input
// wanted from user and stores input to integer variable.
// Pre: none
// Post: the input is returned and is not 0.
int getInputAsTickets()
{
   int input = 0;
   cout << "Enter number of tickets: ";
   cin >> input;
   return input;
}


// calcTotalPrice() takes number of tickets as an input and multiplies them by
// the ticket amount. Returns result as integer.
// Pre: numTickets is an integer
// Post: The result is returned and doesn't equal 2147483647 and is not less
// than 0.
int calcTotalPrice(int numTickets)
{
   const int totalPrice = numTickets * TICKET_PRICE;
   return totalPrice;
}


// printReceipt() outputs a receipt to the console based on number of tickets,
// the ticket name, the amount purchased, and price.
// Pre: Movie is const string reference, number of tickets as integer,
// totalPrice as double
// Post: receipt outputs to console
void printReceipt(const string &movie, int numTickets, double totalPrice)
{
   cout << "\nHere is your order:" << endl;
   cout << fixed << setprecision(2);
   cout << "Movie: " << movie << endl;
   cout << numTickets << " tickets @ $" << TICKET_PRICE << " each." << endl;
   cout << "\nTotal due: $" << totalPrice << endl;
}


// main() displays start and end message and calls other functions
// Pre: none
// Post: "Enjoy your movie!" end message is played.
int main()
{
   string movie = "";
   int numTickets = 0;
   float totalPrice = 0;

   cout << "Welcome to Living Room Theaters!\n";

   movie = getInputAsMovie();
   numTickets = getInputAsTickets();

   totalPrice = calcTotalPrice(numTickets);
   printReceipt(movie, numTickets, totalPrice);

   cout << "\nEnjoy your movie!" << endl;

   return 0;
}
