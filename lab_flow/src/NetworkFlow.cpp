#include <vector>
#include <algorithm>
#include <set>

#include "graph/graph.h"

#include "NetworkFlow.h"

int min(int a, int b) {
  if (a<b)
    return a;
  else return b;
}

NetworkFlow::NetworkFlow(Graph & startingGraph, Vertex source, Vertex sink) :
  g_(startingGraph), residual_(Graph(true,true)), flow_(Graph(true,true)), source_(source), sink_(sink) {

  // YOUR CODE HERE
  maxFlow_ = 0;

  vector<Vertex> vertices = g_.getVertices();
  for (auto const & v : vertices) {
    flow_.insertVertex(v);
    residual_.insertVertex(v);
  }

  for (auto const & u : vertices) {
    vector<Vertex> adjs = g_.getAdjacent(u);
    for (auto const & v : adjs) {
      int w = g_.getEdgeWeight(u, v);

      flow_.insertEdge(u, v);
      flow_.setEdgeWeight(u, v, 0);

      residual_.insertEdge(u, v);
      residual_.setEdgeWeight(u, v, w);

      if (!residual_.edgeExists(v, u)) {
        residual_.insertEdge(v, u);
        residual_.setEdgeWeight(v, u, 0);
      }
    }
  }
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the helper function.
   *
   * @param source  The starting (current) vertex
   * @param sink    The destination vertex
   * @param path    The vertices in the path
   * @param visited A set of vertices we have visited
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, 
  std::vector<Vertex> &path, std::set<Vertex> &visited) {

  if (visited.count(source) != 0)
    return false;
  visited.insert(source);

  if (source == sink) {
    return true;
  }

  vector<Vertex> adjs = residual_.getAdjacent(source);
  for(auto it = adjs.begin(); it != adjs.end(); it++) {
    if (visited.count(*it) == 0 && residual_.getEdgeWeight(source,*it) > 0) {
      path.push_back(*it);
      if (findAugmentingPath(*it,sink,path,visited))
        return true;
      else {
        path.pop_back();
      }
    }
  }

  return false;
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the main function.  It initializes a set to keep track of visited vertices.
   *
   * @param source The starting (current) vertex
   * @param sink   The destination vertex
   * @param path   The vertices in the path
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, std::vector<Vertex> &path) {
   std::set<Vertex> visited;
   path.clear();
   path.push_back(source);
   return findAugmentingPath(source,sink,path,visited);
}

  /**
   * pathCapacity - Determine the capacity of a path in the residual graph.
   *
   * @param path   The vertices in the path
   */

int NetworkFlow::pathCapacity(const std::vector<Vertex> & path) const {
  // YOUR CODE HERE
  if (path.size() < 2) {
    return 0;
  }

  int capacity = residual_.getEdgeWeight(path[0], path[1]);

  for (size_t i = 1; i < path.size() - 1; i++) {
    capacity = min(capacity, residual_.getEdgeWeight(path[i], path[i + 1]));
  }

  return capacity;
}

  /**
   * calculateFlow - Determine the maximum flow of the entire graph.
   * Sets the member variable `maxFlow_` to the computed maximum flow, and updates the
   * residual graph and flow graph according to the algorithm.
   *
   * @return The network flow graph.
   */

const Graph & NetworkFlow::calculateFlow() {
  // YOUR CODE HERE
  vector<Vertex> p;

  while (findAugmentingPath(source_, sink_, p)) {
    int cap = pathCapacity(p);
    maxFlow_ += cap;

    for (size_t i = 0; i < p.size() - 1; i++) {
      Vertex u = p[i];
      Vertex v = p[i + 1];

      if (flow_.edgeExists(u, v)) {
        flow_.setEdgeWeight(u, v, flow_.getEdgeWeight(u, v) + cap);
      } else {
        flow_.setEdgeWeight(v, u, flow_.getEdgeWeight(v, u) - cap);
      }

      residual_.setEdgeWeight(u, v, residual_.getEdgeWeight(u, v) - cap);
      residual_.setEdgeWeight(v, u, residual_.getEdgeWeight(v, u) + cap);
    }
  }

  return flow_;
}

int NetworkFlow::getMaxFlow() const {
  return maxFlow_;
}

const Graph & NetworkFlow::getGraph() const {
  return g_;
}

const Graph & NetworkFlow::getFlowGraph() const {
  return flow_;
}

const Graph & NetworkFlow::getResidualGraph() const {
  return residual_;
}

