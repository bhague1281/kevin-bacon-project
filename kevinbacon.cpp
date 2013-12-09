/*
    kevinbacon.cpp
    09/18/13

    The MIT License (MIT)

    Copyright (c) 2013 Lacher, Kozhevnikov, Hague, Tanner

*/


#include <moviematch.h>
#include <xstring.h>

int main()
{
  std::cout << "Welcome to Kevin Bacon\n";
  MovieMatch mm("Bacon, Kevin");
  std::cout << "Wait a few seconds while the movie database loads...\n";
  mm.Load("movies.txt");
  std::cout << "Movie database loaded.\n";
  fsu::String actor = "1";
  unsigned long int distance;
  do
  {
    std::cout << "Enter actor name [lastname, firstname] ('0' to quit): ";
    actor.GetLine(std::cin);
    if (actor[0] == '0')
    {
      break;
    }
    if(actor == fsu::String("Bacon, Kevin"))
    {
      std::cout << "Kevin Bacon has a KB Number of 0 by definition.\n";
      continue; /*Skip the rest of this loop iteration.*/
    }
    distance = mm.MovieDistance(actor.Cstr());
    if(distance == 0)
    {
      std::cout << "Actor " << actor
                << " could not be connected to Kevin Bacon\n";
    }
    else if(distance == mm.UnconnectedVal())
    {
      std::cout << "Actor " << actor << " was not found in the database.\n";
    }
    else
    {
      std::cout << " The KB Number of " << actor << " is: "
                << distance << '\n';
    }
  }
  while (actor[0] != '0');
  std::cout << "Thank you for playing Kevin Bacon\n";
  return EXIT_SUCCESS;
}

