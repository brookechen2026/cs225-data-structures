#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <queue>

// Curiously recurring template pattern
// You don't actually need to extend this class,
// but here are the functions you need to implement
template <class DerivedDescriptor>
class GenericNodeDescriptor
{
	public:
	virtual bool isNull() const = 0;
	virtual std::string key() const = 0;
	virtual DerivedDescriptor left() const = 0;
	virtual DerivedDescriptor right() const = 0;
};

namespace private_print_functions
{
	template <class NodeDescriptor>
	int getPrintHeight(NodeDescriptor croot);

  template <class NodeDescriptor>
	int getPrintWidth(NodeDescriptor croot);

	template <class NodeDescriptor>
	void printSubtree(NodeDescriptor croot,
    std::vector<std::string> & output,
    int left,
    int top,
    int curr_width);
    
  int getStartIdx(const std::vector<std::string> & output);

  template <class NodeDescriptor>
  struct PrintInfo {
    int anchor; // where branches should connect to
    int labelStart; // start coordinate of the label
    // dimensions of the subtree's bounding box
    int width;
    int depth;
    int leftIdx;
    int rightIdx;

    NodeDescriptor node;
    PrintInfo(NodeDescriptor n): width(0), depth(-1), node(n) {}
  };
  /*
  All coordinates are with respect to the left edge of the bounding box
                    anchor
                      |      _______parent___
        --------------v_____/                \...
      : |           label           | |                   |
      d |           ^               | |                   |
      e |           |               | |                   |
      p |       labelStart          | |       (sibling)   |
      t |                           | |                   |
      h |                           | |                   |
      : |                           | |                   |
        |___________________________| |___________________|
        -----------width-------------
  */
  template <class NodeDescriptor>
  void resolveTreeStructure(std::vector<PrintInfo<NodeDescriptor>>& trees, NodeDescriptor root);

  template <class NodeDescriptor>
  void resolveTreeStructureCompact(std::vector<PrintInfo<NodeDescriptor>>& trees, NodeDescriptor root);

  
  template <class NodeDescriptor>
  void printSubtree(int idx, const std::vector<PrintInfo<NodeDescriptor>> trees, std::vector<std::string>& output, int top, int left);
}

template <class NodeDescriptor>
void printTree(NodeDescriptor root, std::ostream & out = std::cout, bool compact = false) {
  if (root.isNull()) {
    out << "(empty)\n";
    return;
  }
  using namespace private_print_functions;
  std::vector<PrintInfo<NodeDescriptor>> subroots;
  if (!compact) {
    resolveTreeStructure(subroots, root);
  } else {
    resolveTreeStructureCompact(subroots, root);
  }
  int height = subroots[0].depth, width = subroots[0].width;
  std::vector<std::string> output(height);

	for (size_t i = 0; i < output.size(); ++i)
    output[i] = std::string(width, ' ');
  
  printSubtree(0, subroots, output, 0, 0);

  // Trim beginning whitespace (hack)
  int idx = getStartIdx(output);

  for (size_t row = 0; row < output.size(); ++row) {
    while (output[row].back() == ' ') {
      output[row].pop_back();
    }
		out << output[row].substr(idx) << std::endl;
  }
}

/* NONCOMPACT TREES
  The bounding box for every subtree has odd width so that it can be centered
        left    center  right
          |       |       |
  ---------------- ----------------
  |          _rootlabel_          |
  |        _/     |     \_        |
  |---------------|---------------|
  |               |               |
  |     left      |     right     |
  |    subtree    |    subtree    |
  |               |               |
  |_______________|_______________|

  The '/' and '\' characters are in the middle of the ranges [left, center] and [center, right].
  This is a unambiguous since every subtree has odd width, except if we are at a leaf level, in which case we bias
  towards the outside. Branches go just before reaching left or right.
  Labels are centered with a bias to the left (TODO(?): make bias outwards)
  Examples:
                          ________dev________                                  
                _________/                   \_________                  
           ___bob___                               _hagrid__            
     ____/         \____                     ____/         \____                        
  addie              charlie             fatima               liam_            
       \_           _/     \_           _/     \_           _/     \_          
       alice   chandler    chloe     ellen    gerald    janice   sebastian          
          

           1.23_                         __hello__
        __/     \__                   __/         \__                  
      _0_        56.4                hey            hi            
     /   \       /   \   
  -1.2   0.5  3.14   60
*/
/*
  COMPACT TREES
  Compact trees only use as much width as is needed: Instead of rendering this:
                       _______1_______                        
               _______/               \_______                
           ___2___                         ___4___            
       ___/       \___                 ___/       \___        
     _3               9              _5               6       
   _/                              _/                         
  8                               7                           
                                 /                            
                               10
  We render this:                          
     1__    
    /   \_  
    2      4 
  / \    / \
  3 9    5 6
 /      /   
 8      7   
      /    
     10  
  This lets us render larger trees without distorting them with newlines:
                             ________1___________                     
                    ________/                    \__________          
                 __4_____                               ____2___      
              __/        \____                      ___/        \__   
            _5__             _7_               ____6__            _3  
           /    \_          /   \          ___/       \_        _/  \ 
        _11       9       28    12        8__          19_    15_   13
      _/   \     /      _/  \   / \      /   \__       /  \   /  \    
    16     25   14    29_   35 27 30    10     20     34  21 24  31   
   /  \         /    /   \         \    /     /  \    /   /      /    
  23  17       22   40   38        33  32   39   47  42  26     43    
   \   \       /     \   /             /    / \      /                
   41  18     37     49 45            50   48 44    46                
        \                                                             
        36                                                              
*/
namespace private_print_functions
{
	template <class NodeDescriptor>
	int getPrintHeight(NodeDescriptor croot)
	{
		if (croot.isNull())
			return -1;
		int l_height = getPrintHeight(croot.left());
		int r_height = getPrintHeight(croot.right());
		return 1 + std::max(l_height, r_height);
	}

  template <class NodeDescriptor>
  int getPrintWidth(NodeDescriptor croot) {
    if (croot.isNull()) return 0;
    int l_width = getPrintWidth(croot.left());
    int r_width = getPrintWidth(croot.right());
    int hedge = croot.key().length();
    if (hedge % 2 == 0) hedge++; // Ensure we have an odd width
    return std::max(2 * std::max(l_width, r_width) + 1, hedge);
  }

  // inline to prevent duplicate definitions
  inline int getStartIdx(const std::vector<std::string> & output) {
    for (size_t col = 0; col < output[0].length(); col++) {
      for (size_t row = 0; row < output.size(); row++) {
        if (output[row][col] != ' ') return col;
      }
    }
    return -1; // output should never be just whitespace
  }

  template <class NodeDescriptor>
  void resolveTreeStructure(std::vector<PrintInfo<NodeDescriptor>>& trees, NodeDescriptor root) {
    int depth = 2 * getPrintHeight(root) + 1;
    std::cout << "Depth is " << depth << "\n";
    int width = getPrintWidth(root);

    trees.push_back(PrintInfo<NodeDescriptor>(root));

    // Level-order traversal
    std::queue<std::pair<int, int>> queue; // idx, level
    queue.push({0, 0});
    int level = 0;

    while (!queue.empty()) {
      auto [idx, l] = queue.front();
      auto& info = trees[idx];
      queue.pop();
      if (trees[idx].node.isNull()) continue;
      if (l > level) {
        level++;
        width /= 2;
        depth -= 2;
      }
      trees[idx].width = width;
      trees[idx].depth = depth;
      trees[idx].anchor = width / 2;
      trees[idx].labelStart = trees[idx].anchor - trees[idx].node.key().length() / 2;

      auto left = trees[idx].node.left(), right = trees[idx].node.right();
      queue.push({trees.size(), l + 1});
      trees[idx].leftIdx = trees.size();
      trees.push_back(PrintInfo<NodeDescriptor>(left));

      queue.push({trees.size(), l + 1});
      trees[idx].rightIdx = trees.size();
      trees.push_back(PrintInfo<NodeDescriptor>(right));
    }
  }

  template <class NodeDescriptor>
  void resolveTreeStructureCompact(std::vector<PrintInfo<NodeDescriptor>>& trees, NodeDescriptor root) {
    trees.push_back(PrintInfo(root));
    if (root.isNull()) return;
    int leftIdx = trees.size();
    resolveTreeStructureCompact(trees, root.left());
    int rightIdx = trees.size();
    resolveTreeStructureCompact(trees, root.right());
    PrintInfo info(root);
    info.leftIdx = leftIdx;
    info.rightIdx = rightIdx;
    PrintInfo left = trees[leftIdx];
    PrintInfo right = trees[rightIdx];
    info.depth = 2 + std::max(left.depth, right.depth);
    info.leftIdx = leftIdx;
    info.rightIdx = rightIdx;
    info.width = std::max(1 + left.width + right.width, (int)root.key().length());
    info.anchor = left.width;
    info.labelStart = std::max(info.anchor - ((int)root.key().length() + 1) / 2, 0);
    trees[leftIdx - 1] = info;
  }

  
  template <class NodeDescriptor>
  void printSubtree(int idx, const std::vector<PrintInfo<NodeDescriptor>> trees, std::vector<std::string>& output, int top, int left) {
    using namespace private_print_functions;
    auto info = trees[idx];
    if (info.node.isNull()) return;

    PrintInfo<NodeDescriptor> leftChild = trees[info.leftIdx];
    PrintInfo<NodeDescriptor> rightChild = trees[info.rightIdx];

    // Print left child
		if (!leftChild.node.isNull()) {
      int leftBranch = left + (leftChild.anchor + info.anchor) / 2; // biases to the left
      for (int i = leftBranch + 1; i < left + info.anchor; i++)
        output[top][i] = '_';
      for (int i = left + leftChild.anchor + 1; i < leftBranch; i++)
        output[top + 1][i] = '_';
      output[top + 1][leftBranch] = '/';
      printSubtree(info.leftIdx, trees, output, top + 2, left);
		}

		// Print right child
		if (!rightChild.node.isNull()) {
			int rightBranch = left + (2 * info.anchor + rightChild.anchor + 2) / 2; // biases to the right
      for (int i = left + info.anchor + 1; i < rightBranch; i++) 
        output[top][i] = '_';
      for (int i = rightBranch + 1; i < left + info.anchor + 1 + rightChild.anchor; i++)
        output[top + 1][i] = '_';
      output[top + 1][rightBranch] = '\\';
      printSubtree(info.rightIdx, trees, output, top + 2, left + info.anchor + 1);
		}

    std::string label = info.node.key();
    for (int i = 0; i < (int)label.length(); i++) {
      output[top][i + left + info.labelStart] = label[i];
    }
  }
}