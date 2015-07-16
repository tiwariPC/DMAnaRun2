#include "DelPanj/TreeMaker/interface/patMetTree.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
patMetTree::patMetTree(std::string name, TTree* tree, const edm::ParameterSet& iConfig):
  baseTree(name,tree)
{
  pfMetRawLabel_  = iConfig.getParameter<edm::InputTag>("pfMetRaw");
  pfMetLabel_     = iConfig.getParameter<edm::InputTag>("pfType1Met");
  pfMVAMETLabel_  = iConfig.getParameter<edm::InputTag>("pfMVAMET");
  SetBranches();
}


patMetTree::~patMetTree(){
} 


void
patMetTree::Fill(const edm::Event& iEvent){
  Clear();
  /*  edm::Handle<pat::METCollection> patMetRawHandle;
  if(not iEvent.getByLabel(patMetRawLabel_,patMetRawHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "
	     <<patMetRawLabel_<<std::endl; exit(0);}
  */
  
  // adding Raw PF MET to the tree
  edm::Handle<reco::PFMETCollection> patMetRawHandle;
  if(not iEvent.getByLabel(pfMetRawLabel_,patMetRawHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "
	     <<pfMetRawLabel_<<std::endl; exit(0);}
  
  // adding Type-1 MET to the tree
  edm::Handle<pat::METCollection> patMetHandle;
  if(not iEvent.getByLabel(pfMetLabel_,patMetHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "
	     <<pfMetLabel_<<std::endl; exit(0);}

  
  // adding mva met to the tree
  edm::Handle<vector<reco::PFMET> > recomethandle;
  if(not iEvent.getByLabel(pfMVAMETLabel_,recomethandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "
	     <<pfMVAMETLabel_<<std::endl; exit(0);}

  // raw distributions
  // corrected distributions
  auto metraw=patMetRawHandle.product()->begin();
  patMetRawPt_ = metraw->et();
  patMetRawPhi_ = metraw->phi();
  patMetRawSumEt_ = metraw->sumEt();
  patMetRawCov00_ = metraw->getSignificanceMatrix()(0,0);
  patMetRawCov01_ = metraw->getSignificanceMatrix()(0,1);
  patMetRawCov10_ = metraw->getSignificanceMatrix()(1,0);
  patMetRawCov11_ = metraw->getSignificanceMatrix()(1,1);

  pat::METCollection::const_iterator met=patMetHandle.product()->begin();
  patMetCorrPt_    = met->et();
  patMetCorrPhi_   = met->phi();
  patMetCorrSumEt_ = met->sumEt();
  patMetCorrSig_   = met->significance() < 1.e10 ? met->significance() : 0;

  
  reco::PFMETCollection::const_iterator recmet=recomethandle.product()->begin();
  mvaMetPt_ = recmet->et();
  mvaMetPhi_ = recmet->phi();
  mvaMetSumEt_   = recmet->sumEt();
  mvaMetSig_   = recmet->significance() < 1.e10 ? recmet->significance() : 0;
    

  // std::cout<<"met = "<<met->et()
  // 	   <<" raw met = "<<metraw->et()
  // 	   << "correct met = "<<met->et()
  // 	   <<std::endl;
  


} 



void 
patMetTree::SetBranches(){

  AddBranch(&patMetCorrPt_, "MetCorrPt");
  AddBranch(&patMetCorrPhi_, "MetCorrPhi"); 
  AddBranch(&patMetCorrSumEt_, "MetCorrSumEt");
  AddBranch(&patMetCorrSig_, "MetCorrSig");
  AddBranch(&patMetRawPt_, "MetRawPt");
  AddBranch(&patMetRawPhi_, "MetRawPhi");
  AddBranch(&patMetRawSumEt_, "MetRawSumEt");
  AddBranch(&patMetRawCov00_, "MetRawCov00");
  AddBranch(&patMetRawCov01_, "MetRawCov01");
  AddBranch(&patMetRawCov10_, "MetRawCov10");
  AddBranch(&patMetRawCov11_, "MetRawCov11");

  AddBranch(&mvaMetPt_,     "mvaMetPt_");
  AddBranch(&mvaMetPhi_,    "mvaMetPhi_");
  AddBranch(&mvaMetSumEt_,  "mvaMetSumEt_");
  AddBranch(&mvaMetSig_,    "mvaMetSig_");
}


void
patMetTree::Clear(){

  float dummy = -99999;
  patMetCorrPt_= dummy;  
  patMetCorrPhi_= dummy; 
  patMetCorrSumEt_= dummy;
  patMetCorrSig_= dummy;
  patMetRawPt_= dummy;
  patMetRawPhi_= dummy;
  patMetRawSumEt_= dummy;
  patMetRawCov00_= dummy;
  patMetRawCov01_= dummy;
  patMetRawCov10_= dummy;
  patMetRawCov11_= dummy;

  mvaMetPt_     = dummy ;
  mvaMetPhi_    = dummy ;
  mvaMetSumEt_  = dummy ;
  mvaMetSig_    = dummy ;


}
