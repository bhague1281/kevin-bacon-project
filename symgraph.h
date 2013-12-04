#ifndef _SYMGRAPH_H
#define _SYMGRAPH_H

#include <graph.h>
#include <hashtbl.h>
#include <hashclasses.h>
#include <xstring.h>

namespace fsu
{
  template < typename S , typename N >
  class SymbolGraph
  {
  public:
    typedef S      Vertex;
    typedef xxxxx  AdjIterator;
    //typedef typename Graph::AdjIterator AdjIterator;
    typedef hashclass::MM<fsu::String> H;

    void   SetVrtxSize  (N n);
    void   AddEdge      (Vertex from, Vertex to);
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
    //...
  private:
    ALUGraph<N>      g_;
    HashTable<S,N,H> s2n_;
    Vector<S>        n2s_;
    //...
  }; // class SymbolGraph
  
  template < typename S , typename N >
  class SymbolDirectedGraph
  {
  public:
    typedef S             Vertex;
    typedef xxxxx         AdjIterator;
    typedef hashclass::MM<fsu::String> H;

    void   SetVrtxSize  (N n);
    void   AddEdge      (Vertex from, Vertex to);
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
    //...
  private:
    ALDGraph<N>      g_;
    HashTable<S,N,H> s2n_;
    Vector<S>        n2s_;
    //...
  }; // class SymbolDirectedGraph
  
  /**\
  SymbolGraph implementation
  \**/
  
  template <typename S, typename N>
  typename const ALDGraph<N> & SymbolGraph<S,N>::GetAbstractGraph() const
  {
    return g_;
  }
  
  template <typename S, typename N, class H>
  typename const HashTable<S,N,H> & SymbolGraph<S,N>::GetSymbolMap() const
  {
    return s2n_;
  }
  
  template <typename S, typename N>
  typename const Vector<S> & SymbolGraph<S,N>::GetVertexMap() const
  {
    return n2s_;
  }
  
  /**\
  SymbolDirectedGraph implementation
  \**/
  
  template <typename S, typename N>
  typename const ALDGraph<N> & SymbolDirectedGraph<S,N>::GetAbstractGraph() const
  {
    return g_;
  }
  
  template <typename S, typename N, class H>
  typename const HashTable<S,N,H> & SymbolDirectedGraph<S,N>::GetSymbolMap() const
  {
    return s2n_;
  }
  
  template <typename S, typename N>
  typename const Vector<S> & SymbolDirectedGraph<S,N>::GetVertexMap() const
  {
    return n2s_;
  }
} // namespace fsu

#endif
