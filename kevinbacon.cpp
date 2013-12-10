/*
    kevinbacon.cpp
    09/18/13

    The MIT License (MIT)

    Copyright (c) 2013 Lacher, Kozhevnikov, Hague, Tanner

*/


#include <moviematch.h>
#include <xstring.h>

fsu::String FormatName(fsu::String inputName);			// Converts input to a consistent format, i.e., John Smith -> Smith, John
bool CompareIgnoreCase(fsu::String first, fsu::String second);	// Compare input string with actor name ignoring case, i.e., "smith, john" == "SMITH, JOHN" is true
fsu::String AddGrammar(std::string str, size_t indexOfSpace);
fsu::String CorrectCapitalization(fsu::String a);

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
    std::cout << "\nEnter actor name: ('0' to quit): ";
    actor.GetLine(std::cin);
    
    if (actor.Size() == 0)
    {
      continue;
    }    
    
    if (actor == "0")
    {
      break;
    }
    
    // Format string input to the format used for matching
    // [Lastname], [Firstname]
    fsu::String formattedInput = FormatName(actor);
    
    std::cout << '\n';	// for good measure
    
    if(CompareIgnoreCase(formattedInput, "Bacon, Kevin"))
    {
      std::cout << "Kevin Bacon has a KB Number of 0 by definition.\n";
      continue; /*Skip the rest of this loop iteration.*/
    }    
    
    distance = mm.MovieDistance(formattedInput.Cstr());
    
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
  while (actor != "0");
  std::cout << "Thank you for playing Kevin Bacon\n";
  return EXIT_SUCCESS;
}

fsu::String FormatName(fsu::String inputName)
{
  std::size_t foundComma;
  std::size_t foundSpace;
  std::string str(inputName.Cstr());
  
  foundComma = str.find(',');
  // if there is a comma, then assume the input is correct format
  // but check capitalization
  if (foundComma != std::string::npos)
  {
    
    std::string first(inputName.Cstr());
    first = (first.substr(foundComma + 2, first.size()));	// + 2 removes the space and the comma
    std::string last = (inputName.Cstr());
    last = (last.substr(0, foundComma));
    
    str = first + " " + last;	// intentional misformat that will be corrected on return statement
    
    //std::cout << "Name now: " << str << '\n';
    
  }
  
  foundSpace = str.find(' ');
    
  // if there is no comma and no space, no match will be found...return
  if (foundSpace == std::string::npos)
    return inputName;
  
  std::size_t foundSpace2;
  // find the second space for actors with more than 2 names
  foundSpace2 = str.find(' ', foundSpace + 1);	
  // make sure the space isn't at the end for when we check indexes
  if (foundSpace2 != std::string::npos && foundSpace2 < str.size() - 1)	
  {
    foundSpace = foundSpace2;
  }  
  
  // if there is no comma and a space, convert it to correct format
  return AddGrammar(str, foundSpace);
}

fsu::String AddGrammar(std::string str, size_t indexOfSpace)
{
  fsu::String first(str.substr(0, indexOfSpace).c_str());
  fsu::String last(str.substr(indexOfSpace + 1, str.size()).c_str());
  
  first = CorrectCapitalization(first);
  last = CorrectCapitalization(last);
  
  fsu::String actorName = last + ", " + first;
  
  //std::cout << "Final name: " << actorName << '\n';
  
  return actorName;
}

fsu::String CorrectCapitalization(fsu::String a)
{
  bool passedSpace = false;
  bool passedParen = false;
  // capitalize the first letter
  // some names have more than two names
  // for those occasions, capitalize the next letter
  // for text inside parens, capitalize it
  a[0] = toupper(a[0]);
  for (size_t i = 1; i < a.Size(); ++i)
  {    
    if (a[i - 1] == ' ' || a[i - 1] == '.')
      passedSpace = true;
    if (a[i] == '(')
      passedParen = true;
    
    if (!passedSpace && !passedParen)
    {
      a[i] = tolower(a[i]);
    }
    else
    {
      a[i] = toupper(a[i]);
      passedSpace = false;
    }
  }
  return a;
}

bool CompareIgnoreCase(fsu::String a, fsu::String b)
{
  // if sizes don't match, strings don't match
  unsigned int sz = a.Size();
  if (b.Size() != sz)
    return false;
  // loop through each character, comparing tolower() versions of each char
  // as soon as one char doesn't match, return false
  for (size_t i = 0; i < sz; ++i)
    if (tolower(a[i]) != tolower(b[i]))
      return false;
    return true;
}