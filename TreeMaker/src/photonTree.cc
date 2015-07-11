
#include "DelPanj/TreeMaker/interface/photonTree.h"

  photonTree::photonTree(std::string name, TTree* tree, const pset& iConfig):baseTree(name,tree){
  photonLabel_  = iConfig.getParameter<edm::InputTag> ("photonLabel");
  phoLooseIdMapToken_  =iConfig.getParameter<edm::InputTag>("phoLooseIdMap"),
  phoMediumIdMapToken_ =iConfig.getParameter<edm::InputTag>("phoMediumIdMap"),
  phoTightIdMapToken_  =iConfig.getParameter<edm::InputTag>("phoTightIdMap"),
  

  //usePFObjects_ = iConfig.getParameter<bool> ("usePFlow");
  SetBranches();
}

photonTree::~photonTree(){

}

void photonTree::Fill(const edm::Event& iEvent){
  Clear();
  //fetch the input collection
  //
  edm::Handle<edm::View<pat::Photon> > photonHandle;
 // edm::Handle<std::vector<pat::Photon> > photonHandle;
  if(not iEvent.getByLabel(photonLabel_,photonHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "<<photonLabel_<<std::endl; 
    exit(0);
  }  
  //pat::PhotonCollection phColl(*(photonHandle.product()));

 //sort the objects by transverse momentum
  //std::sort(phColl.begin(),phColl.end(),PtGreater());

  // IDs 
  edm::Handle<edm::ValueMap<bool> >  loose_id_decisions;
  edm::Handle<edm::ValueMap<bool> >  medium_id_decisions;
  edm::Handle<edm::ValueMap<bool> >  tight_id_decisions;

  
  iEvent.getByLabel(phoLooseIdMapToken_,  loose_id_decisions);
  iEvent.getByLabel(phoMediumIdMapToken_,  medium_id_decisions);
  iEvent.getByLabel(phoTightIdMapToken_,  tight_id_decisions);



  edm::View<pat::Photon>::const_iterator ph;
  //pat::PhotonCollection::const_iterator ph;
  for(ph=photonHandle->begin(); ph!=photonHandle->end(); ph++){
    nPho_++;
    photonPt_.push_back(ph->pt());
    photonEta_.push_back(ph->eta());
    photonPhi_.push_back(ph->eta());
    photonE_.push_back(ph->energy());
    //Ids
    const auto pho = photonHandle->ptrAt(nPho_-1);
    std::cout<<" loose id = "<<(*loose_id_decisions)[pho]<<std::endl;

    isPassLoose.push_back((*loose_id_decisions)[pho]);
    isPassMedium.push_back((*medium_id_decisions)[pho]);
    isPassTight.push_back((*tight_id_decisions)[pho]);


  }  

}

void photonTree::SetBranches(){
  AddBranch(&nPho_  ,"nPho");
  AddBranch(&photonPt_ ,"phoPt");
  AddBranch(&photonEta_,"phoEta");
  AddBranch(&photonPhi_,"phoPhi");
  AddBranch(&photonE_,"phoE");
  AddBranch(&isPassTight,"isPassTight");
  AddBranch(&isPassLoose,"isPassLoose");
  AddBranch(&isPassMedium,"isPassMedium");

}

void photonTree::Clear(){
  nPho_ = 0; 
  photonPt_.clear();
  photonEta_.clear();
  photonPhi_.clear();
  photonE_.clear();
  isPassLoose.clear();
  isPassMedium.clear();
  isPassTight.clear();

}
