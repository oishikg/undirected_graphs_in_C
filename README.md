# Undirected Graphs in `C`

### The data structure

- This project implements an undirected graphical structure in C, in which
nodes store data values of types integer, float, or string.
- The structs used to define the graph and node data structures may be viewed
in the header file. We allow for isolated nodes (users are not forced to link
a new node to some node), but all nodes are kept track of via a 2-d array
which holds pointers to the nodes.
- Edges are implemented only between distinct nodes; self-referencing edges
are not allowed by our data type.

### Implementation

- The entire program is interactive, prompting the user to create graphs, and
choose 1 of the 14 options. Notable features about the implementation
of these functions is as follows:

   - We use mutliple helper function. create_edge, delete_edge, and edit_edge
   all have helper functions.
   - The graph traversal functions, to find the shortest path and distance,
   use breadth first search. The function implementating this, along with
   helper functions for it (namely, enqueue and dequeue functions) are also
   placed in the helper function section of the c file.
   - get_nodes_at_distance is implemented using recursion and a marked_node
   array to ensure that paths with repeating vertices (trails) did not occur.
   - The program is mostly resistant to input errors by running multiple
   checks on user inputs. As such, the error checking was not extensive
   because this is not a software development class. Two caveats must be
   mentioned in that regard:
   	     (i) only strings without spaces may be input
	     (ii) the file names for the last two functions must be precise,
	     or the program will SEGFAULT.

- The export_graph and load_graph function implemented a rudimentary
pseudo-XML schema. The schema is as follows:

    - `< >` : Holds the entire graph
    - `~ ~` : Holds primary information, i.e., number of nodes, and maxmimum nodes
      	  for graph.
    - `$ $` : Holds information about the nodes
    - `^ ^` : Holds infomration about the edges of a node; is always nested in
      	  the $ $ environment.
    - `# #` : Holds primary data and node data, e.g., if the number of nodes is
      	  20, it will represented as #20#. This allows the graph to
	  arbitrarily long sequences of data.
    - `* *` : Holds edge data, i.e., the references to the adjacent nodes of a
      	  given node. This is only contained in the ^ ^ environment.
	  
 Examples of exported graphs may be found in the zip folder that contains the executable and the
 C files.


## Getting Started


### Prerequisites

Any `C` compiler. I used Ubuntu's preinstalled `gcc`. 


### Installing

- Pass the `C` compiler the `midterm_oishik.c` file to build the executable file. 
- Run the executable to start user-interface loop. 



## Running the tests

See `tests` for sample `.xml` files which encode the graphs. These file paths may be passed to
the user loop on prompting for testing. 



## Authors

* **Oishik Ganguly**


