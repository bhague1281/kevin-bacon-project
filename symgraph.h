#ifndef _SYMGRAPH_H
#define _SYMGRAPH_H

#include <graph.h>
#include <hashtbl.h>
#include <hashclasses.h>
#include <xstring.h>
/*
These below are technically chain included from the above includes, but better
practice to include everything we are actually using.
*/
#include <vector.h>

namespace fsu
{
  template < typename S , typename N >
  class SymbolGraph
  {
  public:
    typedef S                                   Vertex;
    typedef typename ALUGraph<N>::AdjIterator   AdjIterator;
    typedef hashclass::MM<fsu::String>          H;

    void   SetVrtxSize  (N n);
    void   SetVrtxSize  (N n, bool rehash);
    void   AddEdge      (Vertex from, Vertex to);
    size_t Size         () const;
    size_t VrtxSize     () const;
    size_t EdgeSize     () const;
    size_t OutDegree    (Vertex x) const;
    size_t InDegree     (Vertex x) const;
    AdjIterator Begin   (Vertex x) const;
    AdjIterator End     (Vertex x) const;

    void   Push         (const S& s); // add s to the vertex set

    // access to underlying data
    const ALUGraph<N>&      GetAbstractGraph() const; // reference to g_
    const HashTable<S,N,H>& GetSymbolMap() const; // reference to s2n_
    const Vector<S>&        GetVertexMap() const; // reference to n2s_

    SymbolGraph ( );
    SymbolGraph ( N n );
    SymbolGraph ( N n , bool bucketNumPrime);
    
  private:
    ALUGraph<N>      g_;
    HashTable<S,N,H> s2n_;
    Vector<S>        n2s_;
    size_t           size_; //Added for performance gains.
  }; // class SymbolGraph
  
  template < typename S , typename N >
  class SymbolDirectedGraph
  {
  public:
    typedef S                                   Vertex;
    typedef typename ALDGraph<N>::AdjIterator   AdjIterator;
    typedef typename hashclass::MM<fsu::String> H;

    void   SetVrtxSize  (N n);
    void   SetVrtxSize  (N n, bool rehash);
    void   AddEdge      (Vertex from, Vertex to);
    size_t Size         () const;
    size_t VrtxSize     () const;
    size_t EdgeSize     () const;
    size_t OutDegree    (Vertex x) const;
    size_t InDegree     (Vertex x) const;
    AdjIterator Begin   (Vertex x) const;
    AdjIterator End     (Vertex x) const;

    void   Push         (const S& s); // add s to the vertex set

    // access to underlying data
    const ALDGraph<N>&      GetAbstractGraph() const; // reference to g_
    const HashTable<S,N,H>& GetSymbolMap() const; // reference to s2n_
    const Vector<S>&        GetVertexMap() const; // reference to n2s_

    SymbolDirectedGraph ( );
    SymbolDirectedGraph ( N n );
    SymbolDirectedGraph ( N n , bool bucketNumPrime);
    
  private:
    ALDGraph<N>      g_;
    HashTable<S,N,H> s2n_;
    Vector<S>        n2s_;
    size_t           size_; //Added for performance gains.
  }; // class SymbolDirectedGraph
  
  /**\
  SymbolGraph implementation
  \**/
  template <typename S, typename N>
  SymbolGraph<S,N>::SymbolGraph() : g_(), s2n_(1), n2s_(), size_(0) {}
  
  template <typename S, typename N>
  SymbolGraph<S,N>::SymbolGraph(N n)
  : g_((size_t)n), s2n_((size_t)n), n2s_((size_t)n), size_(n) {}
  
  template <typename S, typename N>
  SymbolGraph<S,N>::SymbolGraph(N n , bool bucketNumPrime)
  : g_((size_t)n), s2n_((size_t)n, bucketNumPrime), n2s_((size_t)n), size_(n){}
  
  template <typename S, typename N>
  void SymbolGraph<S,N>::SetVrtxSize(N n)
  {
    SetVrtxSize(n, 1);
  }

  template <typename S, typename N>
  void SymbolGraph<S,N>::SetVrtxSize(N n, bool rehash)
  {
    g_.SetVrtxSize((size_t)n);
    n2s_.SetSize((size_t)n);
    if(rehash)
    {
      s2n_.Rehash((size_t)n);
    }
    /*Rehash doesn't remove excess elements, like fsu::Vector's SetSize()*/
    if(size_ > n)
    {
      /*So clear out hashtable and repopulate.*/
      s2n_.Clear();
      for(size_t i = 0; i < n2s_.Size(); i+=1)
      {
        s2n_.Put(n2s_[i], (N)i); /*n2s_[i] is string/key, i is number/data*/
      }
    }
    size_ = n;
  }
  
  template <typename S, typename N>
  void SymbolGraph<S,N>::AddEdge(Vertex from, Vertex to)
  {
    /*Just the internal graph's 'AddEdge' but lookup vertex name to index.*/
    g_.AddEdge(s2n_.Get(from), s2n_.Get(to));
  }
  
  template <typename S, typename N>
  size_t SymbolGraph<S,N>::Size() const
  {
    return size_;
  }
  
  template <typename S, typename N>
  size_t SymbolGraph<S,N>::VrtxSize() const
  {
    return g_.VrtxSize();
  }
  
  template <typename S, typename N>
  size_t SymbolGraph<S,N>::EdgeSize() const
  {
    return g_.EdgeSize();
  }
  
  template <typename S, typename N>
  size_t SymbolGraph<S,N>::OutDegree(Vertex x) const
  {
    return g_.OutDegree(s2n_[x]);
  }
  
  template <typename S, typename N>
  size_t SymbolGraph<S,N>::InDegree(Vertex x) const
  {
    return g_.InDegree(s2n_[x]);
  }
  
  template <typename S, typename N>
  typename SymbolGraph<S,N>::AdjIterator SymbolGraph<S,N>::Begin(Vertex x)
    const
  {
    return g_.Begin(s2n_[x]);
  }
  
  template <typename S, typename N>
  typename SymbolGraph<S,N>::AdjIterator SymbolGraph<S,N>::End(Vertex x) const
  {
    return g_.End(s2n_[x]);
  }
  
  template <typename S, typename N>
  void SymbolGraph<S,N>::Push(const S& s)
  {
    N dump = N();
    if(!s2n_.Retrieve(s, dump))
    {
      /*g_ and n2s_ always same size, but s2n_ might be smaller, due to the
      SetVrtxSize() expanding the former two.*/
      if(size_ < n2s_.Size())
      {
        n2s_[size_] = s;
        s2n_.Rehash(size_ + 1); //Do we want to rehash here? Inefficient...
      }
      else /*But otherwise we need to expand g_ and n2s_*/
      {
        g_.PushVertex();
        n2s_.PushBack(s);
      }
      s2n_.Put(s, size_);
      size_+=1;
    }
  }
  
  template <typename S, typename N>
  const ALUGraph<N> & SymbolGraph<S,N>::GetAbstractGraph() const
  {
    return g_;
  }
  
  template <typename S, typename N>
  const HashTable <S,N,hashclass::MM <fsu::String> > &
    SymbolGraph<S,N>::GetSymbolMap() const
  {
    return s2n_;
  }
  
  template <typename S, typename N>
  const Vector<S> & SymbolGraph<S,N>::GetVertexMap() const
  {
    return n2s_;
  }
  
  /**\
  SymbolDirectedGraph implementation
  \**/
  template <typename S, typename N>
  SymbolDirectedGraph<S,N>::SymbolDirectedGraph()
  : g_(), s2n_(1), n2s_(), size_(0) {}
  
  template <typename S, typename N>
  SymbolDirectedGraph<S,N>::SymbolDirectedGraph(N n)
  : g_((size_t)n), s2n_((size_t)n), n2s_((size_t)n), size_(n) {}
  
  template <typename S, typename N>
  SymbolDirectedGraph<S,N>::SymbolDirectedGraph(N n, bool bucketNumPrime)
  : g_((size_t)n), s2n_((size_t)n, bucketNumPrime), n2s_((size_t)n), size_(n){}
  
  template <typename S, typename N>
  void SymbolDirectedGraph<S,N>::SetVrtxSize(N n)
  {
    SetVrtxSize(n, 1);
  }

  template <typename S, typename N>
  void SymbolDirectedGraph<S,N>::SetVrtxSize(N n, bool rehash)
  {
    g_.SetVrtxSize((size_t)n);
    n2s_.SetSize((size_t)n);
    if(rehash)
    {
      s2n_.Rehash((size_t)n);
    }
    /*Rehash doesn't remove excess elements, like fsu::Vector's SetSize()*/
    if(size_ > n)
    {
      /*So clear out hashtable and repopulate.*/
      s2n_.Clear();
      for(size_t i = 0; i < n2s_.Size(); i+=1)
      {
        s2n_.Put(n2s_[i], (N)i); /*n2s_[i] is string/key, i is number/data*/
      }
      size_ = n;
    }
  }
  
  template <typename S, typename N>
  void SymbolDirectedGraph<S,N>::AddEdge(Vertex from, Vertex to)
  {
    /*Just the internal graph's 'AddEdge' but lookup vertex name to index.*/
    g_.AddEdge(s2n_.Get(from), s2n_.Get(to));
  }
  
  template <typename S, typename N>
  size_t SymbolDirectedGraph<S,N>::Size() const
  {
    return size_;
  }
  
  template <typename S, typename N>
  size_t SymbolDirectedGraph<S,N>::VrtxSize() const
  {
    return g_.VrtxSize();
  }
  
  template <typename S, typename N>
  size_t SymbolDirectedGraph<S,N>::EdgeSize() const
  {
    return g_.EdgeSize();
  }

  template <typename S, typename N>
  size_t SymbolDirectedGraph<S,N>::OutDegree(Vertex x) const
  {
    return g_.OutDegree(s2n_[x]);
  }

  template <typename S, typename N>
  size_t SymbolDirectedGraph<S,N>::InDegree(Vertex x) const
  {
    return g_.InDegree(s2n_[x]);
  }

  template <typename S, typename N>
  typename SymbolDirectedGraph<S,N>::AdjIterator
    SymbolDirectedGraph<S,N>::Begin(Vertex x) const
  {
    return g_.Begin(s2n_[x]);
  }

  template <typename S, typename N>
  typename SymbolDirectedGraph<S,N>::AdjIterator
    SymbolDirectedGraph<S,N>::End(Vertex x) const
  {
    return g_.End(s2n_[x]);
  }
  
  template <typename S, typename N>
  void SymbolDirectedGraph<S,N>::Push(const S& s)
  {
    if(!s2n_.Includes(s))
    {
      /*g_ and n2s_ always same size, but s2n_ might be smaller, due to the
      SetVrtxSize() expanding the former two.*/
      if(size_ < g_.Size())
      {
        n2s_[size_] = s;
        s2n_.Rehash(size_ + 1); //Do we want to rehash here? Inefficient...
      }
      else /*But otherwise we need to expand g_ and n2s_*/
      {
        g_.PushVertex();
        n2s_.PushBack(s);
      }
      s2n_.Put(s, size_);
      size_+=1;
    }
  }
  
  template <typename S, typename N>
  const ALDGraph<N> & SymbolDirectedGraph<S,N>::GetAbstractGraph() const
  {
    return g_;
  }
  
  template < typename S, typename N >
  const HashTable <S,N,hashclass::MM <fsu::String> > &
    SymbolDirectedGraph<S,N>::GetSymbolMap() const
  {
    return s2n_;
  }
  
  template <typename S, typename N>
  const Vector<S> & SymbolDirectedGraph<S,N>::GetVertexMap() const
  {
    return n2s_;
  }
} // namespace fsu

#endif
