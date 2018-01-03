#include <phylonaut/wASTRAL.hpp>
#include <phylonaut/DefaultTaxonSetExtractor.hpp>
#include <phylonaut/ASTRALCladeExtractor.hpp>
#include "FastRFTripartitionScorer.hpp"
#include <phylonaut/SingleTreeAnalysis.hpp>
#include <phylonaut/ConsensusTreeAnalysis.hpp>
#include <phylonaut/CountTreesAnalysis.hpp>
#include <phylonaut/ScoreAnalysis.hpp>
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <libgen.h>

using namespace std;

int main(int argc, char** argv) {
  vector<string> wastral_args;

  string input;
  string output = "";
  string extra = "";
  int debug = 0;

  vector<string> output_labels;
  
  string astralpath = string(dirname(argv[0])) + "/astral.4.7.8.jar";

  Logger::disable("DEBUG");

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
    if (string(argv[i]) == "--debug") {
      Logger::enable("DEBUG");
      Logger::enable("INFO");
      Logger::enable("PROGRESS");
    }
    if (string(argv[i]) == "--score") {
      conf.analyses.push_back(new ScoreAnalysis());
      output_labels.push_back("score");	
    }

    if (string(argv[i]) == "--single") {
      conf.analyses.push_back(new SingleTreeAnalysis());
      output_labels.push_back("single");	
    }
    if (string(argv[i]) == "--greedy") {
      conf.analyses.push_back(new ConsensusTreeAnalysis(0.0));
      output_labels.push_back("greedy");	
    }
    if (string(argv[i]) == "--majority") {
      conf.analyses.push_back(new ConsensusTreeAnalysis(0.5));
      output_labels.push_back("majority");	
    }
    if (string(argv[i]) == "--strict") {
      conf.analyses.push_back(new ConsensusTreeAnalysis(1.0));
      output_labels.push_back("strict");	
    }
    if (string(argv[i]) == "--all") {
      // conf.analyses.push_back();
      // output_labels.push_back("all");	
    }
    if (string(argv[i]) == "--count") {
      conf.analyses.push_back(new CountTreesAnalysis());
      output_labels.push_back("count");	
    }
    if (string(argv[i]) == "--profile") {
      conf.profile="profile";
    }
  }

  if (input.size() == 0) {
    cerr << "FastRFS -i <source tree file> [-o <output file>] [-e <extra trees>]" << endl;
    exit(-1);
  }



  conf.scorer = new FastRFTripartitionScorer(input);

  conf.taxon_extractor = new DefaultTaxonSetExtractor(input);

  if (extra != "")
    conf.extractors.push_back(new ASTRALCladeExtractor(astralpath, input, extra));
  else
    conf.extractors.push_back(new ASTRALCladeExtractor(astralpath, input));
  
  vector<string> trees = wASTRAL(conf);

  for (int i = 0; i < trees.size(); i++) {
    cout << output_labels.at(i) << endl;    
    cout << trees.at(i) << endl;
    if (output.size() ) {
      ofstream outfile(output + "." + output_labels.at(i));
      outfile << trees.at(i) << endl;
      outfile.close();
    }
  }
}
