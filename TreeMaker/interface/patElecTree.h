#ifndef __ELEC_TREE_H_
#define __ELEC_TREE_H_
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h"
#include "TClonesArray.h"
#include <bitset>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DelPanj/TreeMaker/interface/baseTree.h"
#include "DelPanj/TreeMaker/interface/utils.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"



#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/RefHolder.h"
#include "DataFormats/Common/interface/RefVectorHolder.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "TTree.h"
#include "Math/VectorUtil.h"

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
  edm::InputTag patElecLabel_;
  edm::InputTag eleVetoDecisionsMapToken_;
  
  edm::InputTag eleVetoIdMapToken_;
  edm::InputTag eleLooseIdMapToken_;
  edm::InputTag eleMediumIdMapToken_;
  edm::InputTag eleTightIdMapToken_;
  edm::InputTag eleHEEPIdMapToken_;
  
  std::vector<Bool_t> isPassVeto;
  std::vector<Bool_t> isPassLoose;
  std::vector<Bool_t> isPassMedium;
  std::vector<Bool_t> isPassTight;
  std::vector<Bool_t> isPassHEEP;
  
  
  Float_t patElecRho_;
  Int_t nEle_;
  TClonesArray *patElecP4;
  std::vector<Float_t> patElecEffArea_;
  std::vector<Float_t> patElecCharge_;
  std::vector<Float_t> patElecChargeConsistent_;
  //std::vector<Float_t> patElecEt_;
  //std::vector<Float_t> patElecEnergy_;
  //std::vector<Float_t> patElecPt_;
  //std::vector<Float_t> patElecEta_;
  //std::vector<Float_t> patElecPhi_;
  //std::vector<Float_t> patElecM_;
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
