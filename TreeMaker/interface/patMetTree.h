#ifndef __MET_TREE_H_
#define __MET_TREE_H_
/*
  Updated By: Raman Khurana 
  Date      : 24 June 2015.

 -- it can save three types of MET now : 
   = PFMET uncorrected
   = PFMET corrected
   = MVA MET
*/

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DelPanj/TreeMaker/interface/utils.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DelPanj/TreeMaker/interface/baseTree.h"
using namespace std;
using namespace edm;



class patMetTree : public baseTree{

 public:
  patMetTree(std::string name, TTree* tree, const edm::ParameterSet& cfg);
  ~patMetTree();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();
  
 private:
  patMetTree();
  
  edm::InputTag pfMetRawLabel_;
  edm::InputTag pfMetLabel_;
  edm::InputTag pfMVAMETLabel_;
  
  float patMetCorrPt_;  
  float patMetCorrPhi_; 
  float patMetCorrSumEt_;
  float patMetCorrSig_;
  float patMetRawPt_;
  float patMetRawPhi_;
  float patMetRawSumEt_;
  float patMetRawCov00_;
  float patMetRawCov01_;
  float patMetRawCov10_;
  float patMetRawCov11_;


  float mvaMetPt_;
  float mvaMetPhi_;
  float mvaMetSumEt_;
  float mvaMetSig_;
  

};

#endif

