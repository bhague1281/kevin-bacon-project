#ifndef _MOVIEMATCH_H
#define _MOVIEMATCH_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <deque.h>
#include <cstdlib> /*For "exit()", used but not really needed.*/
#include <xstring.h>
#include <symgraph.h>

size_t const dEF_SIZE = 65536;
size_t const dEF_SIZE_STR = 128;
size_t const dEF_MULT = 2;
char const * const eRR_GRAPH_CAP = "Error: Graph capacity smaller than graph!\n";
char const * const eRR_STR_CAP = "Error: String Parser: i > strSize!\n";
char const * const eRR_READ_FILE = "Error: File read failed!\n";
char const * const wTF = "This should never happen!\n";

class MovieMatch
{
public:
  typedef
    typename fsu::SymbolGraph< fsu::String, size_t >::AdjIterator AdjIterator;

  MovieMatch (char const * baseActor);

  void Load (char const * filename);
  // loads a movie/actor file

  unsigned long MovieDistance(char const * actor);
  // returns the number of movies required to get from actor to baseActor_

  //...

private:
  char * baseActor_;
  typename fsu::SymbolGraph <fsu::String, size_t> sg_;
  
  fsu::Vector < bool >          visited_;
  fsu::Vector < unsigned long > distance_;
  fsu::Vector < AdjIterator >   neighbor_;
  fsu::Deque < fsu::String >    conQ_;
  AdjIterator NextNeighbor(fsu::String v);
  
  int ParseSubStr(std::istream & is, fsu::String & str);
  int ParseSubStrNoop(std::istream & is);
  char * ResizeCStr(char * cstr, size_t sizeOld, size_t sizeNew);
  
};

MovieMatch::MovieMatch(char const * baseActor)
: baseActor_(0), sg_(), visited_(), distance_(), neighbor_(), conQ_()
/*
sg_ is either uninitialized or just initialized blank at this point; there is
no way for it's VrtxSize or EdgeSize to be anything but zero here, so we just
use default constructors to initialize the other variables blank.
*/
{
  size_t length = strlen(baseActor);
  baseActor_ = new char [length + 1];
  baseActor_[length] = '\0';
  strcpy(baseActor_,baseActor);
  
  /* This needs to go somewhere else, it does nothing here.
  for (size_t i = 0; i < sg_.VrtxSize(); ++i)
  {
    neighbor_[i] = sg_.Begin(i);
  }
  */
}


void MovieMatch::Load(char const * filename)
{
  std::ifstream infile(filename);
  /*Variant two*/
  fsu::String curMovie = fsu::String(), str = fsu::String();
  int delim;
  bool movieNext = true;
  size_t count;
  /*Two pass, first pass counts delimiters blindly*/
  for(count = 0; infile.good(); count += 1)
  {
    /*Parse through data ignoring everything*/
    delim = ParseSubStrNoop(infile);
  }
  if(!infile.eof())
  {
    std::cerr << eRR_READ_FILE;
    exit(1);
  }
  infile.clear();
  infile.seekg(0, infile.beg);
  /*Above a certain count it seems better to set a fixed smaller fixes size*/
  /*Not sure why though. (Vertex Size still increases though.)*/
  if(count > 10000)
  {
    count = 10000;
  }
  sg_.SetVrtxSize(count);
  for(count = 0; infile.good(); count += 1)
  {
//std::cout << count << '\n';
//std::cout << sg_.Size() << '\n';
//std::cout << sg_.VrtxSize() << '\n';
    /*Get movie title or actor name*/
    delim = ParseSubStr(infile, str);
//std::cout << str << std::endl; //Extra statement to debug split strings.
//std::cout << (char)delim; //Extra statement to debug splitting by delim.
    /*both actors and movies are vertexes. Push() accounts for duplicates.*/
    sg_.Push(str);
    
    if(movieNext) /*Got a movie, prepare to get actors next.*/
    {
      curMovie = str;
      movieNext = false;
    }
    else /*Got an actor, so need to link*/
    {
      sg_.AddEdge(curMovie, str);
    }
    
    if(delim == '\n') /*Movie records at the beginning of every line*/
    {
      movieNext = true;
    }
  }
  
  if(!infile.eof())
  {
    std::cerr << eRR_READ_FILE;
std::cout << "test2";
  }
  infile.close();
}

/*Herein lies the biggest reinvention of many wheels of this project.
And yet no wheel herein reinvented was available in exactly the form needed
for what we were doing here in the standard lib. Also, I am awesome. -A.K.*/
int MovieMatch::ParseSubStr(std::istream & is, fsu::String & str)
{
  size_t i, strCap = dEF_SIZE_STR;
  char * cstr = new char[strCap];
  int temp;
  
  for(i = 0; (temp = is.get()) != '\n' && temp != '/' && temp != EOF; i+=1)
  {
    if(i == strCap) /*Resize string if it needs to get bigger.*/
    {
      strCap *= dEF_MULT;
      cstr = ResizeCStr(cstr, i, strCap);
    }
    else if(i > strCap)
    {
      std::cerr << eRR_STR_CAP << wTF;
      exit(1); /*Omfg panic!*/
    }
    cstr[i] = (char) temp;
  }
  if(i == strCap)
  {
    strCap += 1;
    cstr = ResizeCStr(cstr, i, strCap);
  }
  else if(i > strCap)
  {
    std::cerr << eRR_STR_CAP << wTF;
    exit(1); /*Omfg panic!*/
  }
  cstr[i] = '\0';
  
  str.Wrap(cstr);
  delete[] cstr;
  return temp;
}

int MovieMatch::ParseSubStrNoop(std::istream & is)
{
  int temp;
  while((temp = is.get()) != '\n' && temp != '/' && temp != EOF){}
  return temp;
}

char * MovieMatch::ResizeCStr(char * cstr, size_t sizeOld, size_t sizeNew)
{
  char * cstrOld = cstr;
  cstr = new char[sizeNew];
  memcpy(cstr, cstrOld, sizeOld);
  /*memcpy instead of strcpy or strncpy because if we're here, we don't need
  null character checks or the other overhead of the 'str' family.*/
  delete[] cstrOld;
  return cstr;
}

/*\
 * Reference:
To convert name to index: sg_.GetSymbolMap().Get(fsu::String);
To convert index to name: (sg_.GetVertexMap())[index];

Also, remember to wrap char * in fsu::String with Wrap().
\*/
unsigned long MovieMatch::MovieDistance(char const * actor)
{
  fsu::String str = fsu::String();
  
  str.Wrap(baseActor_);
  conQ_.PushBack(str);
  visited_[sg_.GetSymbolMap().Get(str)] = true;
  
  while (!conQ_.Empty())
  {
    fsu::String f = conQ_.Front();
    fsu::String n = (sg_.GetVertexMap())[*NextNeighbor(f)];
    AdjIterator l = (NextNeighbor(f))++;
    if (l != sg_.End(f) && visited_[sg_.GetSymbolMap().Get(n)] == false)
    {
      conQ_.PushBack(n);
      visited_[sg_.GetSymbolMap().Get(n)] = true;
      distance_[sg_.GetSymbolMap().Get(n)] = distance_[sg_.GetSymbolMap().Get(f)] + 1;
      if(n == actor) /*fsu::String overloads == but doesn't define .equals()*/
      {
        return distance_[sg_.GetSymbolMap().Get(n)] / 2;
      }
    }
    else
      conQ_.PopFront();
  }
  
  return 0;
}

typename MovieMatch::AdjIterator MovieMatch::NextNeighbor(fsu::String v)
{
  return neighbor_[sg_.GetSymbolMap().Get(v)];
}

#endif
