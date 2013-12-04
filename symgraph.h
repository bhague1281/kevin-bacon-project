#include 

namespace fsu
{
  template < typename S , typename N >
  class SymbolGraph
  {
  public:
    typedef S      Vertex;
    typedef xxxxx  AdjIterator;

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
    ...
  private:
    ALUGraph<N>      g_;
    HashTable<S,N,H> s2n_;
    Vector<S>        n2s_;
    ...
  }; // class SymbolGraph
} // namespace fsu
