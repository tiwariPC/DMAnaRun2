
#include "DelPanj/TreeMaker/interface/photonTree.h"

photonTree::photonTree(std::string name, TTree* tree, const pset& iConfig):
  baseTree(name,tree),
  photonLabel_(iConfig.getParameter<edm::InputTag> ("photonLabel")),
  phoLooseIdMapToken_(iConfig.getParameter<edm::InputTag>("phoLooseIdMap")),
  phoMediumIdMapToken_(iConfig.getParameter<edm::InputTag>("phoMediumIdMap")),
  phoTightIdMapToken_(iConfig.getParameter<edm::InputTag>("phoTightIdMap")),
  
  phoChargedIsolationToken_(iConfig.getParameter<edm::InputTag>("phoChargedIsolationToken")),
  phoNeutralHadronIsolationToken_(iConfig.getParameter<edm::InputTag>("phoNeutralHadronIsolationToken")),
  phoPhotonIsolationToken_(iConfig.getParameter<edm::InputTag>("phoPhotonIsolationToken"))
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

  
  edm::Handle<edm::ValueMap<float> > phoChargedIsolationMap;
  edm::Handle<edm::ValueMap<float> > phoNeutralHadronIsolationMap;
  edm::Handle<edm::ValueMap<float> > phoPhotonIsolationMap;

  
  iEvent.getByLabel(phoLooseIdMapToken_,  loose_id_decisions);
  iEvent.getByLabel(phoMediumIdMapToken_,  medium_id_decisions);
  iEvent.getByLabel(phoTightIdMapToken_,  tight_id_decisions);

  
  iEvent.getByLabel(phoChargedIsolationToken_,       phoChargedIsolationMap);
  iEvent.getByLabel(phoNeutralHadronIsolationToken_, phoNeutralHadronIsolationMap);
  iEvent.getByLabel(phoPhotonIsolationToken_,        phoPhotonIsolationMap);


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


    phoPFChIso_              .push_back((*phoChargedIsolationMap)[pho]);
    phoPFPhoIso_             .push_back((*phoPhotonIsolationMap)[pho]);
    phoPFNeuIso_             .push_back((*phoNeutralHadronIsolationMap)[pho]);

    // --------
    // Photon ID variables used to make photon ID booleans. 
    // --------
    
    phoSCE_           .push_back((*ph).superCluster()->energy());
    phoSCRawE_        .push_back((*ph).superCluster()->rawEnergy());
    phoSCEta_         .push_back((*ph).superCluster()->eta());
    phoSCPhi_         .push_back((*ph).superCluster()->phi());
    phoSCEtaWidth_    .push_back((*ph).superCluster()->etaWidth());
    phoSCPhiWidth_    .push_back((*ph).superCluster()->phiWidth());
    phoSCBrem_        .push_back((*ph).superCluster()->phiWidth()/(*ph).superCluster()->etaWidth());
    phohasPixelSeed_  .push_back((Int_t)ph->hasPixelSeed());
    phoEleVeto_       .push_back((Int_t)ph->passElectronVeto());
    phoR9_            .push_back(ph->r9());
    phoHoverE_        .push_back(ph->hadTowOverEm());
    phoSigmaIEtaIEta_ .push_back(ph->see());
    phoSigmaIEtaIPhi_ .push_back(ph->sep());
    phoSigmaIPhiIPhi_ .push_back(ph->spp());
    phoSigmaIEtaIEtaFull5x5_ .push_back(ph->full5x5_sigmaIetaIeta());
    phoR9Full5x5_            .push_back(ph->full5x5_r9());
    



  }  

}

void photonTree::SetBranches(){
  AddBranch(&nPho_  ,"nPho");
  AddBranch(&photonP4_, "phoP4");
  AddBranch(&isPassTight,"phoIsPassTight");
  AddBranch(&isPassLoose,"phoIsPassLoose");
  AddBranch(&isPassMedium,"phoIsPassMedium");
  
  AddBranch(&phoSCE_,"phoSCE_");
  AddBranch(&phoSCRawE_,"phoSCRawE_");
  AddBranch(&phoSCEta_,"phoSCEta_");
  AddBranch(&phoSCPhi_,"phoSCPhi_");
  AddBranch(&phoSCEtaWidth_,"phoSCEtaWidth_");
  AddBranch(&phoSCPhiWidth_,"phoSCPhiWidth_");
  AddBranch(&phoSCBrem_,"phoSCBrem_");
  AddBranch(&phohasPixelSeed_,"phohasPixelSeed_");
  AddBranch(&phoEleVeto_,"phoEleVeto_");
  AddBranch(&phoR9_,"phoR9_");
  AddBranch(&phoHoverE_,"phoHoverE_");
  AddBranch(&phoSigmaIEtaIEta_,"phoSigmaIEtaIEta_");
  AddBranch(&phoSigmaIEtaIPhi_,"phoSigmaIEtaIPhi_");
  AddBranch(&phoSigmaIPhiIPhi_,"phoSigmaIPhiIPhi_");
  AddBranch(&phoSigmaIEtaIEtaFull5x5_,"phoSigmaIEtaIEtaFull5x5_");
  AddBranch(&phoR9Full5x5_,"phoR9Full5x5_");
  AddBranch(&phoPFChIso_,"phoPFChIso_");
  AddBranch(&phoPFPhoIso_,"phoPFPhoIso_");
  AddBranch(&phoPFNeuIso_,"phoPFNeuIso_");

}

void photonTree::Clear(){
  nPho_ = 0; 
  photonP4_->Clear();
  isPassLoose.clear();
  isPassMedium.clear();
  isPassTight.clear();

}
