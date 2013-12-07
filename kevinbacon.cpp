 /*
    KevinBacon.cpp
    09/18/13

    Copyright 2013, R.C. Lacher
*/


#include <moviematch.h>
#include <xstring.h>

int main()
{
  MovieMatch mm ("Kevin Bacon");
  mm.Load("movies.txt");
  std::cout << "Welcome to Kevin Bacon\n";
  fsu::String actor = "1";
  do
  {
    std::cout << "Enter actor name ('0' to quit): ";
    actor.GetLine(std::cin);
    if (actor[0] == '0') break;
    std::cout << " The KB Number of " << actor << " is: "
              << mm.MovieDistance(actor.Cstr()) 
              << '\n';
  }
  while (actor[0] != '0');
  std::cout << "Thank you for playing Kevin Bacon\n";
  return EXIT_SUCCESS;
}

