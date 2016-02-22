// Updated By : Raman Khurana 
// Dated      : Mon May 25 15:40:47 CDT 2015
// Added possible triggers for DM analysis, Jets and MET
#include "DelPanj/TreeMaker/interface/patHltTree.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "FWCore/Common/interface/TriggerNames.h" 
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
 #include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

patHltTree::patHltTree(std::string name,TTree* tree, const edm::ParameterSet& iConfig ):
  baseTree(name,tree),
  nTrigs_(0),
  trigTag(iConfig.getParameter<edm::InputTag>("triggerLabel"))
{
  SetBranches();
}

void
patHltTree::Fill(const edm::Event& iEvent)
{
  Clear();
  using namespace edm;
  
  edm::Handle<edm::TriggerResults> trigResults;
  edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
  iEvent.getByLabel("patTrigger",triggerPrescales);
  //edm::InputTag trigTag("TriggerResults::HLT");
  if (not iEvent.getByLabel(trigTag, trigResults)) {
    std::cout << ">>> TRIGGER collection does not exist !!!\n";
    return;
  }

  const edm::TriggerNames & trigNames = iEvent.triggerNames(*trigResults);
  
  //const std::vector<std::string> & triggerNames_ = trigNames.triggerNames();

  for (unsigned int i=0; i<trigResults->size(); i++)
    {
      std::string trigName = trigNames.triggerName(i);
      // lepton triggers
      size_t foundEle_1=trigName.find("HLT_Ele22_eta2p1_WPLoose_Gsf");
      size_t foundEle_2=trigName.find("HLT_Ele22_eta2p1_WP75_Gsf");
      size_t foundDEle33=trigName.find("HLT_DoubleEle33");
      size_t foundEle17_Ele12=trigName.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL");
      size_t foundEle27=trigName.find("HLT_Ele27_eta2p1_WPLoose_Gsf");
      size_t foundEle105=trigName.find("HLT_Ele105");
      size_t foundEle23_1=trigName.find("HLT_Ele23_WPLoose_Gsf_v");
      size_t foundEle23_2=trigName.find("HLT_Ele23_WP75_Gsf");
      size_t foundEle27_2=trigName.find("HLT_Ele27_WP85_Gsf_v");
      size_t foundMu17_Mu8=trigName.find("HLT_Mu17_Mu8");
      size_t foundMu17_TkMu8=trigName.find("HLT_Mu17_TkMu8");
      size_t foundMu30_TkMu11=trigName.find("HLT_Mu30_TkMu11");
      size_t foundMu45=trigName.find("HLT_Mu45");

      // jet, met triggers
      size_t foundMET120HT120                = trigName.find("HLT_PFMET120_PFMHT120_");
      size_t foundMETBTag                    = trigName.find("HLT_PFMET120_NoiseCleaned_BTagCSV07");
      size_t foundPFMET170                   = trigName.find("HLT_PFMET170_NoiseCleaned");
      size_t foundPFMET190MHT90              = trigName.find("HLT_PFMET90_PFMHT90_");
      size_t founfPFMET120MHT120             = trigName.find("HLT_PFMETNoMu120_NoiseCleaned_PFMHTNoMu120_IDTight");
      size_t foundPFMET170NoiseClean         = trigName.find("HLT_PFMET170_NoiseCleaned");
      size_t foundDiCEntralPFJet70PFMET120   = trigName.find("HLT_DiCentralPFJet70_PFMET120_NoiseCleaned");
      size_t foundPFHT350_PFMET120           = trigName.find("HLT_PFHT350_PFMET120_NoiseCleaned");



      size_t DiHiggs_1  = trigName.find("HLT_AK8PFJet360_TrimMass30_v");
      size_t DiHiggs_2  = trigName.find("HLT_AK8PFHT700_TrimR0p1PT0p03Mass50_v");
      size_t DiHiggs_3  = trigName.find("HLT_AK8DiPFJet280_200_TrimMass30_BTagCSV0p4");
      size_t DiHiggs_4  = trigName.find("HLT_PFHT900_v");
      size_t DiHiggs_5  = trigName.find("HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v");
      size_t DiHiggs_6  = trigName.find("HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v");
      size_t DiHiggs_7  = trigName.find("HLT_PFHT800");
            

      if ( foundDEle33==std::string::npos &&
	   foundEle_1==std::string::npos &&
           foundEle_2==std::string::npos &&
	   foundEle23_1==std::string::npos &&
	   foundEle23_2==std::string::npos &&
	   foundEle27_2==std::string::npos &&
	   foundEle27==std::string::npos && 
	   foundEle17_Ele12==std::string::npos && 
	   foundEle105==std::string::npos &&
       	   foundMu17_Mu8==std::string::npos && 
       	   foundMu17_TkMu8==std::string::npos && 
       	   foundMu30_TkMu11==std::string::npos &&
       	   foundMu45==std::string::npos &&
	   foundMET120HT120==std::string::npos &&
	   foundMETBTag==std::string::npos &&
	   foundPFMET170==std::string::npos &&
	   foundPFMET190MHT90==std::string::npos &&
	   founfPFMET120MHT120==std::string::npos &&
	   foundPFMET170NoiseClean==std::string::npos &&
	   foundDiCEntralPFJet70PFMET120==std::string::npos &&
	   foundPFHT350_PFMET120==std::string::npos &&
           DiHiggs_1==std::string::npos  &&
           DiHiggs_2==std::string::npos  &&
           DiHiggs_3==std::string::npos  && 
           DiHiggs_4==std::string::npos  &&
           DiHiggs_5==std::string::npos  &&
           DiHiggs_6==std::string::npos  &&
           DiHiggs_7==std::string::npos 
	   )
       	continue;
      
      if(true) std::cout<<" trigName = "<<trigName
			<<" : "<<trigResults->accept(i)
			<<std::endl;
      


      
      trigName_.push_back(trigName);
      bool trigResult = trigResults->accept(i); //bool not to use
      trigResult_.push_back(trigResult);
      trigPrescale_.push_back(triggerPrescales->getPrescaleForIndex(i));
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


