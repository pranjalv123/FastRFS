#include "RFSupportAnalysis.hpp"

#include "phylonaut/TripartitionScorer/TripartitionScorer.hpp"
#include <sstream>
#include <vector>
#include <string>
using std::stringstream;
using std::string;
using std::vector;

double support(TaxonSet& ts, clade_bitset& bs, vector<unordered_set<Clade>>& trees, vector<Clade>& tree_taxa) {
    double good = 0;
    double bad = 0;

    for (size_t i = 0; i < trees.size(); i++) {
      Clade this_clade(ts, bs);
      bool is_compatible = 1;
      if (this_clade.overlap_size(tree_taxa[i]) < 2)
        continue;

      this_clade = this_clade.overlap(tree_taxa[i]);
      for (const Clade& c : trees[i]) {
        if ( ! c.compatible(this_clade) ) {
          is_compatible = 0;
          break;
        }
      }

      if (is_compatible) {
        good ++;
      } else {
        bad ++;
      }
    }

    double support = good/(good+bad);
    return support;
}


stringstream RFSupportAnalysis::toTree(TaxonSet& ts, clade_bitset& bs, vector<Clade>& clades, TripartitionScorer& tps) {

  vector<unordered_set<Clade>> trees;
  vector<Clade> tree_taxa;

  for (string& t : tree_strs){
    Clade tree_clade = newick_to_taxa(t, ts);
    tree_taxa.push_back(tree_clade);

    unordered_set<Clade> tclades;
    newick_to_clades(t, ts, tclades);
    trees.push_back(tclades);
  }


  auto& subclades = tps.get_subclades(bs)[0];
  stringstream ss;
  if (bs.popcount() == 2) {
    vector<Taxon> tv;
    for (Taxon t : bs) {
      tv.push_back(t);
    }
    ss << "(" << ts[tv[0]] << "," << ts[tv[1]] << "):" << support(ts, bs, trees, tree_taxa);
  }

  else if (bs.popcount() == 1) {
    ss << ts[*(bs.begin())];
  }

  else {

    ss << "(" << toTree(ts, subclades.first, clades, tps).str() << "," << toTree(ts, subclades.second, clades, tps).str() << "):" << support(ts, bs, trees, tree_taxa);
  }

  return ss;
}

string RFSupportAnalysis::analyze(TaxonSet& ts, vector<Clade>& clades, TripartitionScorer& tps) {
  stringstream ss;
  ss << toTree(ts, ts.taxa_bs, clades, tps).str();
  ss << ";";
  return ss.str();
}
