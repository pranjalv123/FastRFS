#ifndef __RFSUPPORTANALYSIS_HPP__
#define __RFSUPPORTANALYSIS_HPP__

#include "phylonaut/Analysis/Analysis.hpp"
#include "phylokit/newick.hpp"
#include "phylokit/Clade.hpp"

#include <sstream>
#include <vector>
#include <string>

class RFSupportAnalysis : public Analysis {
  std::vector<std::string> tree_strs;
public:
  virtual std::string analyze(TaxonSet& ts, std::vector<Clade>& clades, TripartitionScorer& tps);
  std::stringstream toTree(TaxonSet& ts, clade_bitset& bs, std::vector<Clade>& clades, TripartitionScorer& tps);
  RFSupportAnalysis(std::vector<std::string>& trees) : tree_strs(trees){}
};

#endif
