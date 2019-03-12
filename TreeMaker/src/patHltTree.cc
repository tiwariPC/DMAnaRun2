// Updated By : Raman Khurana, Shin-Shan Eiko Yu
// Dated      : Mon May 25 15:40:47 CDT 2015
// Added possible triggers for DM analysis, Jets and MET
#include <memory>
#include <cmath>
#include <vector>
#include <string.h>
#include <TString.h>
#include "DelPanj/TreeMaker/interface/patHltTree.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"



patHltTree::patHltTree(std::string name, TTree* tree, const edm::ParameterSet& iConfig):
  baseTree(name,tree),
  saveAllTrigPaths_(iConfig.getParameter<bool>("saveAllTrigPaths")),
  nTrigs_(0),
  nTrigObj_(0)
{
    SetBranches();
}

void
patHltTree::Fill(const edm::Event& iEvent)
{
  Clear();
  using namespace edm;

  edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
  iEvent.getByToken(triggerPrescalesToken,triggerPrescales);

  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
  iEvent.getByToken(triggerObjectsToken,triggerObjects);

  edm::Handle<edm::TriggerResults> trigResults;
  if (not iEvent.getByToken(trigResultsToken, trigResults)) {
    std::cout << ">>> TRIGGER collection does not exist !!!\n";
    return;
  }

  const edm::TriggerNames & trigNames = iEvent.triggerNames(*trigResults);

  for (unsigned int i=0; i<trigResults->size(); i++)
    {
      std::string trigName = trigNames.triggerName(i);


      bool trigResult = trigResults->accept(i); //bool not to use
      // if(!trigResult && !saveAllTrigPaths_)continue;
      int prescale = triggerPrescales->getPrescaleForIndex(i);
      if(prescale!=1 && !saveAllTrigPaths_)continue;

      if(false) std::cout<<" trigName = "<<trigName
       			<<" : " << trigResults->accept(i)
			<<" : " << triggerPrescales->getPrescaleForIndex(i)
       			<<std::endl;


      trigName_.push_back(trigName);
      trigResult_.push_back(trigResult);
      trigPrescale_.push_back(prescale);
      nTrigs_++;
    }
  //std::cout<<"nTrigs: "<< nTrigs_<<std::endl;
  for (pat::TriggerObjectStandAlone trigObject : *triggerObjects) {
      trigObject.unpackFilterLabels(iEvent, *trigResults);
      trigObject.unpackPathNames(trigNames);
      //std::cout << "\t   Collection: " << trigObject.collection() << std::endl;
      //std::cout << "\t   Type IDs:   ";
      //for (unsigned h = 0; h < trigObject.filterIds().size(); ++h) std::cout << " " << trigObject.filterIds()[h] ;
      //std::cout << std::endl;
      //std::cout << "\t   Filters:    ";
      for (unsigned h = 0; h < trigObject.filterLabels().size(); ++h) {
          //std::cout << " " << trigObject.filterLabels()[h];
          std::string fullpath1 = trigObject.filterLabels()[h];
          std::string fullpath2 = std::to_string(nTrigObj_);
          std::string fullpath = fullpath1 + "_Obj_" + fullpath2;
          trigFilterLabels_.push_back(fullpath);
      }
      std::vector<std::string> pathNamesAll = trigObject.pathNames(false);
      std::vector<std::string>  pathNamesLast = trigObject.pathNames(true);
      
      //std::cout << "\t   Paths (" << pathNamesAll.size()<<"/"<<pathNamesLast.size()<<"):    ";
      for (unsigned h = 0, n = pathNamesAll.size(); h < n; ++h) {
          std::string fullpathName1 = pathNamesAll[h];
          std::string fullpathName2 = std::to_string(nTrigObj_);
          std::string fullpathName = fullpathName1 + "_Obj_" + fullpathName2;
          trigPathName_.push_back(fullpathName);
          //bool isBoth = trigObject.hasPathName( pathNamesAll[h], true, true );
          //bool isL3   = trigObject.hasPathName( pathNamesAll[h], false, true );
          //bool isLF   = trigObject.hasPathName( pathNamesAll[h], true, false );
          //bool isNone = trigObject.hasPathName( pathNamesAll[h], false, false );
          //std::cout << "   " << pathNamesAll[h];
          //if (isBoth) std::cout << "(L,3)";
          //if (isL3 && !isBoth) std::cout << "(*,3)";
          //if (isLF && !isBoth) std::cout << "(L,*)";
          //if (isNone && !isBoth && !isL3 && !isLF) std::cout << "(*,*)";
      }
      //std::cout << std::endl;
      //if (deltaR(trigObject.eta(), trigObject.phi(),mu.eta(),mu.phi()) > 0.3) continue;
      //std::cout<<"trigObject pT: "<<trigObject.pt()<<" trigObject eta: "<<trigObject.eta()<<std::endl;
      trigObj_pT_.push_back(trigObject.pt());
      trigObj_eta_.push_back(trigObject.eta());
      trigObj_phi_.push_back(trigObject.phi());
      nTrigObj_++;
  }
  //std::cout<<"# trigObjects: "<<nTrigObj_<<std::endl;

}

void patHltTree::SetBranches(){

  AddBranch(&nTrigs_,"nTrigs");
  AddBranch(&nTrigObj_,"nTrigObj");
  AddBranch(&trigResult_,"trigResult");
  AddBranch(&trigName_,"trigName");
  AddBranch(&trigPrescale_,"trigPrescale");
  AddBranch(&trigObj_pT_,"trigObj_pT");
  AddBranch(&trigObj_eta_,"trigObj_eta");
  AddBranch(&trigObj_phi_,"trigObj_phi");
  AddBranch(&trigFilterLabels_,"trigFilterLabels");
  AddBranch(&trigPathName_,"trigPathName");

}

void
patHltTree::Clear(){
  nTrigs_ = 0;
  nTrigObj_ = 0;
  trigResult_.clear();
  trigName_.clear();
  trigPrescale_.clear();
  trigObj_pT_.clear();
  trigObj_eta_.clear();
  trigObj_phi_.clear();
  trigFilterLabels_.clear();
  trigPathName_.clear();
}
