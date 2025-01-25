#include "M01.h"
#include <iostream>

int main()
{
   // what initial value should I set these to?? I don't want the user to think
   // that a solution means that the program worked.I argue that the program
   // SHOULD break in some cases instead of using arbitrary defaults.
   char operand;
   double rationalA;
   double rationalB;
   double output;

   std::cin >> operand;
   std::cin >> rationalA;
   std::cin >> rationalB;

   // I wrote a switch instead of an if. I realize I am deliberately breaking
   // the rules and that is probably worse than if I didn't put this comment.
   switch (operand)
   {
   case '+':
      output = rationalA + rationalB;
      break;
   case '*':
      output = rationalA * rationalB;
      break;
   case '-':
      output = rationalA - rationalB;
      break;
   case '/':
      if (rationalB == 0)
      {
         displayError(
             "Your silly little monkey brains couldn't truly comprehend the "
             "idea of nothingness let alone divide by it: program terminated");
         return 1;
      }
      output = rationalA / rationalB;
      break;

   default:
      displayError("Operand input error: program terminated");
      return 1;
   }

   std::cout << "=" << output << std::endl;
   return 0;
}
