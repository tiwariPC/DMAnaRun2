#ifndef patHlttree_h
#define patHlttree_h

#include<iostream>
#include<string>
#include<vector>

#include "TTree.h"
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DelPanj/TreeMaker/interface/baseTree.h"

class patHltTree : public baseTree{

 public:
  patHltTree(std::string name,TTree* tree);
  void Fill(const edm::Event& iEvent);
  void Clear();
  void SetBranches();

 private:
  patHltTree(){};
  int nTrigs_;
  std::vector<bool> trigResult_;
  std::vector<std::string> trigName_;
  std::vector<int> trigPrescale_;
};

#endif
