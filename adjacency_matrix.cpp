//----------------------------------------------------------------------
// NAME: Sami Blevens
// FILE: adjacency_matrix.h
// DATE: Spring 2021
// DESC: Adjacency matrix implementation of the graph interface
//----------------------------------------------------------------------

#ifndef ADJACENCY_MATRIX_CPP
#define ADJACENCY_MATRIX_CPP

#include "adjacency_matrix.h"
#include <iostream>

//default constructor
AdjacencyMatrix::AdjacencyMatrix(int vertex_count) {
   number_of_vertices = vertex_count;
   number_of_edges = 0;
   adj_matrix = new int*[number_of_vertices*number_of_vertices];
   for(int i=0; i < (number_of_vertices*number_of_vertices); ++i){
      adj_matrix[i] = nullptr;
   }
}

//destructor
AdjacencyMatrix::~AdjacencyMatrix(){
   make_empty();
}

//delete the adjacency matrix
void AdjacencyMatrix::make_empty(){
   //empty array
   for(int i=0; i<number_of_vertices*number_of_vertices; ++i){
      delete adj_matrix[i];
      adj_matrix[i]=nullptr; 
   }
   //delete adj matrix
   number_of_vertices = 0;
   number_of_edges = 0;
   delete[] adj_matrix;
   adj_matrix = nullptr;
}

//copy constructor
AdjacencyMatrix::AdjacencyMatrix(const AdjacencyMatrix& rhs){
   number_of_edges = 0;
   number_of_vertices = 0;
   adj_matrix = nullptr;
   *this = rhs;
}

//assignment operator
AdjacencyMatrix& AdjacencyMatrix::operator=(const AdjacencyMatrix& rhs){
   if(this == &rhs){
      return *this;
   }

   make_empty();

   number_of_vertices = rhs.number_of_vertices;
   number_of_edges = rhs.number_of_edges;
   int length = number_of_vertices*number_of_vertices;

   adj_matrix = new int*[length];
   for(int i=0; i < length; ++i){
      if(rhs.adj_matrix[i]) {
         adj_matrix[i] = new int;
         *adj_matrix[i] = *rhs.adj_matrix[i];
      } else {
         adj_matrix[i] = nullptr;
      }
   }

   return *this;
}

//add or update an edge between existing vertices in the graph
void AdjacencyMatrix::set_edge(int v1, int edge_label, int v2){
   int i = index(v1,v2);
   
   if(adj_matrix[i]){
      *adj_matrix[i] = edge_label;
   } else {
      adj_matrix[i] = new int;
      *adj_matrix[i] = edge_label;
      ++number_of_edges;
   }
}

//check if the given edge is in the graph
bool AdjacencyMatrix::has_edge(int v1, int v2) const{
   int i = index(v1,v2);
   if(adj_matrix[i]){
      return true;
   } else {
     return false;
   }  
}

//returns true if there is an edge between v1 and v2 in the graph
//output parameter returns the edge label
bool AdjacencyMatrix::get_edge(int v1, int v2, int& edge) const{
  int i = index(v1,v2);
   if(adj_matrix[i]){
     edge = *adj_matrix[i];
     return true;
  } else {
     return false;
  }
  
}

void AdjacencyMatrix::remove_edge(int v1, int v2){
   if(has_edge(v1,v2)){
      int i = index(v1,v2);
      delete adj_matrix[i];
      adj_matrix[i]=nullptr; 
      --number_of_edges;
   }
}

//get all vertices on an outgoing edge from given vertex - v "row"
void AdjacencyMatrix::connected_to(int v1, std::list<int>& vertices) const{
   for(int i=0; i<number_of_vertices; ++i){
      if(adj_matrix[index(v1,i)]){
         vertices.push_back(i);
      }
   }
}

//get all vertices on an incoming edge to given vertex - v "column"
void AdjacencyMatrix::connected_from(int v2, std::list<int>& vertices) const{
   for(int i=0; i<number_of_vertices; ++i){
      if(adj_matrix[index(i,v2)]){
         vertices.push_back(i);
      }
   }
}

//get all vertices adjacent to a vertex, that is, connected to or connect from
//the vertex (may return duplicate vertices)
void AdjacencyMatrix::adjacent(int v, std::list<int>& vertices) const{
  connected_to(v,vertices);
  connected_from(v,vertices);
}

//get number of nodes in the graph
int AdjacencyMatrix::vertex_count() const{
  return number_of_vertices;
}

//get number of edges in the graph
int AdjacencyMatrix::edge_count() const{
  return number_of_edges;
}

//return the matrix index for edge x->y
int AdjacencyMatrix::index(int x, int y) const {
  return (x*number_of_vertices) + y;
}

#endif