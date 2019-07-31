#ifndef _GEN_INFO_TREE_H_
#define _GEN_INFO_TREE_H_

/*
  Updated by: Shin-Shan Yu
  Date      : 20 March 2016
  Replace getByLabel with getByToken
*/

#include <memory>
#include <vector>
#include <string>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

// For generator-level jet substructure
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/contrib/SoftDrop.hh"
#include "fastjet/contrib/Nsubjettiness.hh"
#include "fastjet/contrib/Njettiness.hh"
#include "fastjet/contrib/NjettinessPlugin.hh"
#include "fastjet/contrib/MeasureDefinition.hh"
#include "fastjet/contrib/EnergyCorrelator.hh"

#include "DelPanj/TreeMaker/interface/baseTree.h"
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"

//
// class declaration
//

class genInfoTree : public baseTree{

 public:
  genInfoTree(std::string name, TTree* tree, const edm::ParameterSet& cfg);
  ~genInfoTree();
  void GetRunInfo(const edm::Run& iRun);
  void Fill(const edm::Event& iEvent);
  void Clear();

  edm::EDGetTokenT<reco::GenParticleCollection>               genParticleToken;
  edm::EDGetTokenT<GenEventInfoProduct>                       genEventToken;
  edm::EDGetTokenT<LHERunInfoProduct>                         lheRunToken;
  edm::EDGetTokenT<LHEEventProduct>                           lheEventToken;

  edm::EDGetTokenT<pat::PackedGenParticleCollection>          genMETToken_true;
  edm::EDGetTokenT<reco::GenMETCollection>                    genMETToken_calo;
  edm::EDGetTokenT<reco::GenMETCollection>                    genMETToken_caloNonPrompt;
  edm::EDGetTokenT<reco::GenJetCollection>                    ak4genJetsToken;
  edm::EDGetTokenT<reco::GenJetCollection>                    ak8genJetsToken;

  unsigned int MAXNGENPAR_;
  bool applyStatusSelection_;  // keep only particles with status code <=30
  bool applyPromptSelection_;  // keep only prompt particles or particles with status<=30
  bool saveLHEWeights_;        // save all LHE weights
  bool saveGenJets_;           // save genJets information
  bool saveGenJetSub_;         // save substructure variables
 private:

  genInfoTree(){};
  void SetBranches();


  /* For substructure: starts here, added by Eiko */
  fastjet::AreaDefinition *areaDef;
  fastjet::GhostedAreaSpec *activeArea;
  fastjet::JetDefinition *jetDefAKT=0;
  fastjet::contrib::SoftDrop *softdrop=0;
  fastjet::contrib::Nsubjettiness *fjtau1=0;
  fastjet::contrib::Nsubjettiness *fjtau2=0;
  fastjet::contrib::Nsubjettiness *fjtau3=0;
  /* For substructure: ends here    */

  float ptHat_;      // added by Eiko
  float mcWeight_;   // added by Eiko
  float HT_;       // added by Eiko
  float genMET_true_; // added by Eiko
  float genMET_calo_;  // added by Eiko
  float genMET_caloNonPrompt_; // added by Eiko

  std::vector<float>       pdf_;
  float                    originalLHEweight_;
  std::vector<float>       pdfscaleSysWeights_;

  int nGenPar_;
  TClonesArray       *genParP4_;
  std::vector<float> genPx_;
  std::vector<float> genPy_;
  std::vector<float> genPz_;
  std::vector<float> genE_;
  std::vector<int>   genParQ_;
  std::vector<int>   genParId_;
  std::vector<int>   genParSt_;
  std::vector<int>   genMomParId_; // added by Eiko
  std::vector<int>   genParIndex_;    // added by Eiko
  std::vector<int>   genNMo_;
  std::vector<int>   genNDa_;
  std::vector<int>   genMo1_;
  std::vector<int>   genMo2_;
  std::vector<int>   genDa1_;
  std::vector<int>   genDa2_;
  std::vector<int>   genStFlag_;


  // save this informatio if saveGenJets is true

  int ak4nGenJet_;
  TClonesArray       *ak4GenJetP4_;

  int ak8nGenJet_;
  TClonesArray       *ak8GenJetP4_;
  /// genjet substructure, added by Eiko
  std::vector<float> ak8GenJetMSD_;        //softdropped jet mass
  std::vector<float> ak8GenJetSDSJdR_;     //softdrop subjet deltaR
  std::vector<float> ak8GenJetSDSJSymm_;   //softdrop subjet pt symmetry
  std::vector<float> ak8GenJetSDMassDrop_; //softdrop mass drop
  std::vector<float> ak8GenJettau1_;
  std::vector<float> ak8GenJettau2_;
  std::vector<float> ak8GenJettau3_;



};

#endif
