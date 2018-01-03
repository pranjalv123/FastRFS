#ifndef FASTRF_TRIPARTITION_SCORER_HPP__
#define FASTRF_TRIPARTITION_SCORER_HPP__

#include <Clade.hpp>
#include <BitVector.hpp>
#include <phylonaut/TripartitionScorer.hpp>

class FastRFTripartitionScorer : public TripartitionScorer {
public:
  FastRFTripartitionScorer(string genetreesfile) : treesfile(genetreesfile) {};
  void setup(Config& conf, vector<Clade>& clades);
  int addSourceTree(string tree);
  virtual double score (const Tripartition& t);
  bool matches(const Tripartition& t, const Bipartition& bp);
  int total_weight;
  virtual double adjust_final_score(double score);
  virtual bool better(double newscore, double oldscore);
private:
  string treesfile;
  unordered_map<Bipartition, double > clade_weights;

  vector<const Bipartition*> bipartitions;
  vector<double> weights;

  
  unordered_map<Clade, BitVectorFixed> possibleA1; //contains all of a1, none of a2
  unordered_map<Clade, BitVectorFixed> possibleB1; //contains at least one of a1, none of a2
  unordered_map<Clade, BitVectorFixed> possibleA2; //contains all of a2, none of a1
  unordered_map<Clade, BitVectorFixed> possibleB2; //contains at least one of a2, none of a1
};



#endif // RF_TRIPARTITION_SCORER_HPP__
