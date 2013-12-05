#ifndef _MOVIEMATCH_H
#define _MOVIEMATCH_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <xstring.h>
#include <hashtbl.h>
#include <symgraph.h>

size_t const dEF_SIZE = 128;
size_t const dEF_MULT = 2;
char const * const eRR_GRAPH_CAP = "Error: Graph capacity smaller than graph!\n";
char const * const wTF = "This should never happen!\n";

class MovieMatch
{
public:

  MovieMatch (char const * baseActor) : baseActor_(0)
  {
    size_t length = strlen(baseActor);
    baseActor_ = new char [length + 1];
    baseActor_[length] = '\0';
    strcpy (baseActor_,baseActor);
  }

  void Load (char const * filename);
  // loads a movie/actor file

  unsigned long MovieDistance(char const * actor);
  // returns the number of movies required to get from actor to baseActor_

  //...

private:
  char* baseActor_;
  fsu::SymbolGraph <fsu::String, size_t> sg_;
  
  fsu::String helperParseSubStr(std::istream & is, char delim);
  
};

void MovieMatch::Load(char const * filename)
{
  std::ifstream infile(filename);
  /*Variant one*/
  //fsu::String * str = ;
  size_t graphCap = dEF_SIZE; /*Current capacity of the graph*/
  sg_.SetVrtxSize(graphCap);
  while(infile.good())
  {
    /*If size reaches capacity, multiply capacity and resize graph.*/
    if(graphCap == sg_.Size())
    {
      graphCap *= dEF_MULT;
      sg_.SetVrtxSize(graphCap);
    }
    else if(graphCap < sg_.Size()) /*Protect against the 'impossible'.*/
    {
      std::cerr << eRR_GRAPH_CAP << wTF;
      exit(1); /*Omfg abort everything*/
    }
    
    /*Get movie title*/
    
  }
  
}

fsu::String MovieMatch::helperParseSubStr(std::istream & is, char delim)
{
  size_t strSize = dEF_SIZE;
  char * cstr = new char[strSize];
  int temp;
  
  while((temp = is.get()) != (int) delim && temp != EOF)
  {
    
  }
  
  fsu::String str(cstr);
  delete[] cstr;
  return str;
}

#endif
