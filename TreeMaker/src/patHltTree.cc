// Updated By : Raman Khurana, Shin-Shan Eiko Yu
// Dated      : Mon May 25 15:40:47 CDT 2015
// Added possible triggers for DM analysis, Jets and MET
#include "DelPanj/TreeMaker/interface/patHltTree.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "FWCore/Common/interface/TriggerNames.h"

patHltTree::patHltTree(std::string name, TTree* tree, const edm::ParameterSet& iConfig):
  baseTree(name,tree),
  saveAllTrigPaths_(iConfig.getParameter<bool>("saveAllTrigPaths")),
  nTrigs_(0)
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

  edm::Handle<edm::TriggerResults> trigResults;
  if (not iEvent.getByToken(trigResultsToken, trigResults)) {
    std::cout << ">>> TRIGGER collection does not exist !!!\n";
    return;
  }
  std::vector<std::string> triggerlist={"HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60","HLT_PFMETNoMu120_PFMHTNoMu120_IDTight","HLT_PFMETNoMu140_PFMHTNoMu140_IDTight","HLT_IsoMu27","HLT_IsoTkMu27","HLT_IsoMu24","HLT_IsoTkMu24","HLT_Ele27_WPTight_Gsf","HLT_Ele32_WPTight_Gsf_L1DoubleEG',HLT_Ele35_WPTight_Gsf,'HLT_Photon200"};

  const edm::TriggerNames & trigNames = iEvent.triggerNames(*trigResults);

  for (unsigned int i=0; i<trigResults->size(); i++)
    {
      std::string trigName = trigNames.triggerName(i);

      bool trigResult = trigResults->accept(i); //bool not to use
      // if(!trigResult && !saveAllTrigPaths_)continue;
      int prescale = triggerPrescales->getPrescaleForIndex(i);
      if(prescale!=1 && !saveAllTrigPaths_)continue;

      if(false) std::cout<<" trigName = "<<trigName <<" : " << trigResults->accept(i)<<" : " << triggerPrescales->getPrescaleForIndex(i)<<std::endl;
      std::string trigName_123 = trigName.substr(0, trigName.find("_v"));
      if (std::find(triggerlist.begin(), triggerlist.end(), trigName_123) == triggerlist.end()) continue;
      trigName_.push_back(trigName);
      trigResult_.push_back(trigResult);
      trigPrescale_.push_back(prescale);
      nTrigs_++;
    }
}

void patHltTree::SetBranches(){

  AddBranch(&nTrigs_,"nTrigs");
  AddBranch(&trigResult_,"trigResult");
  AddBranch(&trigName_,"trigName");
  AddBranch(&trigPrescale_,"trigPrescale");


}

void
patHltTree::Clear(){
  nTrigs_ = 0;
  trigResult_.clear();
  trigName_.clear();
  trigPrescale_.clear();
}
