/* ******** YSC 3217 ******** */
/* ******** Midterm Assignment: Graphs ******** */
/* ******** Student Details ******** */
/* Name : Oishik Ganguly 
 * Matric Number : A0138306J
 * Email : oishik.ganguly@u.yale-nus.edu.sg */

/* ******** Inclusions ******** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "midterm_oishik.h"
#include <unistd.h>


/* ******** Helper functions ******** */

/* function to check an input uid refers to a currently valid node */
int check_validity_of_node (graph *g, int input_node1)
{
  /* check if the input nodes are valid */
  if (input_node1< 0 || input_node1 >= uid)
    {
      printf (" The input UID %d do not exist. You will be redirected to start menu \n",
	      innput_node1);
      return 0;
    }


  /* null pointer references are not allowed */
  if ( (*g).all_nodes[input_node1] == NULL)
    {
      printf (" The nodes you are referring to have been deleted. You will be redirected to the start menu \n");
      return 0;
    }

  /* if all the tests are passed, return 1 */
  return 1;
}

/* function that checks, given the uids of two nodes, whether an edge between them
 * is possible */
int check_validity_of_edge (graph* g, int input_node1, int input_node2)
{
  int j;
  
  /* self-pointing edges are not allowed */
  if (input_node1 == input_node2)
      return 0;
  
  /* repetitions of edges are not allowed */
  for (j=0 ; j<(*g).all_nodes[input_node1][0].num_edges ; j++)
    {
      if ( (*g).all_nodes[input_node1][0].connected_nodes[j] == NULL)
	continue;
      if ((*g).all_nodes[input_node1][0].connected_nodes[j][0].uid == input_node2)
	  return -1; /* use -1 to indicate that edge exists */
    }

  /* if the tests are passed, return 1*/
  return 1;

}

/* helper function for create_edge */
void create_edge_helper (graph* g, int input1, int input2)
{
  /* declarations */
  node node1 , node2;
   
  node1 = (*g).all_nodes[input1][0];
  node2 = (*g).all_nodes[input2][0];
  

  /* check if there is enough space, otherwise reallocate memory */
  if (node1.num_edges == node1.max_edges)
    {
      (*g).all_nodes[input1][0].connected_nodes =
	(node**) realloc ( (*g).all_nodes[input1][0].connected_nodes ,
			   sizeof (node*) * (node1.max_edges + 10) );
      (*g).all_nodes[input1][0].max_edges += 10;
    }

  if (node2.num_edges == node2.max_edges)
    {
      (*g).all_nodes[input2][0].connected_nodes =
	(node**) realloc ( (*g).all_nodes[input2][0].connected_nodes ,
			   sizeof (node*) * (node2.max_edges + 10) );
      (*g).all_nodes[input2][0].max_edges += 10;
    }


  /* finally, add edges to the nodes */
  node1.connected_nodes [ node1.num_edges ] = (*g).all_nodes [input2];
  (*g).all_nodes [input1][0].num_edges += 1;
  /*  node1.num_edges += 1;*/

  node2.connected_nodes [node2.num_edges ] = (*g).all_nodes [input1];
  (*g).all_nodes [input2][0].num_edges += 1;
  /*node2.num_edges += 1;*/
  
  printf (" The edge %d <--------------> %d has succesfully been added to the graph \n \n", input1 , input2);
}


/* helper function for delete_edge */
void delete_edge_helper (graph* g, int input_node1, int input_node2)
{
  /* declarations */
  int i,j;
  node temp_node;

  /* delete edge */
  for (i=0 ; i< (*g).all_nodes[input_node1][0].num_edges ; i++)
    {
      if ( (*g).all_nodes[input_node1][0].connected_nodes[i] == NULL)
	continue;

      /* find pointer to input_node2 */
      if ( (*g).all_nodes[input_node1][0].connected_nodes[i][0].uid == input_node2)
	{
	  temp_node = (*g).all_nodes[input_node1][0].connected_nodes[i][0];
	  /* free reference to input_node1 in input_node2 */
	  for ( j=0 ; j<temp_node.num_edges ; j++)
	    {
	      if (temp_node.connected_nodes[j] == NULL)
		continue;

	      if (temp_node.connected_nodes[j][0].uid == input_node1)
		{
		  /* free (temp_node.connected_nodes [j]); */
		  temp_node.connected_nodes [j] = NULL;
		  break; /* no need to loop once pointer is freed */
		}
	    }

	  /* now free reference to input_node2 in input_node1 */
	  /* free ( (*g).all_nodes[input_node1][0].connected_nodes[i]); */
	  (*g).all_nodes[input_node1][0].connected_nodes[i] = NULL;
	  break; /* no need to loop any further */
	}
    }
}

/* helper funciton for bfs */
void enqueue (int* q, int el, int sizeofq)
{
  int i;

  /* assume that an empty spot in the queue has a value of -1 */
  for (i=0 ; i<sizeofq ; i++)
    {
      if (q[i] == -1)
	{
	  q[i] = el;
	  break;
	}
    }
}

/* helper function for bfs */
int dequeue (int* q, int sizeofq)
{
  int i;
  int el;

  /* set the first element to -1; this move is purely symbolic */
  el = q[0];
  q[0] = -1; 
  
  /* shift elements by 1 */
  for (i=0 ; i<sizeofq ; i++)
    {
      /* to avoid unnecessary shifting */
      if (q[i+1] == -1)
	{
	  q[i] = -1;
	  break;
	}
      else
	q[i] = q[i+1];
    }

  return (el);
}


/* helper function to implement breadth first search of graphs; returns the edgeTo array */
int** bfs (graph* g, int source)
{
  /* declarations */
  int i;
  int** edge_to_array;
  int* node_queue;
  int* marked_table;
  int current_node;
  int current_neighbour;
  
  /* initializations */
  edge_to_array = (int**) malloc (sizeof(int*) * (*g).num_nodes);
  for (i=0 ; i<(*g).num_nodes ; i++)
    edge_to_array[i] = NULL; /* to indicate that no edge has yet been associated */
    

  node_queue = (int*) malloc (sizeof(int) * (*g).max_nodes); /*+1 as a precautions */
  for (i=0 ; i<(*g).max_nodes ; i++)
    node_queue[i] = -1;

  marked_table = (int*) malloc (sizeof(int) * (*g).num_nodes); /*+1 as a precautions */
  for (i=0 ; i<(*g).num_nodes ; i++)
    marked_table[i] = 0;
  /* implement bfs algorithm */

  /* add source node to the top of the queue */
  marked_table[source] = 1; /* since the source is marked by default */
  enqueue (node_queue, source, (*g).max_nodes);

  /* next, iterate through the node queue */
  while (node_queue[0] != -1) /* this indicates empty queue */
    {
      /* print current queue for debugging */
      /*printf ("The current node queue is: \n"); 
      for (i=0; i< (*g).max_nodes; i++)
	printf ("%d \n", node_queue[i]);
	printf ("\n \n"); */
      
      /* remove the current node from the node queue */
      current_node = dequeue (node_queue, (*g).max_nodes);
     
      /* go through the edges */
      for (i=0 ; i<(*g).all_nodes[current_node][0].num_edges ; i++)
	{
	  /* check for NULL edges */
	  if ((*g).all_nodes[current_node][0].connected_nodes[i] == NULL)
	    continue;
	  
	  /* check if the node at other end of edge is marked or not */
	  current_neighbour = (*g).all_nodes[current_node][0].connected_nodes[i][0].uid;

	  /* if not marked, (i) mark it, (ii) add current node to edge_to_array, and
	   * (iii) add it to the queue */
	  if (marked_table[current_neighbour] == 0)
	    {
	      marked_table[current_neighbour] = 1;
	      edge_to_array[current_neighbour] = (int*) malloc (sizeof(int));
	      edge_to_array[current_neighbour][0] = current_node;
	      enqueue (node_queue, current_neighbour, (*g).max_nodes);
	      /* printf ("%d \n", current_neighbour); */
	    }
	}      
    }
  /* free resources */
  free (node_queue);
  free (marked_table);
  /* return the edge_to_array */
  return (edge_to_array);
}


/* helper function to print shortest path */
void print_shortest_path (int input1, int input2, int** edgeToArray, int stack_counter)
{
  
 
  if (input1 == input2)
    {
      printf ("%d---> ", input1);
      return;
    }
  else  if (edgeToArray[input2] == NULL)
    {
      printf ("No path exists between nodes of isolated components of graph. \n \n");
      return;
    }
  else
    {
      stack_counter ++;
      print_shortest_path (input1, edgeToArray[input2][0], edgeToArray, stack_counter);
      if (stack_counter == 1)
	printf ("%d ", input2);
      else
	printf ("%d---> ", input2);
    }
}



/* ******** Implementaion of graphs ******** */
/*void print_graph(graph *g); Prints the current graph from the root. Prints each node (with
  values) and edges (displayed in a text interface). Note: you may print redundant information. */

void print_graph (graph *g)
{
  /* declarations */
  int i,j;
  node* temp_node;
  printf (" <---------------------------------- CURRENT GRAPH ----------------------------------> \n");
  for (i=0 ; i<(*g).num_nodes ; i++)
    {
      /* Check for NULL pointers */
      if ((*g).all_nodes[i] == NULL)
	continue;
      else
	{
	  temp_node = (*g).all_nodes[i];
	  printf ("\n PRINTING NODE WITH UID %d: \n \n" , (*temp_node).uid );
	  /* print data value depending on data type */
	  switch ( (*temp_node).data_type )
	    {
	    case 1:
	      printf ("This node has an integer value \n");
	      /* to print the data, (i) case the void pointer to an int pointer
	       * and (ii) dereference the newly cast pointer to obtain the integer value */
	      printf ("The value stored in this node is: %d \n",
		      *(int*)(*temp_node).data);
	      break;
	    case 2:
	      printf ("This node has an float value \n");
	      printf ("The value stored in this node is: %f \n",
		      *(float*)(*temp_node).data);
	      break;
	    case 3:
	      printf ("This node has an string value \n");
	      printf ("The valfue stored in this node is: %s \n",
		      (char*)(*temp_node).data);
	      break;
	    }
	  
	  /* print edges */
	  printf (" Printing edges: \n");
	  for (j=0 ; j<(*temp_node).num_edges ; j++)
	    {
	      if ( (*temp_node).connected_nodes[j] == NULL)
		continue;
	      else
		printf ("%d <------> %d \n" , (*temp_node).uid ,
			(*temp_node).connected_nodes[j][0].uid);	      
	    }
	  /* print extra information */
	  printf ("Total number of edges (including deleted edges): %d \n" , (*temp_node).num_edges);
	  printf ("Maximum number of edges : %d \n" , (*temp_node).max_edges);	  
	}
    }
  printf (" <---------------------------------- END OF CURRENT GRAPH ----------------------------------> \n ");
  printf (" \n \n");
}


/* (1) create_new_graph () function */


graph* create_new_graph ()
{
  /* declarations */
  graph* new_graph;
  int i;
  
  /* allocating memory for new graph */
  printf (" Creating new graph ... \n ");
  new_graph = (graph*) malloc (sizeof (graph));
  
  /* allocate memory for root node */
  /*  (*new_graph).root_node = (node*) malloc (sizeof (node)); */
  /* the above malloc is unnecessary */
  (*new_graph).root_node = NULL;

  /* allocate memory to keep track of all nodes in graph */
  (*new_graph).all_nodes = (node**) malloc (sizeof (node*) * 20);
  for (i=0 ; i<5 ; i++)
    (*new_graph).all_nodes[i] = NULL;
  (*new_graph).num_nodes = 0; /* new graphs do not have any nodes in them */
  (*new_graph).max_nodes = 20;
  return (new_graph);
}


/* (2) create_node function to create node */

node* create_node (graph* g , void* d, int data_type)
{
  /* declarations */
  node* new_node;
  int i;
  
  new_node = (node*) malloc (sizeof (node)); 
  printf (" Creating new node ... \n ");
  
  /* add pointer to data value, the void* pointer can take the form of any type of pointer,
   * see header file for integer to data type mapping  */
  switch (data_type)
    {
    case 1:
      (*new_node).data = (int*) malloc (sizeof (int));
      *(int*)(*new_node).data = *(int*) d;
      break;
    case 2:
      (*new_node).data = (float*) malloc (sizeof (float));
      *(float*)(*new_node).data = *(float*) d;
      break;
    case 3:
      (*new_node).data = (char*) malloc (sizeof (char) * 256);
      strcpy ((char*)(*new_node).data, (char*) d);    
      break;
    }
  (*new_node).data_type = data_type;
  
  /* add uid, uid is a global variable */
  printf ("%d \n" , uid);
  (*new_node).uid = uid;
  uid++;
  
  /* allocate memory for pointers to neighbouring nodes */
  (*new_node).connected_nodes = (node**) malloc (sizeof (node*) * 10);
  for (i=0 ; i<10 ; i++)
    (*new_node).connected_nodes[i] = NULL;
  
  /* configure values keeping track of edges */
  (*new_node).num_edges = 0;
  (*new_node).max_edges = 10;
  
  /* return pointer to new node */
  return (new_node); 
}



/* (3) void add_node(graph *g); Adds a node to a graph. This function will prompt the user for
   any important information, such as: data type [either char *, int, float] and value to input
   from the command line. If the node is the first one in the graph, then it becomes the root. */

void add_node (graph *g)
{
  /* declarations */
  int type_flag; /* set 1 for integer, 2 for float, 3 for string */
  int input; /* for user input */
  int rv_on_scanf; /* to detect faulty input */
  node* new_node;
  int* integer_input;
  float* float_input;
  char* string_input;

  /* initialize pointers to NULL */
  integer_input = NULL;
  float_input = NULL;
  string_input = NULL;

  /* set flag to 0, only update after type has been input */
  type_flag = 0;
  input = 0;
  
  /* ask for user input */

  input = 0;
  /* determine type of input */
  printf (" Adding node to graph ... \n ");
  printf (" Enter 1 if you would like to input an integer \n ");
  printf (" Enter 2 if you would like to input a float \n ");
  printf (" Enter 3 if you would like to input a string \n ");
  rv_on_scanf = scanf ("%d" , &input);

  if (rv_on_scanf == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input was incorrect. You will be redirected to start menu \n");
      return;
    }
  /* ask for appropriate input depending on indicated type */
  switch (input)
    {
      /* integers */
    case 1:
      /* type cast pointer first */
      integer_input = (int*) malloc (sizeof(int));
      printf ("Enter integer value \n");
      /* the pointer value is the address of the memory location */
      rv_on_scanf = scanf ("%d" , integer_input);
      type_flag = 1;
      break;
	  
      /* floating values */
    case 2:
      /* type cast pointer first */
      float_input =  (float*) malloc (sizeof(float));
      printf ("Enter float value \n");
      rv_on_scanf = scanf ("%f" , float_input);
      type_flag = 2;
      break;
	  
      /* string values */
    case 3:
      /* type cast pointer first */
      string_input = (char *) malloc (sizeof(char) * 256);
      printf ("Enter string value \n");
      /* the pointer value is the address of the memory location */
      rv_on_scanf = scanf ("%s" , (string_input));
      type_flag = 3;
      break;
      /* in case of faulty input */
    default:
      break;
    }
  
  if (rv_on_scanf  == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf ("Your input type was incorrect. Please choose the correct option/ enter the correct type of value. You will now be redirected to the main menu again. \n \n");
      free (integer_input);
      free (float_input);
      free (string_input);
      return;
    }
  
  printf ("The node was successfully created \n");

  /* initialize new node */
  /*new_node = (node*) malloc (sizeof (node)); */
  /* this is not required since the malloc is done in create_node */
   
  /* next, create new node with user input data */
  switch (type_flag)
    {
    case 1:
      new_node = create_node (g , integer_input, 1);
      free (integer_input);
      break;
    case 2:
      new_node = create_node (g , float_input, 2);
      free (float_input);
      break;
    case 3:
      new_node = create_node (g , string_input, 3);
      free (string_input);
      break;
    }
	
  /* finally, add the node to the graph */

  /* if root node has not been established */
  if ((*g).num_nodes == 0)
    (*g).root_node = new_node;
  
  /* check if space has been exceeded or not */
  if ((*g).num_nodes == (*g).max_nodes)
    {
      /* reallocate memory */
      (*g).all_nodes = (node**) realloc ((*g).all_nodes ,
					 (sizeof (node*) * ((*g).max_nodes + 10)));
      /* update max_nodes */
      (*g). max_nodes += 10;					 
    } 

  /* with root and space checked, proceed to add node to the all_nodes array */
  (*g).all_nodes[(*g).num_nodes] = new_node;
  (*g).num_nodes ++;
}


/* void delete_node(graph *g); Prompts the user to choose a node to delete. This function
 * should display a list of nodes, the user should choose one to delete.
 * Note that deleting a node means that you also need to delete the edges.*/

void delete_node (graph *g)
{
  /* declarations */
  int i,j;
  node temp_node;
  int input_uid;

  input_uid = -1;
  printf ("Displaying list of nodes and the correspoding data value: \n");

  for ( i=0 ; i<(*g).num_nodes ; i++)
    {
      /* check for null pointers indicating deleted nodes */
      if ( (*g).all_nodes[i] == NULL)
	continue;
      
      temp_node = (*g).all_nodes[i][0];
      switch (temp_node.data_type)
	{
	case 1:
	  printf ("UID : %d   ;   Data Value = %d \n" , temp_node.uid , *(int*) temp_node.data);
	  break;
	case 2:
	  printf ("UID : %d   ;   Data Value = %f \n" , temp_node.uid , *(float*) temp_node.data);
	  break;
	case 3:
	  printf ("UID : %d   ;   Data Value = %s \n" , temp_node.uid , (char*) temp_node.data);
	  break;  
	}
    }
  printf (" \n \n");
  printf (" Input the UID of the node you want to delete \n");
  if (scanf ( "%d" , &input_uid) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }
  
  /* check for correct input */
  if (input_uid < 0 || input_uid >= (*g).num_nodes)
    {
      printf (" The UID you have chosen does not exist. You will be redirected to the menu.\n ");
      return;
    }

  /*finally, check to see if node exists or is deleted */
  if ( (*g).all_nodes[input_uid] == NULL)
    {
      printf (" The node you have chosen has already been deleted. You will be redirected to the start menu \n.");
      return;
    }

  /* if input is correct, proceed to remove node by (i) deleting all the edges,
   * (ii) freeing the pointers pointing to the edges, (iii) freeing data value, and 
   * (iv) setting the node pointer to NULL */

  temp_node = (*g).all_nodes [input_uid][0];

  /* delete neighbour's references to node; to do so
   * (i) loop through all the connected nodes
   * (ii) loop through the pointers to the neighbours' neighbours
   * (iii) when the pointer to the current node is encountered, NULLIFY but 
   *       do not delete the pointer; this pointer points to the memory location
   *       that stores the present node, and freeing it will lead to double freeing. */
  for (j=0 ; j< temp_node.num_edges ; j++)
    {

      if (temp_node.connected_nodes[j] == NULL)
	continue;

      
      for (i=0 ; i<temp_node.connected_nodes[j][0].num_edges ; i++)
	{
	  if (temp_node.connected_nodes[j][0].connected_nodes[i] == NULL)
	    continue;
	    
	  /* don't free the pointer from the neighbour to the present node, this is
	   * already being done */	  
	  if (temp_node.connected_nodes[j][0].connected_nodes[i][0].uid
	      == temp_node.uid)		
	    temp_node.connected_nodes[j][0].connected_nodes[i] = NULL;
	}
      /* don't free the pointer to the neighbouring node; this will end up deleting the node
       * itself, because the pointer (in addition to the main pointer in the graph.all_nodes 
       * data value) also points to the memory location that stores the node*/
    } 

  /* free pointer to pointers to neighboring nodes */
  free (temp_node.connected_nodes);
  temp_node.connected_nodes = NULL;
  
  /* free data value */
  free (temp_node.data);
  temp_node.data = NULL;

  /* finally, free pointer to this node, and set its value to NULL */
  free ((*g).all_nodes [input_uid]);
  (*g).all_nodes [input_uid] = NULL;

  /* note that by freeing the pointer to the node, references to it in all other 
   * nodes point to NULL. The edges are effectively deleted */
  printf ("The node is deleted \n");
}


/* (3) void create_edge(graph *g); Creates an edge between two nodes. Note: each node should
   store the edge. Therefore each edge is stored on two nodes. */

void create_edge (graph *g) 
{ 
  /* declarations */
  int input1 , input2; 
  
  input1 = -1;
  input2 = -1;
  
  printf ("Displaying current graph: \n \n");
  print_graph (g);
  printf (" Please choose the nodes between which you want an edge, and enter their UIDs. You will be prompted twice. \n");

  /* accept inputs */
  printf (" Please enter the UID for the first node of the edge \n");
  if (scanf ("%d", &input1) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  printf ("Please enter the UID for the second node of the edge \n");
  if (scanf ("%d", &input2) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  /* check input nodes */
  if (check_validity_of_node (g, input1) == 0 || check_validity_of_node (g, input2) == 0)
    return;

  /* check if edge is valid or not */
  if (check_validity_of_edge (g, input1, input2) == 0)
    {
      printf (" An edge cannot be established between the same node. You will be redirected to the start menu \n");
      return;
    }
  
  if (check_validity_of_edge (g, input1, input2) == -1)
    {
      printf (" The edge you are trying to establish already exists. You will be redirected to the start menu \n");
      return;
    }

  /* the input values are now good to use, creating edge between nodes */
  create_edge_helper (g, input1, input2);
} 
  



/*void edit_node(graph *g); Allows the user to choose a node from a list, and change its
  value. */

void edit_node (graph *g)
{
  /* declarations */
  int i, input_uid, modified_input_datatype;
  node temp_node;
  int* modified_int_input;
  float* modified_float_input;
  char* modified_string_input;

  input_uid = -1;
  modified_input_datatype = 0;
  
  printf ("Displaying list of nodes and the correspoding data value: \n");

  for ( i=0 ; i<(*g).num_nodes ; i++)
    {
      /* check for null pointers indicating deleted nodes */
      if ( (*g).all_nodes[i] == NULL)
	continue;
      
      temp_node = (*g).all_nodes[i][0];
      switch (temp_node.data_type)
	{
	case 1:
	  printf ("UID : %d   ;   Data Value = %d \n" , temp_node.uid , *(int*) temp_node.data);
	  break;
	case 2:
	  printf ("UID : %d   ;   Data Value = %f \n" , temp_node.uid , *(float*) temp_node.data);
	  break;
	case 3:
	  printf ("UID : %d   ;   Data Value = %s \n" , temp_node.uid , (char*) temp_node.data);
	  break;  
	}
    }
  printf (" \n \n");
  printf (" Input the UID of the node you want to modify \n");
  if (scanf ( "%d" , &input_uid) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }
  
  /* check for correct input */
  if (check_validity_of_node (g, input_uid) == 0)
    return;
  
  /* now proceed to modify node */
  temp_node = (*g).all_nodes[input_uid][0];
  printf (" The node you have selected has the following parameters: \n");
  switch (temp_node.data_type)
    {
    case 1:
      printf ("UID : %d   ;   Data Value = %d \n" , temp_node.uid , *(int*) temp_node.data);
      break;
    case 2:
      printf ("UID : %d   ;   Data Value = %f \n" , temp_node.uid , *(float*) temp_node.data);
      break;
    case 3:
      printf ("UID : %d   ;   Data Value = %s \n" , temp_node.uid , (char*) temp_node.data);
      break;  
    }

  printf (" You must first select the datatype for the new node value.\n");
  printf (" Enter 1 for integer, 2 for float, or 3 for string \n");
  if (scanf ("%d" , &modified_input_datatype) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  if (modified_input_datatype < 1 || modified_input_datatype > 3)
    {
      printf ("You have made an incorrect choice. You will now be redirected to start menu \n");
      return;
    }

  /* ask for corresponending data type input */
  switch (modified_input_datatype)
    {
    case 1:
      modified_int_input = (int*) malloc (sizeof(int));
      printf ("Enter the integer data to modify node: \n");
      if (scanf ("%d", modified_int_input) == 0)
	{
	  while (fgetc(stdin) != '\n'); /* Read until a newline is found */
	  printf (" The input format was incorrect. You will be redirected to start menu \n");
	  return;
	}
      /* note: when we assigned temp_node = (*g).all_nodes[input_uid][0], a copy of the
       * node in heap was made in the operating stack for the function. when accessing
       * pointers in the node, i.e., the data and connected_nodes pointers, temp_node.data is
       * the memory location of the data value in the heap. thus, modifying the contents of
       * temp_node.data (e.g. by freeing the memory) has an effect on the actual graph as well.
       * However, when we alloc memory again, note that the malloc function returns a void
       * pointer to the newly allocated memory in the heap, except, this pointer value
       * is stored by temp_node.data WHICH IS A LOCAL VARIABLE ON THE STACK. the original
       * pointer to the node does not point to this memory location! this is why we cannot 
       * use the alias when mallocing after freeing the memory. */
      free (temp_node.data);
      (*g).all_nodes[input_uid][0].data = malloc (sizeof(int));
      *(int*) (*g).all_nodes[input_uid][0].data = *modified_int_input; 
      free (modified_int_input);
      (*g).all_nodes[input_uid][0].data_type = 1;
      break;
    case 2:
      modified_float_input = (float*) malloc (sizeof (float));
      printf ("Enter the float data to modify node: \n");
      if (scanf ("%f", modified_float_input) == 0)
	{
	  while (fgetc(stdin) != '\n'); /* Read until a newline is found */
	  printf (" The input format was incorrect. You will be redirected to start menu \n");
	  return;
	}
      free (temp_node.data);
      (*g).all_nodes[input_uid][0].data =  malloc (sizeof (float));
      *(float*) (*g).all_nodes[input_uid][0].data = *modified_float_input;
      free (modified_float_input);
      (*g).all_nodes[input_uid][0].data_type = 2;
      break;
    case 3:      
      modified_string_input =  malloc (sizeof(char) * 256);
      printf ("Enter the string data to modify node: \n");
      if (scanf ("%s", modified_string_input) == 0)
	{
	  while (fgetc(stdin) != '\n'); /* Read until a newline is found */
	  printf (" The input format was incorrect. You will be redirected to start menu \n");
	  return;
	}
      free (temp_node.data);
      (*g).all_nodes[input_uid][0].data = (char*) malloc (sizeof (char) * 256);
      strcpy ((char*) (*g).all_nodes[input_uid][0].data, modified_string_input);
      free (modified_string_input);
      printf ("Testing string value %s\n" , (char*)temp_node.data);
      (*g).all_nodes[input_uid][0].data_type = 3;

      break;
      
    }

  printf (" The node %d has been modified. \n \n", input_uid);


  
}

/* void delete_edge(graph *g); Prompts the user to choose a given edge, and deletes it.*/

void delete_edge (graph *g)
{
  /* declarations */
  int input_node1, input_node2;

  /* ask user to choose nodes */
  printf (" An edge is of the form: node1 <--------> node2 \n where node1 and node2 are nodes in the graph\n");
  printf ("Enter UID of node1 of the edge you want to delete: \n");
  if (scanf ("%d" , &input_node1) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }
  printf ("Enter UID of node2 of the edge you want to modify: \n");
  if (scanf ("%d" , &input_node2) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  /* error check inputs using function */

  /* check validity of nodes */
  if ( check_validity_of_node (g, input_node1) == 0 ||
       check_validity_of_node (g, input_node2) == 0)
    return;

  /* check validity of edge */
  if (input_node1 == input_node2)
    {   
      printf (" An edge cannot exist between the same node. You will be redirected to the start menu \n");
      return;    
    }
  /* a return value of -1 indicates that the edge exists in the graph*/

  if ( check_validity_of_edge (g, input_node1 , input_node2) != -1 )
    {
      printf (" The edge you are trying to delete does not exist in the graph. You will be redirected to the start menu \n");
      return;
    }

  /* call helper function to delete edge */
  delete_edge_helper (g, input_node1 , input_node2);
  
  printf (" The edge %d <--------> %d has been deleted \n \n", input_node1 , input_node2);
  
}


/* void edit_edge(graph *g); Allows the user to edit a given edge, e.g. changing the edge
   connecting node 1 to node 2 to an edge connecting node 2 to node 3 instead. */

void edit_edge (graph *g)
{
  /* declarations */
  int input_node1, input_node2, base_node, new_node;

  printf ("You have chosen to edit edges. We will now print the graph for you");
  printf ("Printing graph: \n");
  print_graph (g);

  /* ask user to choose nodes */
  printf (" An edge is of the form: node1 <--------> node2 \n where node1 and node2 are nodes in the graph\n");
  printf ("Enter UID of node1 of the edge you want to modify: \n");
  if (scanf ("%d" , &input_node1) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }
  printf ("Enter UID of node2 of the edge you want to modify: \n");
  if (scanf ("%d" , &input_node2) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  /* error check inputs using function */

  /* check validity of nodes */
  if ( check_validity_of_node (g, input_node1) == 0 ||
       check_validity_of_node (g, input_node2) == 0)
    return;

  /* check validity of edge; if the edge exists, -1 is returned */
  if ( check_validity_of_edge (g, input_node1 , input_node2) != -1)
    return;
  
  
  /* with the checks complete, we now determine the modification required */
  printf (" You may modify the edge by choosing either node1 or node2 as the base, and subsequently reconnecting it to another node. For example, if the edge you have selected is node1 <-----> node2,and you want to modify it to obtain node2 <-----> node3, then select node2 as the base node when prompted, and select node3 as the new connection node. \n");
  printf (" Choose your base node by inputting its UID: \n");
  if (scanf ("%d" , &base_node) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  if (base_node != input_node2 && base_node != input_node1)
    {
      printf (" You have input an incorrect base node. Returning to start menu. \n");
      return;
    }

  printf (" Choose the node you want to connect your base node to by entering its UID: \n");
  if (scanf ("%d" , &new_node) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  /* now check to see if the pair of nodes chosen is valid or not */
  
  /* check validity of nodes */
  if ( check_validity_of_node (g, base_node) == 0 ||
       check_validity_of_node (g, new_node) == 0)
    return;

  /* check validity of edge; this time, we want to make a new edge, so rv should be 1 */
  if ( check_validity_of_edge (g, base_node , new_node) != 1)
    return;

  /* edit the edge by (i) creating the new edge and (ii) deleting the new edge */

  /* we already have two functions that do this; let us use them */
  printf (" creating new edge.. \n");
  create_edge_helper (g, base_node, new_node);
  printf (" removing old edge..\n");
  delete_edge_helper (g, input_node1, input_node2);

  printf (" The edge %d <----------> %d as been modified to \n:" , input_node1 , input_node2);
  printf (" %d <---------------> %d \n \n", base_node , new_node);

}



/* void print_node(graph *g); Prompts the user to choose a node. The node is printed with
   every relevant information (value and edges, i.e. connection to over nodes).*/

void print_node (graph *g)
{
  /* declarations */
  int i;
  int uid_input;
  int flag ;

  flag = 0;

  /* get user input */
  printf ( "If not nodes are displayed, your graph is probably empty. Add nodes to it in that case. \n");
  printf ( "Displaying the nodes available: \n");
  for (i=0; i< (*g).num_nodes ; i++)
    {
      if ((*g).all_nodes[i] == NULL)
	continue;
      else
	printf ("The node with UID %d \n", (*g).all_nodes[i][0].uid);
    }

  printf (" Please choose node you would like to print: \n ");
  if (scanf ("%d" , &uid_input) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  /* check if the input is correct */
  for (i=0; i< (*g).num_nodes ; i++)
    {
      if ((*g).all_nodes[i] == NULL)
	continue;
      else
	{
	  if ((*g).all_nodes[i][0].uid == uid_input)
	    flag = 1;
	}
    }

  if (flag == 0)
    {
      printf (" You have entered an invalid UID. Returning to start menu...\n ");
      return;
    }

  /* with the input verified, proceed to print the node */
  printf ("Printing the node with UID %d \n", uid_input);
  printf ("\n \n");
  printf ("<-------------------- NODE %d --------------------> \n", uid_input);

  /* print data type and value */
  printf ("Data type: %d \n", (*g).all_nodes[uid_input][0].data_type);
  
  switch ((*g).all_nodes[uid_input][0].data_type)
    {
    case 1:
      printf ("Data: %d \n", *(int*)(*g).all_nodes[uid_input][0].data);
      break;
    case 2:
      printf ("Data: %f \n", *(float*)(*g).all_nodes[uid_input][0].data);
      break;
    case 3:
      printf ("Data: %s \n", (char*)(*g).all_nodes[uid_input][0].data);
    }

  /* print edges */
  printf (" Edges:  \n");
  for (i=0 ; i<(*g).all_nodes[uid_input][0].num_edges ; i++)
    {
      if ( (*g).all_nodes[uid_input][0].connected_nodes[i] == NULL)
	continue;
      else
	printf ("%d <------> %d \n" , (*g).all_nodes[uid_input][0].uid ,
		(*g).all_nodes[uid_input][0].connected_nodes[i][0].uid);	      
    }

  /* print meta information */
  printf ("Meta information:\n");
  printf ("Number of edges (incl deleted edges): %d \n", (*g).all_nodes[uid_input][0].num_edges);
  printf ("Memory unit allocted for edges: %d \n", (*g).all_nodes[uid_input][0].max_edges);

  printf (" <------------------------------------------------------------> \n \n");
}



/* void distance_nodes(graph *g); Computes the shortest distance between two nodes.
   Prompts the user to select two nodes, then computes the shortest distance between them. */

void distance_nodes(graph *g)
{
  /* declarations */
  int input1 , input2;
  int** edgeToArray;
  int distance;
  int i;
  
  input1 = -1;
  input2 = -1;

  printf ("You have chosen to compute the shortest distance between two nodes. \n");
  printf ("Displaying current graph: \n \n");
  print_graph (g);
  printf (" Please choose the nodes between which you want the shortest distance, and enter their UIDs. You will be prompted twice. \n");

  /* accept inpnuts */
  printf (" Please enter the UID for the first node. \n");
  if (scanf ("%d", &input1) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  printf ("Please enter the UID for the second node. \n");
  if (scanf ("%d", &input2) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  /* check input nodes */
  if (check_validity_of_node (g, input1) == 0 || check_validity_of_node (g, input2) == 0)
    return;

  /* with the checks complete, compute shortest distance */

  /* we use a BFS algorithm for this, it conveniently gives us the shortest distances
   * between a gives source and all vertices */
  
  /* obtain edgeTo array for a given vertex */
  edgeToArray = bfs (g, input1);

  /* calculate distance from input1 node to input2 node using the edgeToArray */
  distance = 0;

  /* we may be trying to find the distance between two isolated components */
  if (edgeToArray[input2] == NULL)
    {
      printf (" You were trying to find the distance between two isolated components. This pat does not exist and so its distance cannot be computed. \n \n");
    }
  else
    {
      for (i = input2; i!= input1 ; i=edgeToArray[i][0])
	distance++;
  
      printf ("The distance between %d and %d is %d units \n", input1, input2, distance);
      printf ("\n \n");
    }
  
  /* free resources */
  for (i=0 ; i<(*g).num_nodes ; i++)
    {
      if (edgeToArray[i] == NULL)
	continue;
      else
	{
	  free (edgeToArray[i]);
	  edgeToArray[i] = NULL;
	}
    }
  free (edgeToArray);
}


/* void print_path(graph *g); Prompts the user to select two nodes and prints the shortest
   path between them, with each intermediate node (and the value of each node). */

void print_path (graph *g)
{
  /* declarations */
  int i;
  int input1 , input2;
  int** edgeToArray;
  
  input1 = -1;
  input2 = -1;

  printf ("You have chosen to compute the shortest path between two nodes. \n");
  printf ("Displaying current graph: \n \n");
  print_graph (g);
  printf (" Please choose the nodes between which you want the shortest path, and enter their UIDs. You will be prompted twice. \n");

  /* accept inpnuts */
  printf (" Please enter the UID for the first node. \n");
  if (scanf ("%d", &input1) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input formatwas incorrect. You will be redirected to start menu \n");
      return;
    }

  printf ("Please enter the UID for the second node. \n");
  if (scanf ("%d", &input2) == 0)
    {
      while (fgetc(stdin) != '\n'); /* Read until a newline is found */
      printf (" The input format was incorrect. You will be redirected to start menu \n");
      return;
    }

  /* check input nodes */
  if (check_validity_of_node (g, input1) == 0 || check_validity_of_node (g, input2) == 0)
      return;


  /* finally, compute shortest path */
  edgeToArray = bfs (g, input1);

  /* print this using helper function */
  print_shortest_path (input1, input2, edgeToArray, 0);
  printf ("\n \n");

  /* free resources */
  for (i=0 ; i<(*g).num_nodes; i++)
    {
      if (edgeToArray[i] == NULL)
	continue;
      else
	{
	  free (edgeToArray[i]);
	  edgeToArray[i] = NULL;
	}
    }
  free (edgeToArray);
}



/* helper function for get_nodes_at distance */

void get_nodes_at_distance_helper (int distance, int uid,
				   int uid_prev, int* marked_nodes, graph* g,
				   int* uid_at_given_distance)
{
  /* declarations */
  int i,j;
  int flag;
  int flag_for_marked_nodes;
  int counter_for_marked_nodes_value_in_present_stack_frame;

  /* base case */
  if (distance == 0)
    {
      /* printf ("We've reached distance 0! \n"); */
      /* check whether uid already exists or not */
      flag = 0;
      for (i=0 ; i<counter_for_at_distance ; i++)
	{
	  if (uid_at_given_distance[i] == uid)
	    {
	      flag = 1;
	      break;
	    }
	  else
	    continue;
	}

      /* only if there are no repetitions, add the node uid to the array */
      if (flag == 0)
	{
	  printf ("The uid at the required distance is %d \n \n", uid);
	  uid_at_given_distance [counter_for_at_distance++] = uid;
	}
      
      return;
    }

  else
    {
      /* for debugging */

      /* mark current nodes */
      marked_nodes[counter_for_marked_nodes++] = uid;
      
      /* iterate through the edges */
      for (i=0; i<(*g).all_nodes[uid][0].num_edges ; i++)
	{
	  printf ("The distance is %d \n", distance);
	  printf ("The root uid is %d \n", uid);
	  printf ("The edge uids are: \n");

	  if ((*g).all_nodes[uid][0].connected_nodes[i] == NULL) 
	    continue;
	  /* set uid_prev to correct value */
	  if (uid_prev == -1)
	    uid_prev = 0;
	  /* check whether any of the current neighbours are marked or not */
	  flag_for_marked_nodes = 0;
	  for (j=0 ; j<counter_for_marked_nodes; j++)
	    {
	      if ((*g).all_nodes[uid][0].connected_nodes[i][0].uid == marked_nodes[j])
		{
		  flag_for_marked_nodes = 1;
		  break;
		}
	      else
		continue;
	    }

	  if (flag_for_marked_nodes == 1)
	    continue;
	  else
	    {
	      /* printf (" The edge UID: %d \n", (*g).all_nodes[uid][0].connected_nodes[i][0].uid);
		 printf (" Recursing...\n \n"); */
	      counter_for_marked_nodes_value_in_present_stack_frame = counter_for_marked_nodes;
	      
	      get_nodes_at_distance_helper
		(distance-1, (*g).all_nodes[uid][0].connected_nodes[i][0].uid, uid,
		 marked_nodes, g, uid_at_given_distance);

	      /* once the function returns, reset the marked_nodes array for reuse */
	      for (j=counter_for_marked_nodes_value_in_present_stack_frame ;
		   j< (*g).num_nodes ; j++)
		marked_nodes[j] = -1;
		
	    }
	}
      printf ("\n");
      return;
    }
  
}




/*  nodes ** get_nodes_at_distance(int distance, graph *g); Returns all the nodes located 
 * at a distance distance from the root of the graph. Consider any distance (not only the
 * shortest distances between two nodes): as long as there is a path that goes from the
 * root to this node with a distance of exactly distance. Note that a path cannot cover 
 * the same edge twice.*/

node** get_nodes_at_distance(int distance, graph *g)
{
  /* declarations */
  int i;
  node** nodes_at_given_distance;
  int* uid_at_given_distance;
  int* marked_nodes;

  
  /* initialization */
  nodes_at_given_distance = (node**) malloc (sizeof(node*) * (*g).num_nodes);
  /*  for (i=0 ; i< (*g).num_nodes ; i++)
      nodes_at_given_distance = (node**) malloc (sizeof(node)); */
  

  uid_at_given_distance = (int*) malloc (sizeof(int) * (*g).num_nodes);

  for (i=0 ; i< (*g).num_nodes ; i++)
    uid_at_given_distance[i] = -1;

  marked_nodes = (int*) malloc (sizeof(int) * (*g).num_nodes);

  for (i=0 ; i< (*g).num_nodes ; i++)
    uid_at_given_distance[i] = -1;
    
  counter_for_at_distance = 0;
  counter_for_marked_nodes = 0;

  /* set pointers to NULL */
  for (i=0 ; i< (*g).num_nodes ; i++)
    nodes_at_given_distance[i] = NULL;
  
  /* check if graph is empty or not */
  if ((*g).num_nodes == 0)
    {
      printf ("This graph has no nodes or edges. \n");
      free (uid_at_given_distance);
      return (nodes_at_given_distance);
    }

  /* find nodes at given distance */
  /* since this is best done recursively, we use a helper function */
  get_nodes_at_distance_helper (distance, 0, -1, marked_nodes, g, uid_at_given_distance);

  /* process uids */
  for (i=0 ; i<(*g).num_nodes ; i++)
    {
      if (uid_at_given_distance[i] == -1)
	break;
      else
	nodes_at_given_distance[i] = (*g).all_nodes[uid_at_given_distance[i]];
    }

  /*for (i = 0; i< (*g).num_nodes ; i++)
    printf("%d \n", uid_at_given_distance[i]);*/
  
  /* free resources */
  free (uid_at_given_distance);
  free (marked_nodes);

  return (nodes_at_given_distance);
}




/* void export_graph(graph *g, char *filename); Exports the current graph as a
   XML file containing all relevant information about the current graph (node and edges) to 
   the filename file */

void export_graph (graph *g, char *filename)
{
  /* declarations */
  FILE* file_pointer;
  int i, j;
  
  /* open file */
  file_pointer = fopen (filename , "w"); /* create a new xml file in write only mode */

  /* write graph to file as XML */

  /* open graph with < */
  fprintf (file_pointer , "<");

  /* store graph metadata */
  fprintf (file_pointer , "~"); /* ~ is wrapper for metadata */
  fprintf (file_pointer , "#%d#" , (*g).max_nodes); /* store max_nodes first to alloc data */
  fprintf (file_pointer , "#%d#" , (*g).num_nodes);
  fprintf (file_pointer , "~");
  /* end of metadata */

  /* iterate through nodes */
  for ( i=0 ; i < (*g).max_nodes ; i++)
    {
      /*start of node i data */
      fprintf (file_pointer , "$");
      /* if node is a NULL pointer */
      if ( (*g).all_nodes[i] == NULL)
	fprintf (file_pointer , "#X#");
      /* if node is not a NULL pointer */
      else
	{
	  /* store data type */
	  fprintf (file_pointer , "#%d#" , (*g).all_nodes[i][0].data_type); 
	  /* store data */
	  switch ( (*g).all_nodes[i][0].data_type )
	    {
	    case 1:
	      fprintf (file_pointer , "#%d#" ,*(int*) (*g).all_nodes[i][0].data);
	      break;
	    case 2:
	      fprintf (file_pointer , "#%f#" ,*(float*) (*g).all_nodes[i][0].data);
	      break;
	    case 3:
	      fprintf (file_pointer , "#%s#" , (char*) (*g).all_nodes[i][0].data);
	      break;
	    }
	  /* store uid */
	  fprintf (file_pointer , "#%d#" , (*g).all_nodes[i][0].uid);
	  /* store num_edges */
	  fprintf (file_pointer , "#%d#" , (*g).all_nodes[i][0].num_edges);
	  /* store max_edges */
	  fprintf (file_pointer , "#%d#" , (*g).all_nodes[i][0].max_edges);
	  /* start of edge data */
	  fprintf (file_pointer , "^");
	  /* store uids of edges, use X if NUll */
	  for (j=0 ; j<(*g).all_nodes[i][0].max_edges ; j++)
	    {
	      if ((*g).all_nodes[i][0].connected_nodes[j] == NULL)
		fprintf (file_pointer , "*X*" );
	      else
		fprintf (file_pointer , "*%d*" ,
			 (*g).all_nodes[i][0].connected_nodes[j][0].uid );
	    }
	  fprintf (file_pointer , "^");
	  /* end of edge data */
	}
      /* end of node i data */
      fprintf (file_pointer , "$");
    }

  /* close graph with > */
  fprintf (file_pointer , ">");
  /* done writing to graph */
  
  /* close file */
  fclose (file_pointer);

  printf (" The graph has been written to the XML file %s \n \n" , filename);
      
}

/* graph * load_graph(char * filename); Imports a graph from the file filename given as
   an argument. Returns NULL if the file is not an XML file correctly formatted. */

graph* load_graph (char* filename)
{
  /* delclarations */
  graph* new_graph;
  int new_node;
  int i,j; /* loop variables */
  char c; /* current character */
  int num_nodes;
  int max_nodes;
  int node_info_array [5]; /* to store the node data */
  int int_node_data;
  float float_node_data;

  char* string_node_data;
  int** edge_array_for_all_nodes; /* to store the edge data for each node */
  char info_accumulator [20]; /* to accumulate integer info */
  int info_counter; /* to count the amount of info accumulated */
  int counter; /* general use counter */
  int node_counter; /* to keep track of the node whose edges are being associated */
  int mflag, nflag,  eflag, hashflag, starflag;
  char current_region;

  FILE *fp;

  char** error;
  
  /* set flag values */
  mflag = 0;
  nflag = 0;
  eflag = 0;
  hashflag = 0;
  starflag = 0;

  /* set granph meta data store values */
  num_nodes = 0;
  max_nodes = 0;
  int_node_data = 0;
  float_node_data = 0.0;
  new_node = 0;

  /* set counter values */
  info_counter = 0;
  counter = 0;
  node_counter = 0;

  /* set error pointer */
  error = (char**) malloc (sizeof (char*));

  /* set info accumulator value */
  for (i = 0; i<20 ; i++)
    info_accumulator [i] = ' ';

  /* set node_info_accumulator value */
  for (j=0; j<5 ;j++)
    node_info_array[j] = -1;

  /* assuming that the path is correct */
  fp = fopen ( filename , "r");

  /* read through file */
  /* c is read as an unsigned char cast to an int or EOF on end of file or error.*/
  while ((c = fgetc (fp)) != EOF)
    {
      /* we first check c to determine what environment we are in */
      switch (c)
	{
	  /* meta data */
	case '~':
	  if (mflag == 0) /* i.e., we are entering meta data region */
	    {
	      current_region = '~';
	      mflag = 1;
	      continue;
	    }
	  else
	    {
	      /* once the closing m is encountered, (i) dynalloc memory to 2-d array
	       * that stores node-edge associations */
	      edge_array_for_all_nodes = (int**) malloc (sizeof (int*) * max_nodes);
	    
	      /* set all the values to NULL initially to account for NULL nodes */
	      for (i=0 ; i<max_nodes ; i++)
		edge_array_for_all_nodes[i] = NULL;
	    
	      /* now intialize the graph */
	      new_graph = (graph*) malloc (sizeof (graph));
	 
	      /* initialize graph fields */
	      (*new_graph).all_nodes = (node**) malloc (sizeof (node*) * max_nodes);
	      (*new_graph).num_nodes = num_nodes;
	      (*new_graph).max_nodes = max_nodes;
	      /* end of initialization code */

	      /* tests */
	      /*printf ("Number of nodes active or deleted %d \n", (*new_graph).num_nodes);
		printf ("Number of allocated nodes %d \n", (*new_graph).max_nodes);*/
	      
	      /* reset variables */
	      current_region = '?';
	      mflag = 0;
	      continue;
	    }
	  break;

	  /* node data */
	case '$':
	  if (nflag == 0)
	    {
	      current_region = '$';
	      nflag = 1;
	      /* malloc memory to store string data */
	      string_node_data = (char*) malloc (sizeof(char) * 256);
	      continue;
	    }
	  else
	    {
	      /* once we exit the node environment, we have the data to make the node
	       * and associate it with the current graph */
	    
	      /* account for NULL nodes */
	      if (node_info_array[4] == -1)
		{
		  /*printf ("This is a null array \n");*/
		  (*new_graph).all_nodes[node_counter++] = NULL;
		}
	      else
		{
		  /* malloc memory for the node */
		  new_node = node_counter++;
		  /* printf ("The new_node value is %d \n", new_node); */
		  (*new_graph).all_nodes[new_node] = (node*) malloc (sizeof (node));
		  
		  /* input the node fields */

		  /* data type */
		  (*new_graph).all_nodes[new_node][0].data_type = node_info_array [0];

		  /* data value */
		  switch ((*new_graph).all_nodes[new_node][0].data_type)
		    {
		    case 1:
		      (*new_graph).all_nodes[new_node][0].data =
			(int*) malloc (sizeof (int));
		      *(int*) (*new_graph).all_nodes[new_node][0].data = int_node_data;
		      break;
		    case 2:
		      (*new_graph).all_nodes[new_node][0].data =
			(float*) malloc (sizeof (float));
		      *(float*) (*new_graph).all_nodes[new_node][0].data = float_node_data;
		      break;
		    case 3:
		      (*new_graph).all_nodes[new_node][0].data =
			(char*) malloc (sizeof(char) * 256);
		      strcpy ( (char*) (*new_graph).all_nodes[new_node][0].data,
			       string_node_data);
		      break;
		    }
		
		  /* uid */
		  (*new_graph).all_nodes[new_node][0].uid = node_info_array[2];

		  /* num edges */
		  (*new_graph).all_nodes[new_node][0].num_edges = node_info_array[3];

		  /* max edges */
		  (*new_graph).all_nodes[new_node][0].max_edges = node_info_array[4];

		  /* edge pointer array */
		  (*new_graph).all_nodes[new_node][0].connected_nodes =
		    (node**) malloc (sizeof (node*) * node_info_array[4]);

		  /* set all the edge values to NULL */
		  for (j=0; j<node_info_array[4] ; j++)
		    (*new_graph).all_nodes[new_node][0].connected_nodes[j] = NULL;
		  
		  /* reset node_info_array */
		  for (j=0; j<5 ;j++)
		    node_info_array[j] = -1;

		  /* tests */
		  /*printf ("The uid %d \n", (*new_graph).all_nodes[new_node][0].uid );
		  printf ("The datatype %d \n", (*new_graph).all_nodes[new_node][0].data_type);
		  printf ("The number of edges %d \n",
			  (*new_graph).all_nodes[new_node][0].num_edges);
		  printf ("The max number of edges %d \n", 
			  (*new_graph).all_nodes[new_node][0].max_edges);
			  printf (" \n \n"); */

		  
		  /* finally, reset counter to 0 */
		  counter = 0;		
		}

	      /* end of node initialization code */

	      /* reset variables */
	      /* free string storer if required */
	      free (string_node_data);
	      current_region = '?';
	      nflag = 0;
	      continue;
	    }
	  break;
	  /* edge data */
	case '^':
	  if (eflag == 0)
	    {
	      /* when entering the e environment, dynalloc to a pointer in
	       * the edge_array_for_all_nodes to store edges */
	      counter = 0;
	    
	      edge_array_for_all_nodes[node_counter] =
		(int*) malloc (sizeof (int) * node_info_array[4]);

	      /* set current region to e */
	      current_region = '^';
	      eflag = 1;
	      continue;
	    }
	  else
	    {
	      /* once we are out of the edge environment, we need to store the
	       * edge values, and associate them with the node */
	      /* this is taken care of by the double pointer edge_array_for_all_nodes */
	    	    
	      /* finally, reset the counter used to fill the array */
	      counter = 0;
	      eflag = 0;
	    }
	case '<':
	case '>':
	  continue;
	default:
	  break;

	}/* end of switch statement */

      /* process data depending on region */
      switch (current_region)
	{
	case '~':
	  /* capture data using the switch statement */
	  switch (c)
	    {
	    case '#':
	      if (hashflag == 0)
		{
		  hashflag = 1;
		  continue;
		}
	      /* when a closing # is encountered, we should have succesfully captured all the
	       * the data; put this into the info accumulator */
	      else 
		{
		  if (counter == 0)
		    {
		      /* store the extracted value */
		      max_nodes = (int) strtol (info_accumulator, error, 10);
		      /*printf ("%d \n", num_nodes);*/
		      
		      /* reset the accumulator and the counter */
		      for (i = 0; i<20 ; i++)
			info_accumulator [i] = ' ';
		      info_counter = 0;
		      hashflag = 0;
		      counter ++;
		    }
		  else
		    {
		      num_nodes = (int) strtol (info_accumulator, error, 10);
		      counter = 0; /* because we are certain that there are no more values*/
		      
		      /* reset accumulators */
		      for (i = 0; i<20 ; i++)
			info_accumulator [i] = ' ';

		      /* reset counters */
		      counter = 0;
		      info_counter = 0;
		      hashflag = 0;
		    }
		}
	      break;
	      /* if not a #, accumulate the data value */
	    default:
	      info_accumulator[info_counter++] = c;
	      break;
	    }/* end of switch statement for 'm' */
	  break;
	  /* when we are in a node region */
	case '$':
	  switch (c)
	    {
	    case '#':
	      if (hashflag == 0)
		{
		  /*printf ("This is the hash value %d \n", hashflag);*/
		  hashflag = 1;
		  continue;
		}
	      /* the data value has been accumulated */
	      else 
		{
		  /* the node value might be NULL */
		  if (info_accumulator[0] == 'X')
		    {
		      /* reset counter to 0 */
		      counter = 0;
		    
		      /* if the node is NULL, it has no edges */
		      /* write code to direct pointer in all_nodes to NULL */
		      /* we continue, because the next character is guaranteed to be an n*/
		    }
		
		  /* if the node is not null, counter == 5 => all the node field values
		   * have been obtained */
		  else
		    {
		      /* printf ("Counter value: %d \n" , counter);
		      printf (" stored value %s \n", info_accumulator);
		      printf (" \n \n");*/
		      if (counter == 1) /* this is the data value */
			{
			  switch (node_info_array[0]) /* the data type */
			    {
			    case 1:
			      int_node_data = (int) strtol (info_accumulator, error, 10);
			      break;
			    case 2:
			      float_node_data = atof (info_accumulator);
			      break;
			    case 3: 
			      for (j=0 ; j<info_counter ; j++)
				string_node_data[j] = info_accumulator [j];
			      break;
			    }
			  counter++;
			}
		      /*		    if (counter == 5)
					    reset counters and flags to 0
					    counter = 0; */ 
		      else
			/* in all other cases, store the data value */
			node_info_array [counter++] = (int) strtol (info_accumulator, error, 10);
		    }

		  /* reset info array and info accumulator */
		  for (i=0 ; i<20 ; i++)
		    info_accumulator [i] = ' ';
		  info_counter = 0;

		  /* reset hashflag */
		  hashflag = 0;
		}
	      break;
	    
	      /* if c !=#, it must be data; accumulate it */
	    default:
	      info_accumulator [ info_counter++ ] = c;
	      /*printf ("%s \n", info_accumulator);*/
	      continue;
	    } /* end of switch statement for the node environment */
	  break;
	  
	  /* the edge environment */
	case '^':
	  switch (c)
	    {
	    case '*':
	      if (starflag == 0)
		{
		  starflag = 1;
		  continue;
		}
	      /* else the edge value has been accumulated, and we must store it */
	      else
		{
		  /*printf ("Looking at edges now..\n");
		    printf ("%s \n" , info_accumulator);*/
		  /* check for NULL value, store it as -1 in the array (since no node will
		   * have this UID */
		  if (info_accumulator[0] == 'X')
		    edge_array_for_all_nodes[node_counter][counter++] = -1;
		  else
		    edge_array_for_all_nodes[node_counter][counter++] =
		      (int) strtol (info_accumulator, error, 10);

		  /* reset the info accumulator and counter */
		  for (i=0 ; i<20 ; i++)
		    info_accumulator[i] = ' ';
		  info_counter = 0;

		  starflag = 0;
		}
	      break;
	      /* if not a star separpator, the character must be a data value */
	    default:
	      info_accumulator [ info_counter++ ] = c;
	      continue;
	    }/* end of switch statement for the e environment */
	  break; 
	}/* end of switch statement for current env */
      
    }/* end of while loop*/

  
  /* close file */
  fclose (fp);

  /* once all the characters are read from the file, connect the nodes to form edges */
  for (i=0 ; i< (*new_graph).max_nodes ; i++)
    {
      /* check if node is null or not */
      if ( (*new_graph).all_nodes[i] == NULL)
	continue;

      /* if not, proceed to update the edge values */
      for (j=0 ; j< (*new_graph).all_nodes[i][0].max_edges ; j++)
	{
	  /* check to make sure that the graph isn't empty */
	  if( (*new_graph).max_nodes == 0)
	    break;
	   	
	  /* check for NULL nodes */
	  if ( edge_array_for_all_nodes[i][j] == -1)
	    (*new_graph).all_nodes[i][0].connected_nodes[j] = NULL;
	  /* the UIDs correspond to the indices of the all_nodes array */
	  else
	    (*new_graph).all_nodes[i][0].connected_nodes[j] =
	      (*new_graph).all_nodes[ edge_array_for_all_nodes[i][j] ];
	}
    }

  /* set root node */
  if ((*new_graph).all_nodes[0] != NULL)
    (*new_graph).root_node = (*new_graph).all_nodes[0];
    

  /* update uid */
  /*printf ("%d \n", (*new_graph).num_nodes);*/
  uid =(*new_graph).num_nodes;

  /* free temp pointers */
  for (i=0 ; i< (*new_graph).max_nodes ; i++)
    {
      /*printf ("%d \n", i); */
      if (edge_array_for_all_nodes[i] == NULL)
	continue;
      free (edge_array_for_all_nodes[i]);
      edge_array_for_all_nodes[i] = NULL; 
    }
  
  free (edge_array_for_all_nodes);
  edge_array_for_all_nodes = NULL;
  free (error);
  error = NULL;
      
  /*return graph */
  return (new_graph);
}
  





/* void delete_graph (graph *g); Deletes a graph and all the nodes associated with the
   graph. */

void delete_graph (graph *g)
{
  /* declarations */
  int i;
  node temp_node;
  printf ("\n \n Deleting graph...\n");
  /* loop through all nodes, and free resources within them */
  for ( i = 0 ; i < (*g).max_nodes ; i++)
    {
      /* check_validity_of_edge for NULL pointers */
      if ( (*g).all_nodes[i] == NULL)
	continue;
      /*printf (" we're trying to delete this graph 1 \n");*/
      temp_node = (*g).all_nodes[i][0];
      
      /* free the data pointer */
      free (temp_node.data);
      /*printf ("we're trying to delete this graph 2 \n");*/
      temp_node.data = NULL;

      /* free pointer to pointers to neighbouring nodes; don't free the pointers to the
       * the neighbouring nodes, because these resources are freed when freeing the nodes
       * themselves */
      /*printf ("we're trying to delete this graph 3 \n");*/
      free (temp_node.connected_nodes);
      /*printf ("we're trying to delete this graph 4 \n");*/
      temp_node.connected_nodes = NULL;
      
      /* finally, free the  pointer to the node  */
      free ( (*g).all_nodes[i]);
      /*printf ("we're trying to delete this graph 5 \n");*/
      (*g).all_nodes[i] = NULL;
    }
  /* what happens to the num_nodes and max_nodes values: they are deleted when 
   * the struct itself is deleted */

  /* free pointer to all nodes */
  free ((*g).all_nodes);
  
  /* free pointer to root node */
  /* free ( (*g).root_node ); */

  /* finally, delete the pointer to the graph itself */
  free (g);
  g = NULL;
  printf ("\n \n Graph succesfully deleted. You will now be directed to main menu \n \n");
}





/* ******** the user_interaction_loop ******** */

void user_interaction_loop()
{
  /* declarations */
  graph* user_graph;
  char char_input;
  int int_input;
  int flag2; /* for the REPL */
  char* file_name_input;
  char* temp_string;
  char* cwd;
  node** nodes_at_given_distance;
  int distance;
  int i;

  /* preambnle */
  printf ("Hello and welcome to this implementation of Graphs \n");
  printf ("Note the following features of this implementation of graphs: \n ");
  /* describe features of graph here */
  printf (" -> Graphs are undirected, and unweighted. \n");
  printf (" -> Graphs may contain isolated nodes; no edge creation is forced. \n ");
  printf (" -> Edges cannot exist between the same node. \n");
  printf (" -> The data values stored are integers, float, or strings. \n");
  printf (" -> 1: integer ; 2: float ; 3: string");
  printf (" \n \n");
    
  
  /* the main loop */
  while (1)
    {
      flag2 = 2;
      printf (" Enter y (lower case) if you would like to initialize a graph , and any other key if you would like to exit this program \n");
      if (scanf ("%c" , &char_input) == 0)
	{
	  while (fgetc (stdin) != '\n');
	  printf (" The input format was incorrect. Please try again. \n \n");
	  continue;
	}
      if (char_input == 'y')
	/* the REPL */
	{
	  user_graph = create_new_graph ();
	  while (flag2 == 2)
	    {
	      printf (" \n \n |---------------------- START MENU ----------------------| \n");
	      printf (" To perform the following actions on the graph you have initialized, enter the integer provided before the action: \n (1) Add node to graph \n (2) Delete node from graph \n (3) Create an edge between two nodes \n (4)  Edit a node \n (5) Edit an edge of the graph \n (6) Delete an edge of the graph \n") ;
	      printf (" (7) Print a node \n (8) Print the graph \n (9) Compute the shortest distance between two nodes \n (10) Compute the shortest path between two nodes \n (11) Get nodes at a given distance from the root \n (12) Delete graph \n (13) Export current graph to XML file \n (14) Import/load graph from XML file. \n \n IF YOU WOULD LIKE TO INITIALIZE A NEW GRAPH OR EXIT, ENTER -1. \n");
	      printf (" |---------------------- ---------- ----------------------| \n \n");

	      if (scanf ("%d" , &int_input) == 0)
		{
		  while (fgetc(stdin) != '\n');
		  printf ("The input choice was incorrect. Please try again. \n \n");
		  continue;
		}
	      switch (int_input)
		{
		case 1:
		  add_node (user_graph);
		  break;
		case 2:
		  delete_node (user_graph);
		  break;
		case 3:
		  create_edge (user_graph);
      		  break;
		case 4:
		  edit_node (user_graph);
		  break;
		case 5:
		  edit_edge (user_graph);
		  break;
		case 6:
		  delete_edge (user_graph);
		  break;
		case 7:
		  print_node (user_graph);
		  break;
		case 8:
		  print_graph (user_graph);
		  break;
		case 9:
		  distance_nodes (user_graph);
		  break;
		case 10:
		  print_path (user_graph);
		  break;
		case 11:
		  /* take user input */
		  printf (" Enter the distance from the root node \n");
		  scanf ("%d", &distance);
		  nodes_at_given_distance =  get_nodes_at_distance (distance, user_graph);
		  if (nodes_at_given_distance[0] == NULL)
		    {/* i.e., no nodes were found at said distace */
		      printf (" No nodes were found at given distance from the root node. The distance value you have entered the maximum distance between the root and any other node of the graph \n \n");
		      free (nodes_at_given_distance);
		      continue;
		      
		    }
		  else
		    {
		      printf ("The nodes at distance %d from the root are: \n", distance);
		      for (i=0 ; nodes_at_given_distance[i] != NULL ; i++)
			{
			  printf ("The node with UID %d \n",
				  nodes_at_given_distance[i][0].uid);
			}
		    }
		  /* free resources */
		  free (nodes_at_given_distance);
		  break;
		case 12:
		  delete_graph (user_graph);
		  flag2 = 0; /* to reinitialize graph */
		  while (fgetc(stdin)!= '\n');
		  break;
		case 13:
		  file_name_input = (char*) malloc (sizeof(char) * 256);
		  temp_string = (char*) malloc (sizeof(char) * 256);
		  printf ("Enter file name without the extension or path. For example, if you would like to create the file ~/../../../testName.xml, just enter testName. The file will be saved in the current working directory, that is, the directory from which you are running this program. To find the xml file, look for [cwd]/[file_name].xml:  \n");
		  scanf ("%s" , file_name_input);
		  strcpy (temp_string, "./");
		  strcat (temp_string, file_name_input);
		  strcat (temp_string, ".xml");
		  export_graph (user_graph, temp_string);
		  free (file_name_input);
		  free (temp_string);
		  break;
		case 14:
		  cwd = (char*) malloc (sizeof(char) * 1024);
		  printf (" You have chosen to import a new graph. \n");
		  printf (" The current graph will be deleted. Deleting current graph... \n");
		  delete_graph (user_graph);
		  /* delete graph sets the user_graph pointer to NULL. We now call the 
		   * load_graph function and set the user_graph to the returned value */
		  file_name_input = (char*) malloc (sizeof(char) * 256);
		  printf ("Please enter file name (WITHOUT PATH, but WITH EXTENSION). For example, if your file name is ~/POSIX/test5.xml, please enter only test5.xml. Also ensure that your file is the current working directory.  \n");
		  scanf( "%s" , file_name_input);
		  getcwd( cwd, sizeof(char)*1024);
		  strcat (cwd, "/");
		  strcat (cwd, file_name_input);
		  printf ("%s\n", cwd);
		  user_graph = load_graph (file_name_input);
		  printf ("The graph was succesfully loaded. \n");
		  printf ("Printing graph...");
		  print_graph (user_graph);

		  /* free pointers */
		  free (cwd);
		  cwd = NULL;
		  free (file_name_input);
		  file_name_input = NULL;
		  break;
		default:
		  /* flush the stdin buffer */
		  flag2 = 0;
		  delete_graph (user_graph) ; /* clear resources */
		  while (fgetc(stdin)!= '\n');
		  break;
		}	      
	    }
	}
      else
	{
	  printf ("Exiting program... \n \n");
	  exit (0);
	}      
    }
}







/* ******** main() function: it all starts here ******** */

int main (void)
{
  /* initialize uid before building graphs */
  uid = 0;
  /* call interactive module */
  user_interaction_loop ();
  /* call the test module */
 
  return (0);
}

/* ******** end of file ******** */
