#ifndef __RFSUPPORTANALYSIS_HPP__
#define __RFSUPPORTANALYSIS_HPP__

#include <phylonaut/Analysis.hpp>
#include <newick.hpp>
#include <Clade.hpp>

class RFSupportAnalysis : public Analysis {
  vector<string> tree_strs;
public:
  virtual string analyze(TaxonSet& ts, vector<Clade>& clades, TripartitionScorer& tps);
  stringstream toTree(TaxonSet& ts, clade_bitset& bs, vector<Clade>& clades, TripartitionScorer& tps);
  RFSupportAnalysis(vector<string>& trees) : tree_strs(trees){}
};

#endif
