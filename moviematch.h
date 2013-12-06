#ifndef _MOVIEMATCH_H
#define _MOVIEMATCH_H

#include <iostream>
#include <fstream>
#include <cstring>
//#include <deque.h>
#include <cstdlib> /*For "exit()", used but not really needed.*/
#include <xstring.h>
#include <symgraph.h>
/*All of these might be unneeded, but slapped in because maybe fixes
compilation.*/

size_t const dEF_SIZE = 128;
size_t const dEF_MULT = 2;
char const * const eRR_GRAPH_CAP = "Error: Graph capacity smaller than graph!\n";
char const * const eRR_STR_CAP = "Error: String Parser: i > strSize!\n";
char const * const eRR_READ_FILE = "Error: File read failed!\n";
char const * const wTF = "This should never happen!\n";

class MovieMatch
{
public:
  //typedef typename fsu::SymbolGraph <fsu::String, size_t>::AdjIterator AdjIterator

  MovieMatch (char const * baseActor);

  void Load (char const * filename);
  // loads a movie/actor file

  unsigned long MovieDistance(char const * actor);
  // returns the number of movies required to get from actor to baseActor_

  //...

private:
  char* baseActor_;
  typename fsu::SymbolGraph <fsu::String, size_t> sg_;
  /*
  fsu::Vector < bool >          visited_;
  fsu::Vector < unsigned long > distance_;
  fsu::Vector < AdjIterator >   neighbor_;
  fsu::Deque < fsu::String >    conQ_;
  AdjIterator NextNeighbor(fsu::String v);
  */
  int ParseSubStr(std::istream & is, fsu::String & str);
  char * ResizeCStr(char * cstr, size_t sizeOld, size_t sizeNew);
  
};

MovieMatch::MovieMatch(char const * baseActor) : baseActor_(0), sg_()//,
/* sg_ is either uninitialized or just initialized blank at this point; there
is no way for it's VrtxSize or EdgeSize to be anything but zero here.
*/
  /*distance_ (sg_.VrtxSize(), 1 + sg_.EdgeSize()),
  visited_ (sg_.VrtxSize(), 0),
  neighbor_ (sg_.VrtxSize()), conQ_()*/
{
  size_t length = strlen(baseActor);
  baseActor_ = new char [length + 1];
  baseActor_[length] = '\0';
  strcpy(baseActor_,baseActor);
  //sg_();
  
  /*for (size_t i = 0; i < sg_.VrtxSize(); ++i)
    neighbor_[i] = sg_.Begin(i);*/
}


void MovieMatch::Load(char const * filename)
{
  std::ifstream infile(filename);
  /*Variant one (there might be others coded later for funsies*/
  fsu::String curMovie = fsu::String(), str = fsu::String();
  int delim;
  bool movieNext = true;
  size_t graphCap = dEF_SIZE; /*Current capacity of the graph*/
  sg_.SetVrtxSize(graphCap);
  /*One pass, creates vertices and edges as it goes, but without*/
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
    /*Get movie title or actor name*/
    delim = ParseSubStr(infile, str);
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
  if(infile.fail())
  {
    std::cerr << eRR_READ_FILE;
  }
  infile.close();
}

/*Herein lies the biggest reinvention of many wheels of this project.
And yet no wheel herein reinvented was available in exactly the form needed
for what we were doing here in the standard lib. Also, I am awesome. -A.K.*/
int MovieMatch::ParseSubStr(std::istream & is, fsu::String & str)
{
  size_t i, strCap = dEF_SIZE;
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

unsigned long MovieMatch::MovieDistance(char const * actor)
{
/*  conQ_.PushBack(baseActor_);
  visited_[/ *actor to number* /] = true;
  
  while (!conQ_.Empty())
  {
    fsu::String f = conQ_.Front();
    fsu::String n = *NextNeighbor(f);
    AdjIterator l = NextNeighbor(f)++;
    if (l != sg_.End(f) && visited[/ *n to number* /] == false)
    {
      conQ_.PushBack(n);
      visited_[/ *n to number* /] = true;
      distance_[/ * n to number* /] = distance_[/ *f to number* /] + 1;
      if (n.equals(actor))
      {
        return distance[/ *n to number* /] / 2;
      }
    }
    else
      conQ_.PopFront();
  }
  */
  return 0;
}

/*typename MovieMatch::AdjIterator MovieMatch::NextNeighbor(fsu::String v)
{
  return neighbor_[/ *v to number* /];
}*/

#endif
