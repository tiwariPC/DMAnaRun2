#ifndef __JET_TREE_H_
#define __JET_TREE_H_





#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TTree.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"











#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DelPanj/TreeMaker/interface/utils.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DelPanj/TreeMaker/interface/baseTree.h"
#include "DelPanj/TreeMaker/interface/jetSelector.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include <boost/shared_ptr.hpp>
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Common/interface/ValueMap.h"
//#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Utilities/interface/InputTag.h"

using namespace std;
using namespace edm;


class jetTree  : public baseTree{

 public:
  jetTree(std::string name, TTree* tree, const edm::ParameterSet& cfg );//name=patJetAk05
  ~jetTree();
  void Fill(const edm::Event& iEvent, edm::EventSetup const& iSetup) ; 
  void SetBranches();
  void Clear();
  
  // typedef IPTI IPTagInfo;
  // typedef typename IPTI::input_container Tracks;
  //  typedef typename IPTI::input_container::value_type TrackRef;
  //  typedef VTX Vertex;
  //  typedef reco::TemplatedSecondaryVertexTagInfo<IPTI,VTX> SVTagInfo;

 

 
 
 private:
  jetTree();

  bool isFATJet_;
  bool isADDJet_;
  
  edm::InputTag JetLabel_;
  edm::InputTag AddjetlabelC_;
  
  edm::InputTag rhoSrc_;
  edm::InputTag pvSrc_;

  std::string  svTagInfosCstr_;
  bool isSpring15_;



  std::vector<std::string> jecPayloadNames_;
  std::string              jecUncName_;

  jetSelector jet2012ID_;

  boost::shared_ptr<JetCorrectionUncertainty> jecUnc_;
  boost::shared_ptr<FactorizedJetCorrector> jec_;
  
  bool runAddJet_;

  //Branches common to all the jets.
  int nJet_;

  TClonesArray*      genjetP4_;
  std::vector<float> genjetEM_;
  std::vector<float> genjetHAD_;
  std::vector<float> genjetINV_;
  std::vector<float> genjetAUX_;
  std::vector<float> matchedDR_;

  std::vector<float> jetRawFactor_;

  TClonesArray *jetP4_;

  std::vector<float> jetCorrUncUp_;
  std::vector<float> jetCorrUncDown_;
  std::vector<int>   jetCharge_;
  std::vector<int>   jetPartonFlavor_;
  std::vector<int>   jetHadronFlavor_;
  std::vector<bool>  jetPassIDLoose_;
  std::vector<bool>  jetPassIDTight_;
  std::vector<float> PUJetID_;
  std::vector<bool>  isPUJetID_;

  //Energy Fraction and Multiplicity 

  std::vector<float> jetCEmEF_;
  std::vector<float> jetCHadEF_;
  std::vector<float> jetPhoEF_;
  std::vector<float> jetNEmEF_;
  std::vector<float> jetNHadEF_;

  std::vector<float> jetMuEF_;
  std::vector<float> jetChMuEF_;

  std::vector<float> jetHFHadEF_;
  std::vector<float> jetHFEMEF_;
  std::vector<float> jetHOEnergy_;
  std::vector<float> jetHOEF_;


  std::vector<int>   jetCMulti_;
  std::vector<int>   jetEleMultiplicity_;
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
  std::vector<float> jetTau4_;


  //ak8jet mass
 
  //
    
  std::vector<float>  jetSDmass_; 
  std::vector<float>  jetTRmass_;
  std::vector<float>  jetPRmass_;
  std::vector<float>  jetFimass_;
  
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
