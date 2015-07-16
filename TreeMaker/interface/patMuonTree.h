#ifndef __MUON_TREE_H_
#define __MUON_TREE_H_

#include <memory>
#include <string>
#include <iostream>
#include <vector>

#include "TTree.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "DelPanj/TreeMaker/interface/baseTree.h"
#include "DelPanj/TreeMaker/interface/utils.h"

using namespace std;
using namespace edm;

class patMuonTree : public baseTree {

 public:

  patMuonTree(std::string name, TTree* tree, const edm::ParameterSet& cfg);
  ~patMuonTree();

  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();

 private:
  //TTree* tree_;
  //Dont Allow User to Call the Default Constructor.
  patMuonTree();
  edm::InputTag pvSrc_;
  edm::InputTag patMuonLabel_;
  // for mini-isolation, same input as that in patElecTree
  edm::InputTag pfCandLabel_;
  double r_iso_min_;
  double r_iso_max_;
  double kt_scale_;
  bool charged_only_;

  // ntuple variabes
  int nMu_;
  TClonesArray *patMuonP4_;

  std::vector<int> patMuonType_;
  std::vector<int> patMuonCharge_;


  std::vector<bool> isGlobalMuon_;
  std::vector<bool> isTrackerMuon_;
  std::vector<bool> isPFMuon_;

  std::vector<int>   patMuonITrkIndex_;
  std::vector<int>   patMuonSegIndex_;
  std::vector<int>   patMuonNSeg_;
  std::vector<bool>   patMuonGood_;
  std::vector<bool>   patMuonIsGood_;

  std::vector<float> patMuonTrkPt_;
  std::vector<float> patMuonTrkPtErr_;
  std::vector<float> patMuondxy_;
  std::vector<float> patMuondz_;
  std::vector<float> patMuonsegmentCompatibility_;
  std::vector<float> patMuonchi2LocalPosition_;
  std::vector<float> patMuontrkKink_;

  std::vector<float> patMuonInnerdxy_;
  std::vector<float> patMuonInnerdz_;
  std::vector<int>   patMuonTrkLayers_;
  std::vector<int>   patMuonPixelLayers_;
  std::vector<int>   patMuonPixelHits_;
  std::vector<int>   patMuonHits_;
  std::vector<float> patMuonTrkQuality_;
  std::vector<float> patMuonChi2NDF_;
  std::vector<float> patMuonInnervalidFraction_;
  std::vector<int>   patMuonMatches_;

  std::vector<float> patMuonTrkIso_;
  std::vector<float> patMuonHcalIso_;
  std::vector<float> patMuonEcalIso_;
  std::vector<float> patMuonChHadIso_;
  std::vector<float> patMuonNeHadIso_;
  std::vector<float> patMuonGamIso_;
  std::vector<float> patMuonPUPt_;
  std::vector<float> patMuonMiniIso_;

 
  
};
#endif
