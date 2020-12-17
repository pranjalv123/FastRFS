#include "phylonaut/wASTRAL.hpp"
#include "phylonaut/CladeExtractor/DefaultTaxonSetExtractor.hpp"
#include "phylonaut/CladeExtractor/ASTRALCladeExtractor.hpp"
#include "phylonaut/CladeExtractor/SimpleCladeExtractor.hpp"
#include "FastRFTripartitionScorer.hpp"
#include "phylonaut/Analysis/SingleTreeAnalysis.hpp"
#include "phylonaut/Analysis/ConsensusTreeAnalysis.hpp"
#include "phylonaut/Analysis/CountTreesAnalysis.hpp"
#include "phylonaut/Analysis/ScoreAnalysis.hpp"
#include <glog/logging.h>
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <libgen.h>
#include "RFSupportAnalysis.hpp"

using namespace std;

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);

  vector<string> wastral_args;

  string input;
  string output = "";
  string clades_file = "";
  string extra = "";
  string scoretree = "";
  

  bool getScore=true;
  bool getSingle=true;
  bool getGreedy=true;
  bool getMajority=true;
  bool getStrict=true;
  bool getCount=true;
  bool useAstral=true;

  bool score_only = false;
  
  vector<string> output_labels;

  Config conf;

  for(int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-i" || string(argv[i]) == "--input") {
      assert(argc > i+1);
      i++;
      input = string(argv[i]);
    }
    if (string(argv[i]) == "-o"  || string(argv[i]) == "--output") {
      assert(argc > i+1);
      i++;
      fclose(fopen(argv[i], "a"));
      output = string(realpath(argv[i], NULL));
    }
    if (string(argv[i]) == "-e" || string(argv[i]) == "--extra") {
      assert(argc > i+1);
      i++;
      extra = string(realpath(argv[i], NULL));
    }
    if (string(argv[i]) == "--clades") {
      assert(argc > i+1);
      i++;
      clades_file = string(realpath(argv[i], NULL));
    }
    if (string(argv[i]) == "--scoreonly") {
      assert(argc > i+1);
      i++;
      scoretree = string(argv[i]);
      score_only = true;
    }
    if (string(argv[i]) == "--noscore") {
      getScore=false;
    }

    if (string(argv[i]) == "--nosingle") {
      getSingle=false;
    }
    if (string(argv[i]) == "--nogreedy") {
      getGreedy=false;
    }
    if (string(argv[i]) == "--nomajority") {
      getMajority=false;
    }
    if (string(argv[i]) == "--nostrict") {
      getStrict=false;
    }
    if (string(argv[i]) == "--nocount") {
      getCount=false;
    }
    if (string(argv[i]) == "--noastral") {
      useAstral=false;
    }
	  
    if (string(argv[i]) == "--score") {
      getScore=true;
    }
    if (string(argv[i]) == "--single") {
      getSingle=true;
    }
    if (string(argv[i]) == "--greedy") {
      getGreedy=true;
    }
    if (string(argv[i]) == "--majority") {
      getMajority=true;
    }
    if (string(argv[i]) == "--strict") {
      getStrict=true;
    }
    if (string(argv[i]) == "--count") {
      getCount=true;
    }

    if (string(argv[i]) == "--profile") {
      conf.profile="profile";
    }
  }

  if (input.size() == 0) {
    cerr << "FastRFS -i <source tree file> [-o <output file>] [-e <extra trees>]" << endl;
    exit(-1);
  }


  if (score_only) {
    conf.analyses.push_back(new ScoreAnalysis());
    output_labels.push_back("score");
    conf.extractors.push_back(new SimpleCladeExtractor(scoretree));
    conf.scorer = new FastRFTripartitionScorer(input);
    conf.taxon_extractor = new DefaultTaxonSetExtractor(scoretree);    
  } else {
  
    if (getScore) {
      conf.analyses.push_back(new ScoreAnalysis());
      output_labels.push_back("score");
    }
    if (getSingle) {
      vector<string> input_trees;
      string tree;
      ifstream file(input);

      while(getline(file, tree)) {
	input_trees.push_back(tree);
      }
      file.close();

      conf.analyses.push_back(new RFSupportAnalysis(input_trees));
      output_labels.push_back("single");
    }
    if (getGreedy) {
      conf.analyses.push_back(new ConsensusTreeAnalysis(0.0));
      output_labels.push_back("greedy");
    }
    if (getMajority) {
      conf.analyses.push_back(new ConsensusTreeAnalysis(0.5));
      output_labels.push_back("majority");
    }
    if (getStrict) {
      conf.analyses.push_back(new ConsensusTreeAnalysis(1.0));
      output_labels.push_back("strict");
    }
    if (getCount) {
      conf.analyses.push_back(new CountTreesAnalysis());
      output_labels.push_back("count");
    }

    conf.scorer = new FastRFTripartitionScorer(input);

    conf.taxon_extractor = new DefaultTaxonSetExtractor(input);

    if (useAstral) {
      if (extra != "")
        conf.extractors.push_back(new ASTRALCladeExtractor(input, extra));
      else
        conf.extractors.push_back(new ASTRALCladeExtractor(input));
    }
    if (clades_file.size()) {
      conf.extractors.push_back(new SimpleCladeExtractor(clades_file));
    }
    
  }
  vector<string> trees = wASTRAL(conf);

  for (size_t i = 0; i < trees.size(); i++) {
    cout << output_labels.at(i) << endl;
    cout << trees.at(i) << endl;
    if (output.size() ) {
      ofstream outfile(output + "." + output_labels.at(i));
      outfile << trees.at(i) << endl;
      outfile.close();
    }
  }
}
