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
#include "whereami.h"
#include "RFSupportAnalysis.hpp"

using namespace std;

int main(int argc, char** argv) {
  vector<string> wastral_args;

  string input;
  string output = "";
  string extra = "";
  int debug = 0;

  bool getScore=true;
  bool getSingle=true;
  bool getGreedy=true;
  bool getMajority=true;
  bool getStrict=true;
  bool getAll=false;
  bool getCount=false;

  vector<string> output_labels;
  int path_length = wai_getExecutablePath(NULL, 0, NULL);
  char* path = new char[path_length + 1];
  int dirname_length;
  wai_getExecutablePath(path, path_length, &dirname_length);
  path[path_length] = '\0';

  Logger::disable("DEBUG");
  Logger::enable("INFO");
  Logger::enable("PROGRESS");

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
    if (string(argv[i]) == "--quiet") {
      Logger::disable("DEBUG");
      Logger::disable("INFO");
      Logger::disable("PROGRESS");
    }
    if (string(argv[i]) == "--noscore") {
      getScore=false;
    }

    if (string(argv[i]) == "--nosingle") {
      getSingle=false;
    }
    if (string(argv[i]) == "--nogreedy") {
      getSingle=false;
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

    if (string(argv[i]) == "--score") {
      getScore=true;
    }
    if (string(argv[i]) == "--single") {
      getSingle=true;
    }
    if (string(argv[i]) == "--greedy") {
      getSingle=true;
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

  if (extra != "")
    conf.extractors.push_back(new ASTRALCladeExtractor(input, extra));
  else
    conf.extractors.push_back(new ASTRALCladeExtractor(input));

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
