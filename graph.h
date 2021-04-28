//----------------------------------------------------------------------
// NAME: S. Bowers
// FILE: graph.h
// DATE: Spring 2021
// DESC: A directed graph consists of vertices and edges. Each vertex
// has a unique identifier (from 0 to n-1) and each edge has a
// (non-unique) integer label.
//----------------------------------------------------------------------

#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <set>
#include <unordered_map>
#include <vector>

// shortened hash-table map name for convenience
typedef std::unordered_map<int,int> Map;
typedef std::set<int> Set;


class Graph
{
public:

  // default destructor
  virtual ~Graph() {};

  // add or update an edge between existing vertices in the graph
  virtual void set_edge(int v1, int edge_label, int v2) = 0;

  // check if the given edge is in the graph
  virtual bool has_edge(int v1, int v2) const = 0;  

  // returns true if there is an edge between v1 and v2 in the graph
  // the output parameter returns the edge label
  virtual bool get_edge(int v1, int v2, int& edge) const = 0; 

  // remove edge between v1 and v2 if the edge exists
  virtual void remove_edge(int v1, int v2) = 0;
  
  // get all vertices on an outgoing edge from given vertex
  virtual void connected_to(int v1, std::list<int>& vertices) const = 0;

  // get all vertices on an incoming edge to given vertex
  virtual void connected_from(int v2, std::list<int>& vertices) const = 0;

  // get all vertices adjacent to a vertex, that is, connected to or connected
  // from the vertex (may return duplicate vertices)
  virtual void adjacent(int v, std::list<int>& vertices) const = 0;

  // get number of nodes in the graph
  virtual int vertex_count() const = 0; 

  // get number of edges in the graph
  virtual int edge_count() const = 0; 

  
  //----------------------------------------------------------------------
  // HW-3 graph operations
  //----------------------------------------------------------------------
  
  //----------------------------------------------------------------------
  // Breadth-first search from a given source vertex. 
  //
  // Inputs: 
  //   dir -- true if directed
  //   src -- the source vertex
  // Outputs:
  //   tree -- search tree that maps vertices found during bfs from the
  //           source to their parent vertices
  //----------------------------------------------------------------------
  void bfs(bool dir, int src, Map& tree) const;
  
  //----------------------------------------------------------------------
  // Shortest path length from source to destination vertices.
  //
  // Conditions:
  //   The source and destination vertices must be unique.  
  // Inputs:
  //   dir -- true if directed
  //   src -- the vertex starting the path
  //   dst -- the vertex ending the path
  // Outputs:
  //   path -- sequence of nodes that define the shortest path
  //----------------------------------------------------------------------
  void shortest_path_length(bool dir, int src, int dst, std::list<int>& path) const;

  //----------------------------------------------------------------------
  // Find connected components based on breadth-first search.
  //
  // Conditions:
  //   Finds strongly connected components in an undirected graph and
  //   weakly-connected components in a directed graph.
  // Inputs:
  //   None
  // Outputs: 
  //   components -- mapping from each graph vertex to its corresponding
  //                 component number where component numbers range from
  //                 0 to c-1 (for c components)
  //----------------------------------------------------------------------
  void bfs_connected_components(Map& components) const;

  //----------------------------------------------------------------------
  // Determine if the graph is bipartite (i.e., 2-colorable)
  //
  // Inputs:
  //   None
  // Outputs:
  //   returns  -- true if the graph is bipartite, false otherwise
  //   coloring -- mapping from each graph vertex to its corresponding
  //               color (either 0 or 1) if graph is bipartite
  //----------------------------------------------------------------------
  bool bipartite_graph(Map& coloring) const;


  //----------------------------------------------------------------------
  // HW-4 graph operations
  //----------------------------------------------------------------------
  
  //----------------------------------------------------------------------
  // Depth-first search from a given source vertex.
  //
  // Inputs: 
  //   dir -- if true assumes graph is directed
  //   src -- the source vertex
  // Outputs:
  //   tree -- search tree that maps vertices found during dfs to their
  //           corresponding parent vertex.
  //----------------------------------------------------------------------
  void dfs(bool dir, int src, Map& tree) const;

  //----------------------------------------------------------------------
  // Determine if the graph is acyclic or not.
  //
  // Inputs:
  //   dir -- if true assumes graph is directed
  // Outputs:
  //   returns -- true if acyclic
  //----------------------------------------------------------------------
  bool acyclic(bool dir) const;

  //----------------------------------------------------------------------
  // Naive implementation to compute the transitive closure of the
  // current graph without consideration of edge weights.
  //
  // Conditions: Assumes that the given graph (the closed_graph) is a
  //             copy of the current graph prior to the call.
  // 
  // Inputs:
  //   dir -- if true assumes graph is directed
  // Outputs:
  //   closed_graph -- the transitively closed graph, where added
  //                   edges have
  //----------------------------------------------------------------------
  void unweighted_transitive_closure(bool dir, Graph& closed_graph) const;

  //----------------------------------------------------------------------
  // Computes a topological sort of the current graph based on dfs.
  //
  // Conditions: Assumes the graph is directed.
  //
  // Inputs:
  //   none
  // Outputs:
  //   vertex_ordering -- a map from vertex to it's corresponding
  //                      order in the topological sort (where nodes
  //                      are ordered from 1 to n)
  //----------------------------------------------------------------------
  void dfs_topological_sort(Map& vertex_ordering) const;
  

  //----------------------------------------------------------------------
  // HW-5 graph operations
  //----------------------------------------------------------------------

  //----------------------------------------------------------------------
  // Computes the strongly connected components.
  //
  // Inputs:
  //   none
  // Outputs: 
  //   components -- mapping from each graph vertex to its corresponding
  //                 component number where component numbers range from
  //                 0 to c-1 (for c components)
  //----------------------------------------------------------------------
  void strongly_connected_components(Map& components) const;

  //----------------------------------------------------------------------
  // Computes the transitive reduction.
  //
  // Conditions: Assumes that the given graph (the closed_graph) has
  //             the same number of nodes as the current graph. But
  //             does not have any edges prior to the call
  //
  // Inputs:
  //   none
  // Outputs:
  //   reduced_graph -- the reduced edges are added to the reduced graph
  //----------------------------------------------------------------------
  void transitive_reduction(Graph& reduced_graph) const;

  //----------------------------------------------------------------------
  // Check if an eulerian exists in a directed graph, and if so,
  // return one.
  //
  // Conditions: Assumes the graph is not disconnected.
  //
  // Inputs:
  //   none
  // Outputs:
  //   path -- the path as an ordered list of vertices
  //----------------------------------------------------------------------
  bool directed_eulerian_path(std::list<int>& path) const;

  
  //----------------------------------------------------------------------
  // HW-6 graph operations
  //----------------------------------------------------------------------

  //----------------------------------------------------------------------
  // Returns a Hamiltonian path if one exists in the current graph.
  //
  // Conditions: Treats the graph as directed.
  //
  // Outputs:
  //   path -- the Hamiltonian path
  //
  // Returns:
  //   true if a Hamiltonian path exists, false otherwise.
  //----------------------------------------------------------------------
  bool directed_hamiltonian_path(std::list<int>& path) const;

  //----------------------------------------------------------------------
  // Find a maximum matched graph using the augmenting paths algorithm
  // from the textbook.
  //
  // Conditions: Finds a matching only if the current graph is bipartite
  //
  // Output:
  //   max_matched_graph -- A graph with same vertices as original
  //                        graph, containing the edges in the
  //                        matching. The output graph is assumed to
  //                        be initialized with the same number of
  //                        vertices, but without any edges.
  // Returns:
  //   true if the current graph is bipartite, false otherwise
  //----------------------------------------------------------------------
  bool bipartite_graph_matching(Graph& max_matched_graph) const;

  //----------------------------------------------------------------------
  // Finds all (maximal) cliques in the graph using the Bron-Kerbosch
  // algorithm.
  //
  // Conditions: Assumes the graph is undirected.
  //
  // Output:
  //   cliques -- a list of list of vertices denoting a maximal clique
  //----------------------------------------------------------------------
  void cliques(std::list<Set>& cliques) const;
  

  //----------------------------------------------------------------------
  // HW-7 graph operations
  //----------------------------------------------------------------------

  //----------------------------------------------------------------------
  // Single-source shortest paths from the given source using
  // Dijkstra's algorithm.
  //
  // Conditions: Assumes graph is directed and maximum weight is
  //             numeric_limits<int>::max()
  // 
  // Input:
  //  src -- the source vertex
  //
  // Output:
  //  path_costs -- the minimum path cost from src to each vertex v
  //                given as path_costs[v].
  //----------------------------------------------------------------------
  void dijkstra_shortest_path(int src, Map& path_costs) const;
  
  //----------------------------------------------------------------------
  // Compute a minimum spanning tree using Prim's algorithm.
  //
  // Conditions: Assumes a connected, undirected graph. The spanning
  //             tree is represented as a graph, which is initialized
  //             with the same vertices as the current graph, but with
  //             no edges (on input).
  //
  // Output:
  //  spanning-tree -- A graph containing the minimum spanning tree
  //                   edges.
  //
  //----------------------------------------------------------------------
  void prim_min_spanning_tree(Graph& spanning_tree) const;

  //----------------------------------------------------------------------
  // Compute a minimum spanning tree using Kruskal's algorithm.
  //
  // Conditions: Assumes a connected, undirected graph. The spanning
  //             tree is represented as a graph, which is initialized
  //             with the same vertices as the current graph, but with
  //             no edges (on input).
  //
  // Output:
  //  spanning-tree -- A graph containing the minimum spanning tree
  //                   edges.
  //
  //----------------------------------------------------------------------
  void kruskal_min_spanning_tree(Graph& spanning_tree) const;


  //----------------------------------------------------------------------
  // HW-8 graph operations
  //----------------------------------------------------------------------

  //----------------------------------------------------------------------
  // Single-source shortest paths from the given source using
  // Bellman-Ford's algorithm.
  //
  // Conditions: Assumes graph is directed and maximum weight is
  //             numeric_limits<int>::max()
  // 
  // Input:
  //  src -- the source vertex
  //
  // Output:
  //  path_costs -- the minimum path cost from src to each vertex v
  //                given as path_costs[v].
  //
  // Returns: true if there is not a negative cycle, and false
  //          otherwise
  //----------------------------------------------------------------------
  bool bellman_ford_shortest_path(int src, Map& path_costs) const;


  //----------------------------------------------------------------------
  // HW-9 graph operations
  //----------------------------------------------------------------------

  //----------------------------------------------------------------------
  // Finds a maximum-weight independent set for a path graph using
  // dynamic programming.
  //
  // Conditions: Assumes the graph is a path-graph starting at vertex 0 and
  //             ending at vertex n-1, that is, E = {(v0,v1), (v1,v2), ...,
  //             v(n-1,vn)}
  //
  // Input:
  //  vertex_weights -- weights for each of the n vertices in the graph
  //
  // Output:
  //  S -- set of vertices representing the maximum independent set
  //
  // Returns: true if the graph is a path graph, false otherwise
  //----------------------------------------------------------------------
  bool path_max_independent_set(int vertex_weights[], Set& S) const;

  //----------------------------------------------------------------------
  // Finds all-pairs shortest paths using the Floyd-Warshall algorithm.
  //
  // Conditions: Assumes weights is an uninitialized "n x n" matrix
  //             represented as a vector of vectors (to be completely
  //             filled in with all-pairs shortest path weights)
  //
  // Output:
  //   weights -- shortest path weights from u to v given by weights[u][v]
  //              for all u,v in the graph's vertices
  //
  // Returns: true if there is not a negative cycle, and false
  //          otherwise
  //----------------------------------------------------------------------
  bool all_pairs_shortest_paths(std::vector<std::vector<int>>& weights) const;
  
private:

  // helper function for directed hamiltonian recursive function
  bool directed_hamiltonian_rec(int v, std::list<int>& path,
                                bool discovered[]) const;

  void cliques_rec(Set& p, Set& r, Set& x, std::list<Set>& cliques) const;
  
};


#endif
