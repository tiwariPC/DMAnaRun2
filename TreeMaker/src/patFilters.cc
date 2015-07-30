// Updated By : Raman Khurana 
// Dated      : Mon May 25 15:40:47 CDT 2015
// Added possible triggers for DM analysis, Jets and MET
#include "DelPanj/TreeMaker/interface/patFilters.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "FWCore/Common/interface/TriggerNames.h" 
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
patFilters::patFilters(std::string name,TTree* tree, const edm::ParameterSet& iConfig ):
  baseTree(name,tree),
  nfilters_(0),
  filterTag(iConfig.getParameter<edm::InputTag>("filterLabel"))
{
  SetBranches();
}

void
patFilters::Fill(const edm::Event& iEvent)
{
  Clear();
  using namespace edm;
  

  edm::Handle<bool> HBHET;
  edm::InputTag  hbhetag("HBHENoiseFilterResultProducer","HBHENoiseFilterResultRun2Tight","MVAMET");
  iEvent.getByLabel(hbhetag,HBHET);
  hbhet_ = (*HBHET.product());
  std::cout<<" HBHE = "<<(*HBHET.product())<<std::endl;
  
  
  edm::Handle<edm::TriggerResults> trigResults;
  if (not iEvent.getByLabel(filterTag, trigResults)) {
    std::cout << ">>> TRIGGER collection for filters does not exist !!!\n";
    return;
  }

  const edm::TriggerNames & trigNames = iEvent.triggerNames(*trigResults);
  //const std::vector<std::string> & triggerNames_ = trigNames.triggerNames();

  for (unsigned int i=0; i<trigResults->size(); i++)
    {
      std::string trigName = trigNames.triggerName(i);
      // lepton triggers
      size_t foundallFlag=trigName.find("FLAG_");
            

      if(false) std::cout<<" trigName = "<<trigName
			<<" : "<<trigResults->accept(i)
			<<std::endl;
      
      if ( foundallFlag==std::string::npos  )     	continue;
      

      
      filterName_.push_back(trigName);
      bool trigResult = trigResults->accept(i); //bool not to use
      filterResult_.push_back(trigResult);
      nfilters_++;
    }
}

void patFilters::SetBranches(){
  
  AddBranch(&nfilters_,"nfilters");
  AddBranch(&filterResult_,"filterResult");
  AddBranch(&hbhet_,"hbhet");
  AddBranch(&filterName_,"filterName");


}

void
patFilters::Clear(){
  nfilters_ = 0;
  filterResult_.clear();
  filterName_.clear();
  hbhet_ = false;
}


