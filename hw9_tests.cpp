//----------------------------------------------------------------------
// Name: 
// File: hw9_tests.cpp
// Date: Spring 2021
// Desc: Unit tests for list and matrix graph implementations
//----------------------------------------------------------------------


#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>
#include <gtest/gtest.h>
#include "graph.h"
#include "adjacency_list.h"
#include "adjacency_matrix.h"


using namespace std;


//----------------------------------------------------------------------
// Helper functions for testing
//----------------------------------------------------------------------

void print_graph(const Graph& g)
{
  int n = g.vertex_count();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (g.has_edge(i,j)) {
        int edge_label;
        g.get_edge(i, j, edge_label);
        cout << "(" << i << "," << edge_label << "," << j << ")" << endl;
      }
    }
  }
}

void print_weight_vector(const vector<vector<int>>& weights, int n) {
  for (int u = 0; u < n; ++u) {
    for (int v = 0; v < n; ++v) {
      if (weights[u][v] == std::numeric_limits<int>::max())
        cout << "inf" << " ";
      else
        cout << weights[u][v] << " ";
    }
    cout << endl;
  }
}


//----------------------------------------------------------------------
// Adjacency List Tests
//----------------------------------------------------------------------

TEST(AdjacencyListTest, BasicMaxIndependentSet) {
  AdjacencyList g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  Set S;
  int weights[4] = {1, 4, 5, 4};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(2, S.size());
  // check members
  ASSERT_EQ(true, S.count(1));
  ASSERT_EQ(true, S.count(3));
}

TEST(AdjacencyListTest, SmallMaxIndependentSet) {
  AdjacencyList g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  Set S;
  int weights[6] = {3, 2, 1, 6, 4, 5};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(3, S.size());
  // check members
  ASSERT_EQ(true, S.count(0));
  ASSERT_EQ(true, S.count(3));
  ASSERT_EQ(true, S.count(5));
}


// TODO: Add two additional max independent set tests
//Test if graph is not a path graph
TEST(AdjacencyListTest, NotPathGraphTest){
  AdjacencyList g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3 ,0 ,0);
  Set s;
  int weights[4] = {1,4,3,2};
  ASSERT_EQ(false, g.path_max_independent_set(weights, s));
}

//larger graph with some repeated weights
TEST(AdjacencyListTest, LargerMaxIndependentSet){
  AdjacencyList g(9);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  g.set_edge(5 ,0, 6);
  g.set_edge(6, 0, 7);
  g.set_edge(7, 0, 8);
  Set s;
  int weights[9] ={6,1,1,1,7,1,1,1,2};
  ASSERT_EQ(true, g.path_max_independent_set(weights,s));
  //check size
  ASSERT_EQ(5, s.size());
  //check members
  ASSERT_EQ(true, s.count(0));
  ASSERT_EQ(true, s.count(2));
  ASSERT_EQ(true, s.count(4));
  ASSERT_EQ(true, s.count(6));
  ASSERT_EQ(true, s.count(8));
}


TEST(AdjacencyListTest, BasicAllPairsShortestPaths) {
  AdjacencyList g(5);
  g.set_edge(0, -10, 1);
  g.set_edge(0, 2, 3);
  g.set_edge(1, -10, 2);
  g.set_edge(3, -4, 4);
  g.set_edge(4, 5, 2);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  // Note that you can call the following to help debug: 
  //   print_weight_vector(weights, 5);
  ASSERT_EQ(-10, weights[0][1]);
  ASSERT_EQ(-20, weights[0][2]);
  ASSERT_EQ(2, weights[0][3]);
  ASSERT_EQ(-2, weights[0][4]);
  ASSERT_EQ(-10, weights[1][2]);
  ASSERT_EQ(1, weights[3][2]);
  ASSERT_EQ(-4, weights[3][4]);
  ASSERT_EQ(5, weights[4][2]);
  // Note should also test MAX_INT cases above
}


// TODO: Add three additional max all pairs  tests
//Check for negative cycle
TEST(AdjacencyListTest, NegativeCycleTest){
  AdjacencyList g(4);
  g.set_edge(0,1,1);
  g.set_edge(1,-4,2);
  g.set_edge(1,-5,2);
  g.set_edge(2,2,1);
  vector<vector<int>> weights;
  ASSERT_EQ(false, g.all_pairs_shortest_paths(weights));
}

//Test max_int cases
TEST(AdjacencyListTest, ShortestPathNoPath){
  AdjacencyList g(5);
  g.set_edge(0, -10, 4);
  g.set_edge(0, 2, 3);
  g.set_edge(2, 3, 1);
  g.set_edge(3, -4, 4);
  g.set_edge(2, 10, 4);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[4][2]);
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[1][2]);
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[0][2]);
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[3][1]);
}

//Larger Graph
TEST(AdjacencyListTest, LargerAllPairsShortestPaths){
  AdjacencyList g(10);
  g.set_edge(0,1,1);
  g.set_edge(0,5,4);
  g.set_edge(0,-5,5);
  g.set_edge(1,2,2);
  g.set_edge(2,-3,3);
  g.set_edge(3,1,8);
  g.set_edge(4,-1,5);
  g.set_edge(5,6,6);
  g.set_edge(6,7,7);
  g.set_edge(7,-5,9);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  ASSERT_EQ(-5,weights[0][5]);
  ASSERT_EQ(3,weights[0][9]);
  ASSERT_EQ(5,weights[0][4]);
  ASSERT_EQ(8,weights[0][7]);
  ASSERT_EQ(1,weights[0][8]);
  ASSERT_EQ(0,weights[0][3]);
  ASSERT_EQ(3,weights[0][2]);
  ASSERT_EQ(3,weights[0][2]);
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[8][0]);
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[7][5]);
}

//----------------------------------------------------------------------
// Adjacency Matrix Tests
//----------------------------------------------------------------------


// TODO: Add your tests above but for your adjacency matrix
TEST(AdjacencyMatrixTest, BasicMaxIndependentSet) {
  AdjacencyMatrix g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  Set S;
  int weights[4] = {1, 4, 5, 4};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(2, S.size());
  // check members
  ASSERT_EQ(true, S.count(1));
  ASSERT_EQ(true, S.count(3));
}

TEST(AdjacencyMatrixTest, SmallMaxIndependentSet) {
  AdjacencyMatrix g(6);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  Set S;
  int weights[6] = {3, 2, 1, 6, 4, 5};
  ASSERT_EQ(true, g.path_max_independent_set(weights, S));
  // check size  
  ASSERT_EQ(3, S.size());
  // check members
  ASSERT_EQ(true, S.count(0));
  ASSERT_EQ(true, S.count(3));
  ASSERT_EQ(true, S.count(5));
}


// TODO: Add two additional max independent set tests
//Test if graph is not a path graph
TEST(AdjacencyMatrixTest, NotPathGraphTest){
  AdjacencyMatrix g(4);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3 ,0 ,0);
  Set s;
  int weights[4] = {1,4,3,2};
  ASSERT_EQ(false, g.path_max_independent_set(weights, s));
}

//larger graph with some repeated weights
TEST(AdjacencyMatrixTest, LargerMaxIndependentSet){
  AdjacencyMatrix g(9);
  g.set_edge(0, 0, 1);
  g.set_edge(1, 0, 2);
  g.set_edge(2, 0, 3);
  g.set_edge(3, 0, 4);
  g.set_edge(4, 0, 5);
  g.set_edge(5 ,0, 6);
  g.set_edge(6, 0, 7);
  g.set_edge(7, 0, 8);
  Set s;
  int weights[9] ={6,1,1,1,7,1,1,1,2};
  ASSERT_EQ(true, g.path_max_independent_set(weights,s));
  //check size
  ASSERT_EQ(5, s.size());
  //check members
  ASSERT_EQ(true, s.count(0));
  ASSERT_EQ(true, s.count(2));
  ASSERT_EQ(true, s.count(4));
  ASSERT_EQ(true, s.count(6));
  ASSERT_EQ(true, s.count(8));
}


TEST(AdjacencyMatrixTest, BasicAllPairsShortestPaths) {
  AdjacencyMatrix g(5);
  g.set_edge(0, -10, 1);
  g.set_edge(0, 2, 3);
  g.set_edge(1, -10, 2);
  g.set_edge(3, -4, 4);
  g.set_edge(4, 5, 2);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  // Note that you can call the following to help debug: 
  //   print_weight_vector(weights, 5);
  ASSERT_EQ(-10, weights[0][1]);
  ASSERT_EQ(-20, weights[0][2]);
  ASSERT_EQ(2, weights[0][3]);
  ASSERT_EQ(-2, weights[0][4]);
  ASSERT_EQ(-10, weights[1][2]);
  ASSERT_EQ(1, weights[3][2]);
  ASSERT_EQ(-4, weights[3][4]);
  ASSERT_EQ(5, weights[4][2]);
  // Note should also test MAX_INT cases above
}


// TODO: Add three additional max all pairs  tests
//Check for negative cycle
TEST(AdjacencyMatrixTest, NegativeCycleTest){
  AdjacencyMatrix g(4);
  g.set_edge(0,1,1);
  g.set_edge(1,-4,2);
  g.set_edge(1,-5,2);
  g.set_edge(2,2,1);
  vector<vector<int>> weights;
  ASSERT_EQ(false, g.all_pairs_shortest_paths(weights));
}

//Test max_int cases
TEST(AdjacencyMatrixTest, ShortestPathNoPath){
  AdjacencyMatrix g(5);
  g.set_edge(0, -10, 4);
  g.set_edge(0, 2, 3);
  g.set_edge(2, 3, 1);
  g.set_edge(3, -4, 4);
  g.set_edge(2, 10, 4);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[4][2]);
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[1][2]);
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[0][2]);
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[3][1]);
}

//Larger Graph
TEST(AdjacencyMatrixTest, LargerAllPairsShortestPaths){
  AdjacencyMatrix g(10);
  g.set_edge(0,1,1);
  g.set_edge(0,5,4);
  g.set_edge(0,-5,5);
  g.set_edge(1,2,2);
  g.set_edge(2,-3,3);
  g.set_edge(3,1,8);
  g.set_edge(4,-1,5);
  g.set_edge(5,6,6);
  g.set_edge(6,7,7);
  g.set_edge(7,-5,9);
  vector<vector<int>> weights;
  ASSERT_EQ(true, g.all_pairs_shortest_paths(weights));
  ASSERT_EQ(-5,weights[0][5]);
  ASSERT_EQ(3,weights[0][9]);
  ASSERT_EQ(5,weights[0][4]);
  ASSERT_EQ(8,weights[0][7]);
  ASSERT_EQ(1,weights[0][8]);
  ASSERT_EQ(0,weights[0][3]);
  ASSERT_EQ(3,weights[0][2]);
  ASSERT_EQ(3,weights[0][2]);
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[8][0]);
  ASSERT_EQ(std::numeric_limits<int>::max(), weights[7][5]);
}



int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

