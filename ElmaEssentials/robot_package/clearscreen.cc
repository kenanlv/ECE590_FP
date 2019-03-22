// clearscreen.cpp
// #include <disclaimer.h>

#include "clearscreen.h"
#include <iostream>
#include <string>
// Paste one of the above ClearScreen code snippets here.
// For example, here's the POSIX stuff:
#include <unistd.h>
#include <term.h>

// void ClearScreen()
//   {
//   if (!cur_term)
//     {
//     int result;
//     setupterm( NULL, STDOUT_FILENO, &result );
//     if (result <= 0) return;
//     }

//   putp( tigetstr( "clear" ) );
//   }

void ClearScreen()
{
    int n;
    for (n = 0; n < 10; n++)
        std::cout<< "\n\n\n\n\n\n\n\n\n\n";
}