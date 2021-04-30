//----------------------------------------------------------------------
// Name: Sami Blevens
// File: graph.cpp
// Date: Spring 2021
// Desc: A directed graph consists of vertices and edges. Each vertex
// has a unique identifier (from 0 to n-1) and each edge has a
// (non-unique) integer label.
//----------------------------------------------------------------------

#include "graph.h"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>

void Graph::bfs(bool dir, int src, Map& tree) const {
   //set root
   tree[src] = -1;
   //queue init with src
   std::queue<int> q;
   q.push(src);

   while(!q.empty()){
      int u = q.front();
      q.pop();

      std::list<int> adj_v;
      //if directed , call connected_to
      //if undirected , call adjacent
      if(dir)
         connected_to(u,adj_v);
      else
         adjacent(u,adj_v);
      for(int v: adj_v){
         if(tree.count(v) == 0){
            tree[v] = u;
            q.push(v);
         }
      }
   }
}

void Graph::shortest_path_length(bool dir, int src, int dst, std::list<int>& path) const {
   Map search_tree;
   bfs(dir,src,search_tree);
   int parent = -1;
   int child = dst;
   path.push_front(dst);
   while(parent!=src){
      //if path does not connect/exist
      if(search_tree.count(child)==0){
         path.clear();
         return;
      }
      parent = search_tree[child];
      path.push_front(parent);
      child = parent;
   }
}

void Graph::bfs_connected_components(Map& components) const {
   Map search_tree;
   int c = 0;
   for(int i=0; i<vertex_count(); ++i){
      if(components.count(i) == 0){
         search_tree.clear();
         bfs(false,i,search_tree);
         for(std::pair<int,int> p: search_tree){
            components[p.first] = c;
         }
         ++c;
      }
   }

}

bool Graph::bipartite_graph(Map& coloring) const {
   if(vertex_count() == 0){
      return false;
   }
   //1 = blue, 0 = green, -1 = not colored
   
   //queue init with src
   std::queue<int> q;

   for(int i=0; i<vertex_count(); ++i){
      if(coloring.count(i)==0){
         //mark first vertex blue
         coloring[i] = 1;
         q.push(i);

         while(!q.empty()){
            int u = q.front();
            q.pop();

            std::list<int> adj_v;
            adjacent(u,adj_v);

            for(int v: adj_v){
               if(coloring.count(v) == 0){
                  //set to opposite color
                  coloring[v] = (coloring[u] == 1) ? 0 : 1;
                  q.push(v);
               } else if(coloring[v] == coloring[u]){
                  return false;
               }
                  
               
            }
         }
      }
   }
   return true;

}

void Graph::dfs(bool dir, int src, Map& tree) const {
   bool *explored = new bool[vertex_count()];
   for(int i=0; i<vertex_count(); ++i){
      explored[i] = false;
   }
   //set root
   tree[src] = -1;
   //stack init with src
   std::stack<int> s;
   s.push(src);
   while(!s.empty()){
      int u = s.top();
      if(explored[u]==true){
         s.pop();
         continue;
      }
      explored[u] = true;

      std::list<int> adj_v;
      //if directed , call connected_to 
      //if undirected , call adjacent 
      if(dir)
         connected_to(u, adj_v);
      else 
         adjacent(u, adj_v);
      if(adj_v.size()>0)
         adj_v.sort();
      for(int v: adj_v){
         if(explored[v]==false){
            tree[v] = u;
            s.push(v); 
         }
      }
      
   }
   delete [] explored;
}

bool Graph::acyclic(bool dir) const {
   enum Color {
      BLACK, GRAY, WHITE
   };

   Color *explored = new Color[vertex_count()];
   for(int i=0; i<vertex_count(); ++i){
      explored[i] = WHITE;
   }

   Map tree;
   std::stack<int> s;

   for(int i=0; i<vertex_count(); ++i){
      if(explored[i]==WHITE){
         s.push(i);
         tree[i] = -1;
         while(!s.empty()){
            int u = s.top();
            if(explored[u]==GRAY){
               s.pop();
               explored[u]=BLACK;
               continue;
            }
            explored[u] = GRAY;

            std::list<int> adj_v;
            //if directed , call connected_to 
            //if undirected , call adjacent 
            if(dir)
               connected_to(u, adj_v);
            else 
               adjacent(u, adj_v);
            if(adj_v.size()>0)
               adj_v.sort();
            for(int v: adj_v){
               if(tree[u]==v)
                  continue;
               if(explored[v]==WHITE){
                  s.push(v);
                  tree[v] = u;
               } else if (explored[v] == GRAY){
                  delete [] explored;
                  return false;
               }
            }
            
         }
      }
   }
   
   delete [] explored;

   return true;
}

void Graph::unweighted_transitive_closure(bool dir, Graph& closed_graph) const {
   //Map tree;
   //std::stack<int> s;
   for(int src=0; src<vertex_count(); ++src){
      bool *explored = new bool[vertex_count()];
      for(int i=0; i<vertex_count(); ++i){
         explored[i] = false;
      }
      //set root
      //tree[start] = -1;
      //stack init with src
      std::stack<int> s;
      s.push(src);
      while(!s.empty()){
         int u = s.top();
         if(explored[u]==true){
            s.pop();
            continue;
         }
         explored[u] = true;

         std::list<int> adj_v;
         //if directed , call connected_to 
         //if undirected , call adjacent 
         if(dir)
            connected_to(u, adj_v);
         else 
            adjacent(u, adj_v);
         if(adj_v.size()>0)
            adj_v.sort();
         for(int v: adj_v){
            if(explored[v]==false){
               s.push(v);
               closed_graph.set_edge(src,0,v);
            }
         }
      }
      delete [] explored;

   }
}

void Graph::dfs_topological_sort(Map& vertex_ordering) const {
   int ordering_index = vertex_count();
   bool *explored = new bool[vertex_count()];
   for(int i=0; i<vertex_count(); ++i){
      explored[i] = false;
   }

   std::stack<int> s;
   for(int i=0; i<vertex_count(); ++i){
      if(explored[i]==false){
         s.push(i);
         //tree[i] = -1;
         while(!s.empty()){
            int u = s.top();
            if(explored[u]==true){
               s.pop();
               if(vertex_ordering.count(u)==0)
                  vertex_ordering[u]=ordering_index--;
               
               continue;
            }
            explored[u] = true;

            std::list<int> adj_v;
            //if directed , call connected_to 
            //if undirected , call adjacent 
            //adjacent(u, adj_v);
            connected_to(u,adj_v);
            if(adj_v.size()>0)
               adj_v.sort();
            for(int v: adj_v){
               if(explored[v]==false){
                  s.push(v);
               }
            }
            
         }
      }
   }
   delete [] explored;
}


void Graph::strongly_connected_components(Map& components) const {
   //Full DFS on reverse graph
   enum Color {
      BLACK, GRAY, WHITE
   };

   Color *explored = new Color[vertex_count()];
   for(int i=0; i<vertex_count(); ++i){
      explored[i] = WHITE;
   }

   Map tree;
   std::stack<int> s;
   std::list<int> linearization;

   for(int i=0; i<vertex_count(); ++i){
      if(explored[i]==WHITE){
         s.push(i);
         while(!s.empty()){
            int u = s.top();
            if(explored[u]==GRAY){
               s.pop();
               explored[u]=BLACK;
               linearization.push_front(u);
               continue;
            }
            explored[u] = GRAY;

            std::list<int> adj_v;
            //connected_from to reverse the graph
            connected_from(u, adj_v);

            if(adj_v.size()>0)
               adj_v.sort();
            for(int v: adj_v){
               if(explored[v]==WHITE){
                  s.push(v);
               }
            }
            
         }
      }
   }
   delete [] explored;

   //DFS on linearization
   int c=0;
   bool *discovered = new bool[vertex_count()];
   for(int i=0; i<vertex_count(); ++i){
      discovered[i] = false;
   }
   
   for(int v: linearization){
      std::stack<int> s;
      if(discovered[v]==false){
         c++;
         s.push(v);
      }
         
      while(!s.empty()){
         int u = s.top();
         if(discovered[u]==true){
            s.pop();
            continue;
         }
         discovered[u] = true;
         components[u]=c;
         std::list<int> adj_v;
         connected_to(u, adj_v);
         if(adj_v.size()>0)
            adj_v.sort();
         for(int adj: adj_v){
            if(discovered[adj]==false){
               s.push(adj);
            }
         }
      }
      
   }
   delete [] discovered; 
}

void Graph::transitive_reduction(Graph& reduced_graph) const {
   Map scc;
   std::list<int> adj_v;
   strongly_connected_components(scc);
   std::unordered_map<int, std::list<int>> c_list;
   for(int i=0; i<vertex_count(); ++i){
      int index = scc[i];
      c_list[index].push_back(i);
   }

   //cycle for each component
   int metagraph[c_list.size()];
   std::list<int> cur_component;
   int first_node;
   int prev_node;
   for(int i=0; i<c_list.size(); ++i){
      cur_component = c_list[i];
      first_node = cur_component.front();
      if(cur_component.size()>1){
         while(cur_component.size()>1){
            int edge1 = cur_component.front();
            cur_component.pop_front();
            prev_node = cur_component.front();
            reduced_graph.set_edge(edge1,0,prev_node);
         }
         reduced_graph.set_edge(prev_node,0,first_node);
      }
      
      cur_component.clear();
      metagraph[i] = first_node;
   }
   //set edges where paths exist between nodes in the metagraph
   std::list<int> path;
   for(int i=0; i<c_list.size(); ++i){
      for(int j=0; j<c_list.size(); ++j){
         path.clear();
         if(i!=j){
            shortest_path_length(true,metagraph[i],metagraph[j],path);
            if(path.size()>0)
               reduced_graph.set_edge(metagraph[i],0,metagraph[j]);
         }
      }
   }

   //irreducible 
   
   for(int i=0; i<vertex_count(); ++i){
      adj_v.clear();
      reduced_graph.connected_to(i,adj_v);
      for(int v: adj_v){
         reduced_graph.remove_edge(i,v);
         reduced_graph.shortest_path_length(true,i,v,path);
         if(path.empty())
            reduced_graph.set_edge(i,0,v);
      }
   }

}

bool Graph::directed_eulerian_path(std::list<int>& path) const {
   //check if eulerian path
   int edge_count = 0;
   int source_count = 0;
   int sink_count = 0;
   int source_vertex =0;
   std::list<int> adj_to;
   std::list<int> adj_from;
   for(int i=0; i<vertex_count(); ++i){
      adj_to.clear();
      adj_from.clear();
      connected_to(i, adj_to);
      connected_from(i, adj_from);
      if(adj_to.size() == adj_from.size()+1){ //check if source - i
         ++source_count;
         source_vertex = i;
      } else if (adj_to.size()+1 == adj_from.size()){ // check if sink - ii
         ++sink_count;
      } else if(adj_to.size() == adj_from.size()) { //same number of edges - iii
         ++edge_count;
      } else{
         return false;
      }
   } 
   //at most 1 source and sink
   if(source_count > 1 || sink_count > 1){
      return false;
   } else if((vertex_count() - source_count - sink_count) != edge_count){
      return false;
   }

   //find eulerian path
   std::unordered_map<int,std::list<int>> edges_left;
   for(int i=0; i<vertex_count(); ++i){
      std::list<int> adj;
      connected_to(i,adj);
      edges_left[i] = adj;
      adj.clear();
   }

   std::stack<int> s;
   s.push(source_vertex);
   while(!s.empty()){
      int u = s.top();
      if(edges_left[u].size()<=0){
         s.pop();
         //deadend - add to path
         //std::cout << "dead end " << u << std::endl;
         path.push_front(u);
         continue;
      }
      

      std::list<int> adj_v;
      connected_to(u,adj_v);
      if(adj_v.size()>0)
         adj_v.sort();
      for(int v: adj_v){
         if((std::find(edges_left[u].begin(), edges_left[u].end(), v) != edges_left[u].end())){
            edges_left[u].remove(v);
            //std::cout << u << " " << v << std::endl;
            s.push(v);
         }
      }
   }
   return true;
}


bool Graph::directed_hamiltonian_path(std::list<int>& path) const {
   for(int i=0; i<vertex_count(); ++i){
      bool discovered[vertex_count()];
      for(int j=0; j< vertex_count(); ++j){
         discovered[j] = false;
      }
      path.push_back(i);
      discovered[i] = true;
      if(directed_hamiltonian_rec(i,path,discovered)){
         return true;
      }
      path.clear();
   }

   return false;
}

bool Graph::directed_hamiltonian_rec(int v, std::list<int>& path, bool discovered[]) const {
   if(path.size() == vertex_count()){
      return true;
   }
   //directed = connected_to
   std::list<int> adj_v;
   connected_to(v, adj_v);
   if(adj_v.size()>0)
      adj_v.sort();
   for(int u: adj_v){
      if(discovered[u]==false){
         path.push_back(u);
         discovered[u] = true;
         if(directed_hamiltonian_rec(u,path,discovered)){
            return true;
         }
         path.pop_back();
         discovered[u]=false;
      }
   }
   return false;
}

bool Graph::bipartite_graph_matching(Graph& max_matched_graph) const {
   Map coloring;
   if(!bipartite_graph(coloring)){
      return false;
   }
   //separate into v1 and v2
   Set v1;
   Set v2;
   Map labels;
   bool free[vertex_count()];
   int mates[vertex_count()];

   for(int i=0; i<vertex_count(); ++i){
      if(coloring[i]==0){
         v1.insert(i);
      } else {
         v2.insert(i);
      }
      free[i] = true;
      mates[i] = -1;
   }   

   std::queue<int> q;
   for(int i: v1){
      q.push(i);
   }

   while(!q.empty()){
      int w = q.front();
      q.pop();
      // w in V (v1)
      if(v1.find(w) != v1.end()){
         std::list<int> adj_v;
         adjacent(w,adj_v);
         for(int u: adj_v){
            if(v2.find(u) != v2.end()){
               if(free[u]){ //augment
                  max_matched_graph.set_edge(w,0,u);
                  free[w] = false;
                  mates[w] = u;
                  free[u] = false;
                  mates[u] = w;
                  int v = w;
                  while(labels.count(v)>0){
                     u = labels[v];
                     max_matched_graph.remove_edge(v,u);
                     free[v] = true;
                     mates[v] = -1;
                     free[u] = true;
                     mates[u] = -1;
                     v = labels[u];
                     max_matched_graph.set_edge(v,0,u);
                     free[v] = false;
                     mates[v] = u;
                     free[u] = false;
                     mates[u] = v;
                  }
                  //start over
                  labels.clear();
                  while(!q.empty()){
                     q.pop();
                  }
                  for(int i: v1){
                     if(free[i])
                        q.push(i);
                  }
                  break;
               } else { //u is matched
                  if(!max_matched_graph.has_edge(w,u) && labels.count(u)==0){
                     labels[u] = w;
                     q.push(u);
                  }
               }
            }
         }
      } else { // w in U (v2)
         int v = mates[w];
         mates[v] = w;
         labels[v] = w;
         q.push(v);
      }
   }


   return true;
}

void Graph::cliques(std::list<Set>& cliques) const {
   Set p;
   Set r;
   Set x;
   for(int i=0; i<vertex_count(); ++i){
      p.insert(i);
   }
   cliques_rec(p,r,x,cliques);
}

void Graph::cliques_rec(Set& p, Set& r, Set& x, std::list<Set>& cliques) const {
   if(p.empty() && x.empty()){
      cliques.push_back(r);
      return;
   }
   if(p.empty()){
      return;
   }
   
   while(!p.empty()){
      int v = *p.begin();
      std::list<int> adj_v;
      adjacent(v,adj_v);
      std::list<int> temp;
      Set adj(adj_v.begin(),adj_v.end());
      //p intersect with adj(v)
      std::set_intersection(p.begin(),p.end(),adj.begin(),adj.end(),std::back_inserter(temp));
      Set p_new(temp.begin(),temp.end());
      //p = p_new;
      //r union with v
      r.insert(v);
      //x intersect with adj(v)
      std::set_intersection(x.begin(),x.end(),adj.begin(),adj.end(),std::back_inserter(temp));
      Set x_new(temp.begin(),temp.end());
      //x = x_new;
      
      cliques_rec(p_new,r,x_new,cliques);
      r.erase(v);
      p.erase(v);
      x.insert(v);
      
   }

   
   return;
}

//use get_edge(v1,v2,edge) to get edge label/weight
void Graph::dijkstra_shortest_path(int src, Map& path_costs) const {
   Set x;
   Set not_x;
   for(int i=0; i<vertex_count(); ++i){
      not_x.insert(i);
   }
   not_x.erase(src);
   x.insert(src);

   for(int i=0; i<vertex_count();++i){
      path_costs[i] = std::numeric_limits<int>::max();
   }
   path_costs[src] = 0;

   while(x.size()<vertex_count()){
      int min_vertex = -1;
      int min_value = std::numeric_limits<int>::max();
      for(std::set<int>::iterator u = x.begin(); u!=x.end(); ++u){
         std::list<int> adj_v;
         connected_to(*u,adj_v);
         for(int v: adj_v){
            //if v is not in x
            if(not_x.find(v) != not_x.end()){
               if(path_costs[v]==std::numeric_limits<int>::max()){
                  int edge;
                  get_edge(*u,v,edge);
                  if(path_costs[*u] + edge < min_value){
                     min_value = path_costs[*u] + edge;
                     min_vertex = v;
                  }
               }
            }
         }
      }
      x.insert(min_vertex);
      not_x.erase(min_vertex);
      path_costs[min_vertex] = min_value;
   }
}

void Graph::prim_min_spanning_tree(Graph& spanning_tree) const {
   Set x;
   Set not_x;
   for(int i=1; i<vertex_count(); ++i){
      not_x.insert(i);
   }
   x.insert(0);

   while(x.size()<vertex_count()){
      int min_vertex = -1;
      int init_vertex;
      int min_value = std::numeric_limits<int>::max();
      for(std::set<int>::iterator u = x.begin(); u!=x.end(); ++u){
         std::list<int> adj_v;
         adjacent(*u,adj_v);
         for(int v: adj_v){
            //if v is not in x
            if(not_x.find(v) != not_x.end()){
               if(!spanning_tree.has_edge(*u,v)){
                  int edge;
                  if(!get_edge(*u,v,edge))
                     get_edge(v,*u,edge);
                  if(edge < min_value){
                     min_value = edge;
                     init_vertex = *u;
                     min_vertex = v;
                  }
               }
            }
         }
      }
      x.insert(min_vertex);
      not_x.erase(min_vertex);
      spanning_tree.set_edge(init_vertex,min_value,min_vertex);
      }
}

void Graph::kruskal_min_spanning_tree(Graph& spanning_tree) const {
   std::tuple<int,int,int> edges[edge_count()];
   int index =0;
   int weight;
   for(int u=0; u<vertex_count(); ++u){
      for(int v=0; v < vertex_count(); ++v){
         if(get_edge(u,v,weight)){
            edges[index] = std::tuple<int,int,int>(u,weight,v);
            ++index;
         }
      }
   }
   int end = edge_count() - 1;
   sort_edges_by_weight(edges,0,end);

   for(int i=0; i < edge_count(); ++i){
      spanning_tree.set_edge(std::get<0>(edges[i]),std::get<1>(edges[i]),std::get<2>(edges[i]));
      if(!spanning_tree.acyclic(false)){
         spanning_tree.remove_edge(std::get<0>(edges[i]),std::get<2>(edges[i]));
      }
   }
}


void Graph::sort_edges_by_weight(std::tuple<int,int,int> edges[], int start, int end) const {
   if(start >= end)
    return;
  int mid = (start+end)/2;
  sort_edges_by_weight(edges,start,mid);
  sort_edges_by_weight(edges,mid+1,end);
  //merge(start,mid,end)
  std::tuple<int,int,int> temp[(end-start)+1];
  int first1 = start;
  int first2 = mid +1;
  int i = 0;
  while(first1 <= mid && first2 <= end){
    if(std::get<1>(edges[first1]) < std::get<1>(edges[first2]))
      temp[i++]=edges[first1++];
    else
      temp[i++]=edges[first2++];
  }
  while(first1<=mid){
    temp[i++]=edges[first1++];
  }
  while(first2<=end){
    temp[i++]=edges[first2++];
  }
  
  for(int i=0; i<=(end-start);++i){
    edges[i+start] = temp[i];
  }
}


bool Graph::bellman_ford_shortest_path(int src, Map& path_costs) const {
   for(int i=0; i<vertex_count();++i){
      path_costs[i] = std::numeric_limits<int>::max();
   }
   path_costs[src] = 0;

   for(int i = 1; i < vertex_count(); ++i){
      //for each edge:
      for(int u=0; u< vertex_count(); ++u){
         std::list<int> adj_v;
         connected_to(u,adj_v);
         for(int v: adj_v){
            int edge;
            get_edge(u,v,edge);
            //if dist s,v > dist s,u + w u,v
            if(path_costs[v] > (path_costs[u] + edge)){
               path_costs[v] = path_costs[u] + edge;
            }
         }
      }
   }

   for(int u=0; u<vertex_count(); ++u){
         std::list<int> adj_v;
         connected_to(u,adj_v);
         for(int v: adj_v){
            int edge;
            get_edge(u,v,edge);

            //if dist s,v > dist s,u + w u,v
            if(path_costs[v] > (path_costs[u] + edge)){
               return false;
            }
         }
   }

   return true;
}


bool Graph::path_max_independent_set(int vertex_weights[], Set& S) const {
   for(int i = 1; i < vertex_count(); ++i) {
        if(!has_edge(i - 1, i)) {
            return false;
        } 
    }
    if(edge_count()!=vertex_count()-1)
      return false;

   int A[vertex_count()+1];
    A[0] = 0;
    A[1] = vertex_weights[0];
    for(int i=2; i<vertex_count()+1; ++i){
       A[i] = std::max(A[i-1],A[i-2] + vertex_weights[i-1]);
    }

   for(int i=vertex_count(); i>0; --i){
      if(A[i]>A[i-1]){
         S.insert(i-1);
         --i;
      }
         
   }
   
   return true;
}

bool Graph::all_pairs_shortest_paths(std::vector<std::vector<int>>& weights) const {
   std::vector<std::vector<std::vector<int>>> A;

   //init A to empty (n+1)*n*n size
   for(int i = 0; i < vertex_count() + 1; ++i) {
        A.push_back(std::vector<std::vector<int>>());
        for(int j = 0; j < vertex_count(); ++j) {
            A[i].push_back(std::vector<int>());
            for(int k = 0; k < vertex_count(); ++k) {
                A[i][j].push_back(std::numeric_limits<int>::max());
            }
        }
    }

   //base cases
   int edge_weight;
   for(int u=0; u < vertex_count(); ++u){
      for(int v=0; v<vertex_count(); ++v){
         if(u==v){
            A[0][u][v] = 0;
         } else if(get_edge(u,v,edge_weight)){
            A[0][u][v]=edge_weight;
         } else {
            A[0][u][v]= std::numeric_limits<int>::max();
         }
      }
   }

   //iterative solve 
   for(int k=1; k<vertex_count(); ++k){
      for(int u=0; u<vertex_count(); ++u){
         for(int v=0; v<vertex_count(); ++v){
            if(A[k-1][u][k]==std::numeric_limits<int>::max() || A[k-1][k][v]==std::numeric_limits<int>::max()){
               A[k][u][v] = A[k-1][u][v];
            } else {
               A[k][u][v]= std::min(A[k-1][u][v],A[k-1][u][k]+A[k-1][k][v]);
            }
         }
      }
   }

   //neg cycle check
   for(int u=0; u<vertex_count(); ++u){
      if(A[vertex_count()-1][u][u] < 0){
         return false;
      }
   }

   //init weight values
   for(int i = 0; i < vertex_count(); ++i) {
        weights.push_back(std::vector<int>());
        for(int j = 0; j < vertex_count(); ++j) {
            weights[i].push_back(0);
        }
    }

   //set weight values
   for(int u=0; u<vertex_count(); ++u){
      for(int v=0; v<vertex_count(); ++v){
         weights[u][v] = A[vertex_count()-1][u][v];
      }
   }
   
   return true;
}
