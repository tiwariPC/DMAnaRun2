
#include "DelPanj/TreeMaker/interface/photonTree.h"

photonTree::photonTree(std::string name, TTree* tree, const pset& iConfig):
  baseTree(name,tree),
  photonLabel_(iConfig.getParameter<edm::InputTag> ("photonLabel")),
  phoLooseIdMapToken_(iConfig.getParameter<edm::InputTag>("phoLooseIdMap")),
  phoMediumIdMapToken_(iConfig.getParameter<edm::InputTag>("phoMediumIdMap")),
  phoTightIdMapToken_(iConfig.getParameter<edm::InputTag>("phoTightIdMap"))
{
  photonP4_ =   new TClonesArray("TLorentzVector");
  SetBranches();
}

photonTree::~photonTree(){
  delete photonP4_;
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
    if(ph->pt() < 10.) continue;
    if(TMath::Abs(ph->eta()) > 2.7) continue;
    nPho_++;
    new( (*photonP4_)[nPho_-1]) TLorentzVector(
					       ph->p4().px(),
					       ph->p4().py(),
					       ph->p4().pz(),
					       ph->p4().energy()
					       );
    //Ids
    const auto pho = photonHandle->ptrAt(nPho_-1);
    // std::cout<<" loose id = "<<(*loose_id_decisions)[pho]<<std::endl;

    isPassLoose.push_back((*loose_id_decisions)[pho]);
    isPassMedium.push_back((*medium_id_decisions)[pho]);
    isPassTight.push_back((*tight_id_decisions)[pho]);


  }  

}

void photonTree::SetBranches(){
  AddBranch(&nPho_  ,"nPho");
  AddBranch(&photonP4_, "phoP4");
  AddBranch(&isPassTight,"phoIsPassTight");
  AddBranch(&isPassLoose,"phoIsPassLoose");
  AddBranch(&isPassMedium,"phoIsPassMedium");

}

void photonTree::Clear(){
  nPho_ = 0; 
  photonP4_->Clear();
  isPassLoose.clear();
  isPassMedium.clear();
  isPassTight.clear();

}
