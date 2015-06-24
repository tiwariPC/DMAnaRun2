
#ifndef __ELEC_TREE_H_
#define __ELEC_TREE_H_
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DelPanj/TreeMaker/interface/baseTree.h"
#include "DelPanj/TreeMaker/interface/utils.h"
using namespace std;
using namespace edm;
class patElecTree : public baseTree {
 public:
  patElecTree(std::string name, TTree* tree, const edm::ParameterSet& cfg);
  ~patElecTree();
  void Fill(const edm::Event& iEvent);
  void SetBranches();
  void Clear();
 private:

  //TTree* tree_;
  //Dont Allow User to Call the Default Constructor.

  patElecTree();
  edm::InputTag pvSrc_;
  edm::InputTag patElecLabel_;
 
  std::vector<Int_t> patElecType;

  Float_t patElecRho_;
  Int_t nEle_;

  std::vector<Float_t> patElecCharge_;
  std::vector<Float_t> patElecChargeConsistent_;
  std::vector<Float_t> patElecEt_;
  std::vector<Float_t> patElecEnergy_;
  std::vector<Float_t> patElecPt_;
  std::vector<Float_t> patElecEta_;
  std::vector<Float_t> patElecPhi_;
  std::vector<Float_t> patElecM_;
  std::vector<Float_t> patElecR9_;
  std::vector<Float_t> patElecHoverE_;
  std::vector<Float_t> patElecD0_;
  std::vector<Float_t> patElecDz_;
  std::vector<Float_t> patElecScEn_;
  std::vector<Float_t> patElecScPreEn_;
  std::vector<Float_t> patElecScEta_;
  std::vector<Float_t> patElecScPhi_;
  std::vector<Float_t> patElecScRawEn_;
  std::vector<Float_t> patElecScEtaWidth_;
  std::vector<Float_t> patElecScPhiWidth_;
  std::vector<Float_t> patElecEoverP_;
  std::vector<Float_t> patElecBrem_;
  std::vector<Float_t> patElecdEtaAtVtx_;
  std::vector<Float_t> patElecdPhiAtVtx_;
  std::vector<Float_t> patElecSigmaIEtaIEta_;
  std::vector<Float_t> patElecSigmaIEtaIPhi_;
  std::vector<Float_t> patElecSigmaIPhiIPhi_;
  std::vector<Float_t> patElecConvVeto_;
  std::vector<Float_t> patElecMissHits_;
  std::vector<Float_t> patElecEoverPInv_;
  std::vector<Float_t> patElecdEtaseedAtVtx_;
  std::vector<Float_t> patElecE1x5_;
  std::vector<Float_t> patElecE2x5_;
  std::vector<Float_t> patElecE5x5_;
  std::vector<Float_t> patElecNeHadIso_;
  std::vector<Float_t> patElecGamIso_;
  std::vector<Float_t> patElecPUPt_;
  std::vector<Float_t> patElecCorrPfIso_;
  std::vector<Float_t> patElecaloEnergy_;
  std::vector<Float_t> patElecChHadIso_;
  std::vector<Float_t> patElecSigmaIEtaIEtaFull5x5_;
  std::vector<Float_t> patElecE1x5Full5x5_;
  std::vector<Float_t> patElecE2x5Full5x5_;
  std::vector<Float_t> patElecE5x5Full5x5_;
  std::vector<Float_t> patElecR9Full5x5_;
  std::vector<Float_t> patElecEcalDrivenSeed_;
  std::vector<Float_t> patElecDr03EcalRecHitSumEt_;
  std::vector<Float_t> patElecDr03HcalDepth1TowerSumEt_;
  std::vector<Float_t> patElecDr03HcalDepth2TowerSumEt_;
  std::vector<Float_t> patElecDr03HcalTowerSumEt_;
  std::vector<Float_t> patElecDr03TkSumPt_;
  std::vector<Float_t> patElecTrkdztrackref_;
  std::vector<Float_t> patElecTrkdxytrackref_;
  std::vector<Float_t> patElecInBarrel_;
  std::vector<Float_t> patElecInEndcap_;

};
#endif
