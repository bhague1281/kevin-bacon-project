#ifndef _MOVIEMATCH_H
#define _MOVIEMATCH_H

#include <string>
#include <symgraph.h>

class MovieMatch
{
public:

  MovieMatch (const char* baseActor) : baseActor_(0)
  {
    size_t length = strlen(baseActor);
    baseActor_ = new char [length + 1];
    baseActor_[length] = '\0';
    strcpy (baseActor_,baseActor);
  }

  void Load (const char* filename);
  // loads a movie/actor file

  unsigned long MovieDistance(const char* actor);
  // returns the number of movies required to get from actor to baseActor_

  ...

private:
  char* baseActor_;
  SymbolGraph < fsu::String , size_t > sg_;
  ...
};

#endif
