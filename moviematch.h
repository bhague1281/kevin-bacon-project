/*
    moviematch.h

    The MIT License (MIT)                                                        
                                                                                     Copyright (c) 2013 Lacher, Kozhevnikov, Hague, Tanner 
*/

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

  // loads a movie/actor file
  void Load (char const * filename);

  // returns the number of movies required to get from actor to baseActor_
  unsigned long MovieDistance(char const * actor);
  
  // returns invalid value number (1 more than number of vertices)
  unsigned long UnconnectedVal();

private:
  char * baseActor_;
  typename fsu::SymbolGraph <fsu::String, size_t> sg_;
  
  /*Helper data for MovieDistance*/
  fsu::Vector < bool >          visited_;
  fsu::Vector < size_t >        distance_;
  fsu::Vector < AdjIterator >   neighbor_;
  fsu::Deque < size_t >         conQ_;
  
  /*Helper functions for MovieDistance*/
  AdjIterator NextNeighbor(fsu::String v);
  void ResetSearchMetadata();
  bool CompareIgnoreCase(fsu::String a, fsu::String b); 
  
  /*Helper functions for Load*/
  int ParseSubStr(std::istream & is, fsu::String & str);
  int ParseSubStrNoop(std::istream & is);
  char * ResizeCStr(char * cstr, size_t sizeOld, size_t sizeNew);
};

MovieMatch::MovieMatch(char const * baseActor)
: baseActor_(0), sg_(), visited_(), distance_(), neighbor_(), conQ_()
{
  size_t length = strlen(baseActor);
  baseActor_ = new char [length + 1];
  baseActor_[length] = '\0';
  strcpy(baseActor_,baseActor);
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
  while((temp = is.get()) != '\n' && temp != '/' && temp != EOF) {}
  return temp;
}

char * MovieMatch::ResizeCStr(char * cstr, size_t sizeOld, size_t sizeNew)
{
  char * cstrOld = cstr;
  cstr = new char[sizeNew];

  /*memcpy instead of strcpy or strncpy because if we're here, we don't need
  null character checks or the other overhead of the 'str' family.*/
  memcpy(cstr, cstrOld, sizeOld);

  delete[] cstrOld;
  return cstr;
}

/*
  Reference:
  To convert name to index: sg_.GetSymbolMap().Get(fsu::String);
  To convert index to name: (sg_.GetVertexMap())[index];

  Also, remember to wrap char * in fsu::String with Wrap().
*/
unsigned long MovieMatch::MovieDistance(char const * actor)
{
  ResetSearchMetadata(); /*Initialization refactored away for clarity.*/
  
  /*Generic string object to hold all temporary strings.*/
  fsu::String str = fsu::String(actor); /*Start with searched actor name.*/

  /*Generic size_t vars to hold the numerical index for each vertex.*/
  size_t neighbor, v;
  
  if(!sg_.GetSymbolMap().Retrieve(str, v))
  {
    return 0; /*Abort if actor supplied isn't in graph.*/
  }
  
  conQ_.PushBack(v);
  visited_[v] = true;
  distance_[v] = 0;
  
  AdjIterator neighborItr;

  while (!conQ_.Empty())
  {
    v = conQ_.Front();
    str = (sg_.GetVertexMap())[v];
    neighborItr = NextNeighbor(str);
    
    /*This ensures we try all neighbors*/
    if(neighborItr != sg_.GetAbstractGraph().End(v))
    {
      /*Iterator *s to the underlying graph's vertex, not a string.*/
      neighbor = (size_t) *neighborItr;

      if(visited_[neighbor] == false) /*Skip vertexes visited prior*/
      {
        conQ_.PushBack(neighbor);
        visited_[neighbor] = true;
        distance_[neighbor] = distance_[v] + 1;

        if(neighbor == sg_.GetSymbolMap().Get(fsu::String(baseActor_)))
        {
          return distance_[neighbor] / 2;
        }
      }
    }
    else
    {
      conQ_.PopFront();
    }
  }
  
  return UnconnectedVal();
}

typename MovieMatch::AdjIterator MovieMatch::NextNeighbor(fsu::String v)
{
  /*Convert fsu::String v to numerical vertex index*/
  size_t i = sg_.GetSymbolMap().Get(v);

  /*Assumption: the current neighbor_[v] is unvisited*/
  AdjIterator nn = neighbor_[i]; /*Save current next neighbor for return.*/
  
  /*Assumption: returned neighbor will be visited before this is called
  again, so the next neighbor will be the first unvisited one by then.*/
  /*Except we don't want to ++ to the end/tail_ link/vertex*/
  /*Use g_ from sg_ to save sg_'s end the second s2n_ lookup.*/
  if(neighbor_[i] != sg_.GetAbstractGraph().End(i))
  {
    ++(neighbor_[i]); /*Increment in-place to prep for next call.*/
  }
  return nn;
}

void MovieMatch::ResetSearchMetadata()
{
  /*SetSize with a value will only set elements from the current size on, so to
  make this work as a reset rather than just initialization, we set the size to
  zero so that the later SetSize call initializes the whole vector.*/
  visited_.SetSize(0);
  distance_.SetSize(0);
  /*Not needed for neighbor_ which is set in a loop anyway*/
  
  visited_.SetSize(sg_.VrtxSize(), false);

  /*Initialize all distances to longer than possible.*/
  distance_.SetSize(sg_.VrtxSize(), 1 + sg_.EdgeSize());

  //Set the neighbor_ to each Vertex' first neighbor in the neighbor list.
  neighbor_.SetSize(sg_.VrtxSize());

  for (size_t i = 0; i < neighbor_.Size(); ++i)
  {
    //neighbor_[i] = sg_.Begin(sg_.GetVertexMap()[i]);
    /*More efficient to get g_ from within sg_ than to translate i back/forth*/
    neighbor_[i] = sg_.GetAbstractGraph().Begin(i);
  }
  conQ_.Clear();
}

unsigned long MovieMatch::UnconnectedVal()
{
  return 1 + sg_.EdgeSize();
}

#endif
