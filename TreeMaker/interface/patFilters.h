#ifndef patFilters_h
#define patFilters_h

#include<iostream>
#include<string>
#include<vector>

#include "TTree.h"
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DelPanj/TreeMaker/interface/baseTree.h"

class patFilters : public baseTree{

 public:
  patFilters(std::string name,TTree* tree, const edm::ParameterSet& iConfig);
  void Fill(const edm::Event& iEvent);
  void Clear();
  void SetBranches();
  
 private:
  patFilters(){};
  int nfilters_;
  std::vector<bool> filterResult_;
  bool hbhet_;
  std::vector<std::string> filterName_;
  edm::InputTag filterTag;
};

#endif
