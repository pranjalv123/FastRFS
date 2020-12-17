#include "FastRFTripartitionScorer.hpp"
#include "phylonaut/TripartitionScorer/TripartitionScorer.hpp"
#include <glog/logging.h>
#include "phylokit/newick.hpp"

#include <limits>
#include <fstream>
#include <cmath>

bool FastRFTripartitionScorer::better(double newscore, double oldscore) {
  return newscore > oldscore;
}

void FastRFTripartitionScorer::setup(Config& conf, vector<Clade>& clades)
{
  string tree;
  ifstream file(treesfile);
  total_weight = 0;
  total_leaves = 0;
  n_trees = 0;
  while(getline(file, tree)) {
    addSourceTree(tree);
    n_trees++; 
    DLOG(INFO) << n_trees << "\t" << total_weight << endl;
  }
  LOG(INFO) << "Total Weight: " << total_weight << endl;

  for (auto& i : clade_weights) {
    bipartitions.push_back(&(i.first));
    weights.push_back(i.second);
  }
  


  LOG(INFO) << "Making possibilities lists\n";


  size_t n = 0;
  for (const Clade& clade : clades) {
    BitVectorFixed& bvf_a1 = (*(possibleA1.emplace(clade, weights.size()).first)).second;
    BitVectorFixed& bvf_a2 = (*(possibleA2.emplace(clade, weights.size()).first)).second;
    BitVectorFixed& bvf_b1 = (*(possibleB1.emplace(clade, weights.size()).first)).second;
    BitVectorFixed& bvf_b2 = (*(possibleB2.emplace(clade, weights.size()).first)).second;
    
    for (unsigned int i = 0; i < bipartitions.size(); i++) {
      const Bipartition& bp = *(bipartitions[i]);
      if (clade.contains(bp.a1) && clade.overlap(bp.a2).size() == 0) {
	      bvf_a1.set(i);
      }
      if (clade.overlap(bp.a1).size() > 0) {
	      bvf_b1.set(i);
      }
      if (clade.contains(bp.a2) && clade.overlap(bp.a1).size() == 0) {
	      bvf_a2.set(i);
      }
      if (clade.overlap(bp.a2).size() > 0) {
	      bvf_b2.set(i);
      }
    }
    n++;
    if (n%1000 == 0 || n == clades.size()) {
      LOG(INFO) << "prepared " << n << "/" << clades.size() <<  endl;
    }
    
  }
}

void FastRFTripartitionScorer::addSourceTree(string tree) {
  unordered_set<Clade> clades;
  newick_to_clades(tree, ts(), clades);

  Clade tree_clade = newick_to_taxa(tree, ts() );
  
  total_leaves += tree_clade.size();

  unordered_set<Clade> clade_complements;
 

 
  int n = 0;
  
  for (const Clade& clade : clades) {
    Clade comp(tree_clade.minus(clade));
    //    Clade comp(clade.complement());
    if (comp.size() && clade.size()) {
      clade_weights[Bipartition(clade, comp)] += 1;
      n++;
    }
  }
  total_weight += n;
}

bool FastRFTripartitionScorer::matches(const Tripartition& t, const Bipartition& bp) {

  if (t.a1.contains(bp.a1) && (t.a1.overlap(bp.a2).size() == 0) && (t.a2.overlap(bp.a2).size() > 0)) {
    return true;
  }
  if (t.a2.contains(bp.a1) && (t.a2.overlap(bp.a2).size() == 0) && (t.a1.overlap(bp.a2).size() > 0)) {
    return true;
  }

  if (t.a1.contains(bp.a2) && (t.a1.overlap(bp.a1).size() == 0) && (t.a2.overlap(bp.a1).size() > 0)) {
    return true;
  }
  if (t.a2.contains(bp.a2) && (t.a2.overlap(bp.a1).size() == 0) && (t.a1.overlap(bp.a1).size() > 0)) {
    return true;
  }
  return false;
}

double FastRFTripartitionScorer::score(const Tripartition& t) {
  double weight = 0;

  BitVectorFixed& t1_a1 = possibleA1.at(t.a1);
  BitVectorFixed& t1_a2 = possibleA2.at(t.a1);
  BitVectorFixed& t1_b1 = possibleB1.at(t.a1);
  BitVectorFixed& t1_b2 = possibleB2.at(t.a1);

  BitVectorFixed& t2_a1 = possibleA1.at(t.a2);
  BitVectorFixed& t2_a2 = possibleA2.at(t.a2);
  BitVectorFixed& t2_b1 = possibleB1.at(t.a2);
  BitVectorFixed& t2_b2 = possibleB2.at(t.a2);

  BitVectorFixed combinations =
    (t1_a1 & t2_b2) | (t2_a1 & t1_b2) | (t1_a2 & t2_b1) | (t2_a2 & t1_b1);
  
  for (int i : combinations) {
    weight += weights[i];
  }
  
  
  return weight;
}

double FastRFTripartitionScorer::adjust_final_score(double score) {
  LOG(INFO) << "Raw Score: " << score << endl;
  int total_fp = total_weight - score;
  int total_fn = total_leaves - (n_trees * 3) - score;
  int total_rf = total_fp + total_fn;
  return total_rf;
}
