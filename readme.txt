Bryan Tanner (B.T.): initiated repo on Bitbucket, created first draft of makefile
Alexander Kozhevnikov (A.K): Beginning work on symgraph.h
A.K.: Copied the project writeup class definitions into the relevant .h files as a start.
A.K.: Added SymbolDirectedGraph definition.
A.K.: Fixed makefile draft CC line to include $(incpath)
A.K.: Got template declarations right in symgraph.h
A.K.: Got template declarations fully right in symgraph.h (needed the hashmap returning functions properly declared still).
A.K.: Got constructors implemented in symgraph.h.
A.K.: Got SetVrtxSize and AddEdge implemented.
A.K.: Got VrtxSize and EdgeSize implemented.
Brian Hague (B.H.): Added multiple read protection and some relevant inclusions to moviematch.h
B.H.: Implemented InDegree, OutDegree, Begin, and End
B.H.: Wrote and implemented 4 functions (stated above) for SymbolDirectedGraph.
B.H.: Defined typedef for AdjIterator. File compilable on linprog.
A.K.: Impemented Push() (for both graph types). Use of rehash is VERY inefficient, for insert, but will help with read. Can be removed if needed.
A.K.: Added a size_ variable to efficiently keep track of size and not
inneficiently query the size from the existing containers. Logically this
means massive runtime increases in Push() and small ones elsewhere.
A.K.: Implemented Size() function to get size_, also fixed Push() to properly
handle already present keys.
A.K.: Fixed Push() even more properly.
A.K.: Fixed symgraph.h constructors (bug didn't surface until trying to
compile moviematch.h).
