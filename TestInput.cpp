/*
    KevinBacon.cpp
    09/18/13

    Copyright 2013, R.C. Lacher
*/


//#include <moviematch.h>
#include <xstring.h>
#include <xstring.cpp>

fsu::String FormatName(fsu::String inputName);			// Converts input to a consistent format, i.e., John Smith -> Smith, John
bool CompareIgnoreCase(fsu::String first, fsu::String second);	// Compare input string with actor name ignoring case, i.e., "smith, john" == "SMITH, JOHN" is true
fsu::String AddComma(std::string str, size_t indexOfSpace);

int main()
{
  //MovieMatch mm ("Kevin Bacon");
  //mm.Load("movies.txt");
  std::cout << "Welcome to Kevin Bacon\n";
  fsu::String actor = "1";
  do
  {
    std::cout << "Enter actor name ('0' to quit): ";
    actor.GetLine(std::cin);
    if (actor[0] == '0') break;
    
    // get the format to be used for comparison
    fsu::String formattedInput = FormatName(actor);
    if (CompareIgnoreCase(formattedInput, "TeStStRiNg"))
      std::cout << "Comparison Works\n";
    
    std::cout << " The KB Number of " << actor << " is: "
              //<< mm.MovieDistance(actor.Cstr()) 
              << '\n';
  }
  while (actor[0] != '0');
  std::cout << "Thank you for playing Kevin Bacon\n";
  return EXIT_SUCCESS;
}

fsu::String FormatName(fsu::String inputName)
{
  std::size_t foundComma;
  std::size_t foundSpace;
  std::string str(inputName.Cstr());
  
  foundComma = str.find(',');
  foundSpace = str.find(' ');
  // if there is a comma, then assume the input is correct format
  if (foundComma != std::string::npos)
    return inputName;
  
  // if there is no comma, there must be a space to separate names
  if (foundSpace == std::string::npos)
  {
    std::cout << "You must enter an actor's first and last name\n\n";
    return inputName;
  }
  
  // if there is no comma and a space, convert it to correct format
  return AddComma(str, foundSpace);
}

fsu::String AddComma(std::string str, size_t indexOfSpace)
{
  std::string first = str.substr(0, indexOfSpace);
  std::string last = str.substr(indexOfSpace + 1, str.size());
  //std::cout << "First: " << first << " and last: " << last << "\n";
  std::string actorName = last + ", " + first;
  std::cout << "Converted Name = " << actorName << "\n";
  return fsu::String(actorName.c_str());
}

bool CompareIgnoreCase(fsu::String a, fsu::String b)
{
  // if sizes don't match, strings don't match
  unsigned int sz = a.Size();
  if (b.Size() != sz)
    return false;
  // loop through each character, comparing tolower() versions of each char
  // as soon as one char doesn't match, return false
  for (unsigned int i = 0; i < sz; ++i)
    if (tolower(a[i]) != tolower(b[i]))
      return false;
    return true;
}