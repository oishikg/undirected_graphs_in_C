/* header file */


/* typedef for nodes, note that this definition is recursive */
typedef struct m_node
{
  /* void pointer is a placeholder for pointers of a concrete type, i.e., data of int* , float*, or char*. */
  void * data;
  /* int type represent the type of data eventually stored, 1: int, 2:float, 3:char*, 0:
   * uninitialized */
  int data_type;
  /* int uid is an id uniquely identifies the node; this helps visualize the graph */
  int uid;
  /* pointers to pointers of other nodes */
  struct m_node ** connected_nodes;
  /* int value to store the number of edges */
  int num_edges;
  /* int value to keep track of the maximum space for edges, 
   * , m_node is realloced when num_edges = max_edges */
  int max_edges;
} node;


/* typedef for graphs. only store the pointer to the root node,
 * since all other nodes are accessible from it */
typedef struct
{
  /* pointer to root node */
  node* root_node;
  /* double pointer to keep track of all nodes in the graph */
  node** all_nodes;
  /* number of nodes presently if graph */
  int num_nodes;
  /* maximum number of nodes in graph. this is updated when num_nodes = max_nodes
  * using realloc. */
  int max_nodes;
} graph;

/* helper function prototypes */
int check_validity_of_node (graph *g, int input_node1);
int check_validity_of_edge (graph* g, int input_node1, int input_node2);
void create_edge_helper (graph* g, int input1, int input2);
void delete_edge_helper (graph* g, int input_node1, int input_node2);
void enqueue (int* q, int el, int sizeofq);
int dequeue (int* q, int sizeofq);
int** bfs (graph* g, int source);
void print_shortest_path (int input1, int input2, int** edgeToArray, int stack_counter);

/* function prototypes */
void print_graph (graph *g);
graph* create_new_graph ();
node* create_node (graph* g , void* d, int data_type);
void add_node (graph *g);
void delete_node (graph *g);
void create_edge (graph *g);
void edit_node (graph *g);
void delete_edge (graph *g);
void edit_edge (graph *g);
void print_node (graph *g);
void distance_nodes(graph *g);
void print_path (graph *g);
void get_nodes_at_distance_helper (int distance, int uid,
				   int uid_prev, int* marked_nodes, graph* g,
				   int* uid_at_given_distance);
node** get_nodes_at_distance(int distance, graph *g);
void export_graph (graph *g, char *filename);
graph* load_graph (char* filename);
void delete_graph (graph *g);

/* function to create user interactivity */
void user_interaction_loop();

  



/* global variables */
int uid;
int counter_for_at_distance;
int counter_for_marked_nodes;
