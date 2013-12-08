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
A.K.: Made helper functions in MovieMatch for string parsing.
B.T.: Committed a test program into the tree.
A.K.: Made makefile as it should be, given that moviematch.h and symgraph.h are
template headers. Left old versions of stuff commented out.
Added optional rehash operand to the setVrtxSize function, left size-only parameterized one as a wrapper around the new one.
Finished moviematch.h Load(), but can't test because compile errors.
A.K.: Fixed dumb typos and other mistakes that Brian pointed out.
B.H.: Began laying out MovieDistance, based on BFSurvey in project4.
B.H.: Added control structures, NextNeighbor function for operating MovieDistance. Still need to deal with going from actor to number and vice versa. Add functions in symgraph.h?
A.K.: Fixed compilation bugs along with understanding what the problem was (no object files for the code used from $LIB/cpp/ nor #include clauses for the .cpp used therein.
A.K.: Verified and tested the parsing of Load works correctly (didn't test if
it saved everything to database right).
A.K.: Patched up compile errors in the above contribution by B.H.
A.K.: Added "clean" to makefile to help delete old builds' files.
B.H.: Fixed multiple declaration error by removing xstring.cpp from kb.cpp (alternate solution provided by Alexander).
A.K.: Alternative 2 of the Load(). I think this works very well, unless I
missed something it seems to be very fast.
B.H.: Fixed bracket error occuring in MovieDistance. The search (in terms of functionality) works, but
the logic is still flawed. I'm guessing (?) it's due to the neighbor iterator being incorrect.
B.H.: Bracket error fixed by correctly initializing control structures (visited_, neighbor_, distance_).
A.K.: Finished up MovieDistance, ironed out bugs, ensured that non-existant
actors or ones with no link to Kevin Bacon are reported correctly. (Not
thoroughly tested.)
A.K.: Fixed the (by now refactored out from MovieDistance) search metadata
resetting code.
