#ifndef __JET_TREE_H_
#define __JET_TREE_H_

/*
  Updated by: Shin-Shan Yu
  Date      : 20 March 2016
  Replace getByLabel with getByToken
*/


#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include <boost/shared_ptr.hpp>


#include "DelPanj/TreeMaker/interface/utils.h"
#include "DelPanj/TreeMaker/interface/baseTree.h"
#include "DelPanj/TreeMaker/interface/jetSelector.h"

using namespace std;
using namespace edm;


class jetTree  : public baseTree{

 public:
  jetTree(std::string name, TTree* tree, const edm::ParameterSet& cfg );//name=patJetAk05
  ~jetTree();
  

  void Fill(const edm::Event& iEvent, edm::EventSetup const& iSetup) ; 
  void Clear();

  edm::EDGetTokenT<reco::VertexCollection>          vertexToken;
  edm::EDGetTokenT<double>                          rhoForJetToken;
  edm::EDGetTokenT<pat::JetCollection>              jetToken;
  edm::EDGetTokenT<pat::JetCollection>              prunedMToken;
  edm::EDGetTokenT<pat::JetCollection>              softdropMToken;
  
    
 private:

  jetTree(){};
  void SetBranches();

  bool isTHINJet_;
  bool isFATJet_;
  bool isADDJet_;
  bool isAK4PuppiJet_;
  bool isAK8PuppiJet_;
  bool useJECText_;
  
  std::string svTagInfosCstr_;

  std::string jecUncPayLoadName_; // for global tag
  std::vector<std::string> prunedMassJecNames_; // for reading text file
  std::vector<std::string> softdropMassJecNames_; // for reading text file
  std::vector<std::string> jecNames_; // for reading text file
  std::string              jecUncName_; // for reading text file

  jetSelector jet2012ID_;


  boost::shared_ptr<FactorizedJetCorrector> prunedjecText_;
  boost::shared_ptr<FactorizedJetCorrector> softdropjecText_;
  boost::shared_ptr<FactorizedJetCorrector> jecText_;
  boost::shared_ptr<JetCorrectionUncertainty> jecUncText_;


  //Branches common to all the jets.
  int nJet_;
  float jetRho_;
  int jetNPV_;

  TClonesArray*      genjetP4_;
  std::vector<float> genjetEM_;
  std::vector<float> genjetHAD_;
  std::vector<float> genjetINV_;
  std::vector<float> genjetAUX_;
  std::vector<float> matchedDR_;

  std::vector<float> jetRawFactor_;

  TClonesArray *jetP4_;
  TClonesArray *unCorrJetP4_;

  std::vector<float> jetArea_;
  std::vector<float> jetCorrUncUp_;
  std::vector<float> jetCorrUncDown_;
  std::vector<int>   jetCharge_;
  std::vector<int>   jetPartonFlavor_;
  std::vector<int>   jetHadronFlavor_;
  std::vector<bool>  jetPassIDLoose_;
  std::vector<bool>  jetPassIDTight_;
  std::vector<float> PUJetID_;
  std::vector<bool>  isPUJetIDLoose_;
  std::vector<bool>  isPUJetIDMedium_;
  std::vector<bool>  isPUJetIDTight_;

  //Energy Fraction and Multiplicity 

  std::vector<float> jetCEmEF_;
  std::vector<float> jetCHadEF_;
  std::vector<float> jetPhoEF_;
  std::vector<float> jetNEmEF_;
  std::vector<float> jetNHadEF_;

  std::vector<float> jetEleEF_;
  std::vector<float> jetMuoEF_;
  std::vector<float> jetChMuEF_;

  std::vector<float> jetHFHadEF_;
  std::vector<float> jetHFEMEF_;
  std::vector<float> jetHOEnergy_;
  std::vector<float> jetHOEF_;


  std::vector<int>   jetCMulti_;
  std::vector<int>   jetEleMultiplicity_;
  std::vector<int>   jetMuoMultiplicity_;
  std::vector<int>   jetCHHadMultiplicity_;
  std::vector<int>   jetPhMultiplicity_;
  std::vector<int>   jetNMultiplicity_;
  std::vector<int>   jetNHadMulplicity_;
  std::vector<int>   jetHFHadMultiplicity_;
  std::vector<int>   jetHFEMMultiplicity_;


  // btag information
  std::vector<float> jetSSV_;
  std::vector<float> jetCSV_;
  std::vector<float> jetSSVHE_;
  std::vector<float> jetCISVV2_;
  std::vector<float> jetTCHP_;
  std::vector<float> jetTCHE_;
  std::vector<float> jetJP_;
  std::vector<float> jetJBP_;


  std::vector<float> jetTau1_;
  std::vector<float> jetTau2_;
  std::vector<float> jetTau3_;
  std::vector<float> jetTau21_;


  //ak8jet mass
 
  //
    
  std::vector<float>  jetSDmass_; 
  std::vector<float>  jetPRmass_; // from miniAOD
  std::vector<float>  jetPRmassL2L3Corr_; 
  

  //puppi related stuff
  std::vector<float> jetPuppiTau1_;
  std::vector<float> jetPuppiTau2_;
  std::vector<float> jetPuppiTau3_;
  std::vector<float> jetPuppiSDmass_;
  std::vector<float> jetPuppiSDmassL2L3Corr_;

  TClonesArray *jetPuppiP4_;
  TClonesArray *jetPuppiSDRawP4_;
  std::vector<int>   nSubSDPuppiJet_;
  std::vector<std::vector<int> >   subjetSDPuppiFatJetIndex_; 
  std::vector<std::vector<float> > subjetSDPuppiPx_;
  std::vector<std::vector<float> > subjetSDPuppiPy_;
  std::vector<std::vector<float> > subjetSDPuppiPz_;
  std::vector<std::vector<float> > subjetSDPuppiE_;
  std::vector<std::vector<float> > subjetSDPuppiCSV_;
  
  
  //jet  Hbb tagger for fat and add jet

  std::vector<float> jet_DoubleSV_;

  //jet secondary vtx

  std::vector<int>   jet_nSV_;
  std::vector<std::vector<float> > jet_SVMass_;
  std::vector<std::vector<float> > jet_SVEnergyRatio_;



  // subjet of jets

  std::vector<int>   nSubSDJet_;
  std::vector<std::vector<int> >   subjetSDFatJetIndex_; 
  std::vector<std::vector<float> > subjetSDPx_;
  std::vector<std::vector<float> > subjetSDPy_;
  std::vector<std::vector<float> > subjetSDPz_;
  std::vector<std::vector<float> > subjetSDE_;
  std::vector<std::vector<int> >   subjetSDCharge_; 
  std::vector<std::vector<int> >   subjetSDPartonFlavor_;
  std::vector<std::vector<int> >   subjetSDHadronFlavor_;
  std::vector<std::vector<float> > subjetSDCSV_;        



};

#endif
