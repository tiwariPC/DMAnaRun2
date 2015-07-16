#include "DelPanj/TreeMaker/interface/patElecTree.h"
// for conversion finder
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

// Issues to be resolved : 
// -- mypv
// -- rho
// -- fix the impact parameter
// -- make conv veto boolean
// -- fix the initial values for isolation

patElecTree::patElecTree(std::string name, TTree* tree, const edm::ParameterSet& iConfig):
  baseTree(name,tree),
  pvSrc_    (iConfig.getParameter<edm::InputTag>("pvSrc") ),   
  patElecLabel_( iConfig.getParameter<edm::InputTag>("patElectrons")),
  eleVetoIdMapLabel_( iConfig.getParameter<edm::InputTag>("eleVetoIdMap")),
  eleLooseIdMapLabel_( iConfig.getParameter<edm::InputTag>("eleLooseIdMap")),
  eleMediumIdMapLabel_(iConfig.getParameter<edm::InputTag>("eleMediumIdMap")),
  eleTightIdMapLabel_(iConfig.getParameter<edm::InputTag>("eleTightIdMap")),
  eleHEEPIdMapLabel_(iConfig.getParameter<edm::InputTag>("eleHEEPIdMap")),
  
  eleMVAMediumIdMapLabel_(iConfig.getParameter<edm::InputTag>("eleMVAMediumIdMap")),
  eleMVATightIdMapLabel_(iConfig.getParameter<edm::InputTag>("eleMVATightIdMap")),
  mvaValuesMapLabel_(iConfig.getParameter<edm::InputTag>("mvaValuesMap")),
  mvaCategoriesMapLabel_(iConfig.getParameter<edm::InputTag>("mvaCategoriesMap")),
  pfCandLabel_(iConfig.getParameter<edm::InputTag>("pfForMiniIso") ),
  r_iso_min_(iConfig.getParameter<double>("r_iso_min")),
  r_iso_max_(iConfig.getParameter<double>("r_iso_max")),
  kt_scale_(iConfig.getParameter<double>("kt_scale")),
  charged_only_(iConfig.getParameter<bool>("charged_only"))
{
  patElecP4_ =   new TClonesArray("TLorentzVector");
  SetBranches();
  
}
patElecTree::~patElecTree(){
  delete patElecP4_;
}

void
patElecTree::Fill(const edm::Event& iEvent){
  Clear();
  
  
  edm::Handle<edm::View<pat::Electron> > electronHandle;
  iEvent.getByLabel(patElecLabel_,electronHandle);
  //id boolean

  edm::Handle<edm::ValueMap<bool> > veto_id_decisions;
  edm::Handle<edm::ValueMap<bool> > loose_id_decisions;
  edm::Handle<edm::ValueMap<bool> > medium_id_decisions;
  edm::Handle<edm::ValueMap<bool> > tight_id_decisions; 
  edm::Handle<edm::ValueMap<bool> > heep_id_decisions;
  edm::Handle<edm::ValueMap<bool> > medium_MVAid_decisions;
  edm::Handle<edm::ValueMap<bool> > tight_MVAid_decisions; 
  iEvent.getByLabel(eleVetoIdMapLabel_ ,veto_id_decisions);
  iEvent.getByLabel(eleLooseIdMapLabel_ ,loose_id_decisions);
  iEvent.getByLabel(eleMediumIdMapLabel_,medium_id_decisions);
  iEvent.getByLabel(eleTightIdMapLabel_,tight_id_decisions);
  iEvent.getByLabel(eleHEEPIdMapLabel_ ,heep_id_decisions);
  iEvent.getByLabel(eleMediumIdMapLabel_,medium_MVAid_decisions);
  iEvent.getByLabel(eleTightIdMapLabel_,tight_MVAid_decisions);
  // std::cout<<" veto size  ---------------- "<<veto_id_decisions->size()<<std::endl;
  // std::cout<<" ele size  ---------------- "<<electronHandle->size()<<std::endl;

  // Get MVA values and categories (optional)
  edm::Handle<edm::ValueMap<float> > mvaValues;
  edm::Handle<edm::ValueMap<int> > mvaCategories;
  iEvent.getByLabel(mvaValuesMapLabel_,mvaValues);
  iEvent.getByLabel(mvaCategoriesMapLabel_,mvaCategories);

  edm::Handle<reco::VertexCollection> recVtxs;
  iEvent.getByLabel(pvSrc_, recVtxs);
   
  if (recVtxs->empty()) return; // skip the event if no PV found                                                                               
  vector<reco::Vertex>::const_iterator firstGoodVertex = recVtxs->end(); 
  //VertexCollection::const_iterator firstGoodVertex = recVtxs->end();
  int firstGoodVertexIdx = 0;
  //  for (VertexCollection::const_iterator vtx = recVtxs->begin(); vtx != recVtxs->end(); ++vtx, ++firstGoodVertexIdx) {
  for (vector<reco::Vertex>::const_iterator vtx = recVtxs->begin(); vtx != recVtxs->end(); ++vtx, ++firstGoodVertexIdx) {
 
    // Replace isFake() for miniAOD because it requires tracks and miniAOD recVtxs don't have tracks:                                          
    // Vertex.h: bool isFake() const {return (chi2_==0 && ndof_==0 && tracks_.empty());}                                                        
    // bool isFake = vtx->isFake();
    //if( !isAOD ) //we are here for MINIAOD only 
    bool isFake = (vtx->chi2()==0 && vtx->ndof()==0);
    // Check the goodness                                          
    if ( !isFake &&  vtx->ndof()>=4. && vtx->position().Rho()<=2.0 && fabs(vtx->position().Z())<=24.0) {
      firstGoodVertex = vtx;
      break;
    }
  }

  if ( firstGoodVertex==recVtxs->end() )
    return; // skip event if there are no good PVs                  


  // handle pfcandidates
  Handle<pat::PackedCandidateCollection> pfcands;
  iEvent.getByLabel(pfCandLabel_, pfcands);  
 
  // Get rho value
  edm::Handle<double> rhoH;
  iEvent.getByLabel("fixedGridRhoAll",rhoH);
  patElecRho_ = *rhoH;
  
  for (edm::View<pat::Electron>::const_iterator ele = electronHandle->begin(); ele != electronHandle->end(); ++ele) {

    if(ele->pt() < 5.) continue;
    nEle_++;

    new( (*patElecP4_)[nEle_-1]) TLorentzVector(
						ele->p4().px(),
						ele->p4().py(),
						ele->p4().pz(),
						ele->p4().energy()
						);

    patElecInBarrel_.push_back(ele->isEB());
    patElecInEndcap_.push_back(ele->isEE());
    
    patElecCharge_.push_back(ele->charge());
    patElecChargeConsistent_.push_back(ele->isGsfCtfScPixChargeConsistent());
    
    
    patElecaloEnergy_.push_back(ele->caloEnergy());

    double R = sqrt(ele->superCluster()->x()*ele->superCluster()->x() + ele->superCluster()->y()*ele->superCluster()->y() +ele->superCluster()->z()*ele->superCluster()->z());
    double Rt = sqrt(ele->superCluster()->x()*ele->superCluster()->x() + ele->superCluster()->y()*ele->superCluster()->y());
    patElecScEt_.push_back( (ele->superCluster()->energy())*(Rt/R) );
    patElecScEn_.push_back(ele->superCluster()->energy());
    patElecScPreEn_.push_back(ele->superCluster()->preshowerEnergy());
    patElecScEta_.push_back(ele->superCluster()->eta());
    patElecScPhi_.push_back(ele->superCluster()->phi());
    patElecScRawEn_.push_back(ele->superCluster()->rawEnergy());
    patElecScEtaWidth_.push_back(ele->superCluster()->etaWidth());
    patElecScPhiWidth_.push_back(ele->superCluster()->phiWidth());

    patElecR9_.push_back(ele->r9());
    patElecHoverE_.push_back(ele->hcalOverEcal());
    
    // fix this 
    patElecD0_.push_back(ele->gsfTrack()->dxy(firstGoodVertex->position()));
    patElecDz_.push_back(ele->gsfTrack()->dz(firstGoodVertex->position()));
    patElecEoverP_.push_back(ele->eSuperClusterOverP());
    patElecBrem_.push_back(ele->fbrem());
    patElecdEtaAtVtx_.push_back(ele->deltaEtaSuperClusterTrackAtVtx());
    patElecdPhiAtVtx_.push_back(ele->deltaPhiSuperClusterTrackAtVtx());
    patElecSigmaIEtaIEta_.push_back(ele->sigmaIetaIeta()); ///new sigmaietaieta
    patElecSigmaIEtaIPhi_.push_back(ele->sigmaIetaIphi());
    patElecSigmaIPhiIPhi_.push_back(ele->sigmaIphiIphi());

    patElecConvVeto_.push_back(ele->passConversionVeto()); // ConvVtxFit || missHit == 0
    patElecMissHits_.push_back(ele->gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS));
    if (ele->ecalEnergy() == 0) {
      patElecEoverPInv_.push_back(1e30);
    } else if (!std::isfinite(ele->ecalEnergy())) {
      patElecEoverPInv_.push_back(1e30);
    } else {
      patElecEoverPInv_.push_back(fabs(1./ele->ecalEnergy() - ele->eSuperClusterOverP()/ele->ecalEnergy()));
    }
    ///HEEP ID
    double eledEtaseedAtVtx = ele->superCluster().isNonnull() && ele->superCluster()->seed().isNonnull() ?
      ele->deltaEtaSuperClusterTrackAtVtx() - ele->superCluster()->eta() + ele->superCluster()->seed()->eta() : std::numeric_limits<float>::max();
    patElecdEtaseedAtVtx_.push_back(eledEtaseedAtVtx);
    patElecE1x5_.push_back(ele->e1x5());
    patElecE2x5_.push_back(ele->e2x5Max());
    patElecE5x5_.push_back(ele->e5x5());

    // this variable is for debugging

    patElecSigmaIEtaIEtaFull5x5_.push_back(ele->full5x5_sigmaIetaIeta());
    patElecE1x5Full5x5_.push_back(ele->full5x5_e1x5());
    patElecE2x5Full5x5_.push_back(ele->full5x5_e2x5Max());
    patElecE5x5Full5x5_.push_back(ele->full5x5_e5x5());
    patElecR9Full5x5_.push_back(ele->full5x5_r9());


    //To include in anlyzer code 
    /*    edm::FileInPath eaConstantsFile("EgammaAnalysis/ElectronTools/data/PHYS14/effAreaElectrons_cone03_pfNeuHadronsAndPhotons.txt");
	  EffectiveAreas effectiveAreas(eaConstantsFile.fullPath());
	  float eA = effectiveAreas.getEffectiveArea(fabs(ele->superCluster()->eta()));
	  patElecEffArea_.push_back(eA);    */

    //fix the initial value
    float iso1 = 999.;
    float iso2 = 999.;
    float iso3 = 999.;
    float isoPU = -999.;
    iso1 = ele->pfIsolationVariables().sumChargedHadronPt;
    iso2 = ele->pfIsolationVariables().sumNeutralHadronEt;
    iso3 = ele->pfIsolationVariables().sumPhotonEt;
    isoPU = ele->pfIsolationVariables().sumPUPt;
    patElecChHadIso_.push_back(iso1);
    patElecNeHadIso_.push_back(iso2);
    patElecGamIso_.push_back(iso3);
    patElecPUPt_.push_back(isoPU);

    double miniIso = getPFIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*ele)), r_iso_min_, r_iso_max_, kt_scale_, charged_only_);
    patElecMiniIso_.push_back(miniIso);

    ///For HEEP ID
    patElecEcalDrivenSeed_.push_back(ele->ecalDrivenSeed());
    patElecDr03EcalRecHitSumEt_.push_back(ele->dr03EcalRecHitSumEt());
    patElecDr03HcalDepth1TowerSumEt_.push_back(ele->dr03HcalDepth1TowerSumEt());
    patElecDr03HcalDepth2TowerSumEt_.push_back(ele->dr03HcalDepth2TowerSumEt());
    patElecDr03HcalTowerSumEt_ .push_back(ele->dr03HcalTowerSumEt());
    patElecDr03TkSumPt_ .push_back(ele->dr03TkSumPt());



    // reco::GsfTrackRef trackref = ele->gsfTrack();
    // Fix this impact parameter
    /* if (ele->gsfTrack().isNonnull()) {
       if (recVtxs->size() > 0){
       patElecTrkdz_.push_back(trackref->dz(recVtxs->front().position())); 
       patElecTrkdxy_.push_back(trackref->dxy(recVtxs->front().position()));
       }
       }*/
    
    const auto el = electronHandle->ptrAt(nEle_-1);
    // std::cout<<" veto id = "<<(*veto_id_decisions)[el]<<std::endl;
    
    isPassVeto_.push_back( (*veto_id_decisions)[el]);
    isPassLoose_.push_back( (*loose_id_decisions)[el]);
    isPassMedium_.push_back( (*medium_id_decisions)[el]);
    isPassTight_.push_back( (*tight_id_decisions)[el]);
    isPassHEEP_.push_back( (*heep_id_decisions)[el]);
    
    isPassMVAMedium_.push_back( (*medium_MVAid_decisions)[el]);
    isPassMVATight_.push_back( (*tight_MVAid_decisions)[el]);
    
    mvaValue_.push_back( (*mvaValues)[el] );
    mvaCategory_.push_back( (*mvaCategories)[el] );
    
  }
}

  
void
patElecTree::SetBranches(){

  AddBranch(&patElecRho_, "eleRho");
  AddBranch(&nEle_, "nEle");

  AddBranch(&patElecP4_,"patElecP4");

  AddBranch(&patElecInBarrel_,"eleInBarrel");
  AddBranch(&patElecInEndcap_,"eleInEndcap");


  AddBranch(&patElecCharge_, "eleCharge");
  AddBranch(&patElecChargeConsistent_,"eleChargeConsistent");


  AddBranch(&patElecaloEnergy_,"elecaloEnergy");

  AddBranch(&patElecScEt_,"eleScEt"); 
  AddBranch(&patElecScEn_,"eleScEn");
  AddBranch(&patElecScPreEn_,"eleScPreEn");
  AddBranch(&patElecScEta_,"eleScEta");
  AddBranch(&patElecScPhi_,"eleScPhi");
  AddBranch(&patElecScRawEn_,"eleScRawEn");
  AddBranch(&patElecScEtaWidth_,"eleScEtaWidth");
  AddBranch(&patElecScPhiWidth_,"eleScPhiWidth");

  AddBranch(&patElecR9_,"eleR9");
  AddBranch(&patElecHoverE_,"eleHoverE");

  AddBranch(&patElecD0_,"eleD0");
  AddBranch(&patElecDz_,"eleDz");

  AddBranch(&patElecEoverP_,"eleEoverP");
  AddBranch(&patElecBrem_,"eleBrem");
  AddBranch(&patElecdEtaAtVtx_,"eledEtaAtVtx");
  AddBranch(&patElecdPhiAtVtx_,"eledPhiAtVtx");
  AddBranch(&patElecSigmaIEtaIEta_,"eleSigmaIEtaIEta");
  AddBranch(&patElecSigmaIEtaIPhi_,"eleSigmaIEtaIPhi");
  AddBranch(&patElecSigmaIPhiIPhi_,"eleSigmaIPhiIPhi");

  AddBranch(&patElecConvVeto_,"eleConvVeto");
  AddBranch(&patElecMissHits_,"eleMissHits");
  AddBranch(&patElecEoverPInv_,"eleEoverPInv");

  AddBranch(&patElecdEtaseedAtVtx_,"eleEtaseedAtVtx");
  AddBranch(&patElecE1x5_,"eleE1x5");
  AddBranch(&patElecE2x5_,"eleE2x5");
  AddBranch(&patElecE5x5_,"eleE5x5");

  AddBranch(&patElecSigmaIEtaIEtaFull5x5_,"eleSigmaIEtaIEtaFull5x5");
  AddBranch(&patElecE1x5Full5x5_,"eleE1x5Full5x5");
  AddBranch(&patElecE2x5Full5x5_,"eleE2x5Full5x5");
  AddBranch(&patElecE5x5Full5x5_,"eleE5x5Full5x5");
  AddBranch(&patElecR9Full5x5_,"eleR9Full5x5");

  AddBranch(&patElecChHadIso_, "eleChHadIso");
  AddBranch(&patElecNeHadIso_, "eleNeHadIso");
  AddBranch(&patElecGamIso_, "eleGamIso");
  AddBranch(&patElecPUPt_, "elePUPt");
  AddBranch(&patElecMiniIso_,"eleMiniIso");

  AddBranch(&patElecEcalDrivenSeed_,"eleEcalDrivenSeed");
  AddBranch(&patElecDr03EcalRecHitSumEt_,"eleDr03EcalRecHitSumEt");
  AddBranch(&patElecDr03HcalDepth1TowerSumEt_,"eleDr03HcalDepth1TowerSumEt");
  AddBranch(&patElecDr03HcalDepth2TowerSumEt_,"eleDr03HcalDepth2TowerSumEt");
  AddBranch(&patElecDr03HcalTowerSumEt_,"eleDr03HcalTowerSumEt");
  AddBranch(&patElecDr03TkSumPt_,"eleDr03TkSumPt");
   
  AddBranch(&isPassVeto_,"isPassVeto");
  AddBranch(&isPassLoose_,"isPassLoose");
  AddBranch(&isPassMedium_,"isPassMedium");
  AddBranch(&isPassTight_,"isPassTight");
  AddBranch(&isPassHEEP_,"isPassHEEP");
  AddBranch(&isPassMVAMedium_,"isPassMVAMedium");
  AddBranch(&isPassMVATight_,"isPassMVATight");

  AddBranch(&mvaValue_,"mvaValue");
  AddBranch(&mvaCategory_,"mvaCategory");




}
void
patElecTree::Clear(){
    
    
  patElecRho_ =-999;
  nEle_ =0;
  patElecP4_->Clear();

  patElecInBarrel_.clear();
  patElecInEndcap_.clear();
    

  patElecCharge_.clear();
  patElecChargeConsistent_.clear();

  patElecaloEnergy_.clear();

  patElecScEt_.clear(); 
  patElecScEn_.clear();
  patElecScPreEn_.clear();
  patElecScEta_.clear();
  patElecScPhi_.clear();
  patElecScRawEn_.clear();
  patElecScEtaWidth_.clear();
  patElecScPhiWidth_.clear();

  patElecR9_.clear();
  patElecHoverE_.clear();

  patElecD0_.clear();
  patElecDz_.clear();

  patElecEoverP_.clear();
  patElecBrem_.clear();
  patElecdEtaAtVtx_.clear();
  patElecdPhiAtVtx_.clear();
  patElecSigmaIEtaIEta_.clear();
  patElecSigmaIEtaIPhi_.clear();
  patElecSigmaIPhiIPhi_.clear();

  patElecConvVeto_.clear();
  patElecMissHits_.clear();
  patElecEoverPInv_.clear();

  patElecdEtaseedAtVtx_.clear();
  patElecE1x5_.clear();
  patElecE2x5_.clear();
  patElecE5x5_.clear();

  patElecSigmaIEtaIEtaFull5x5_.clear();
  patElecE1x5Full5x5_.clear();
  patElecE2x5Full5x5_.clear();
  patElecE5x5Full5x5_.clear();
  patElecR9Full5x5_.clear();

  patElecChHadIso_.clear();
  patElecNeHadIso_.clear();
  patElecGamIso_.clear();
  patElecPUPt_.clear();
  patElecMiniIso_.clear();

  patElecEcalDrivenSeed_.clear();
  patElecDr03EcalRecHitSumEt_.clear();
  patElecDr03HcalDepth1TowerSumEt_.clear();
  patElecDr03HcalDepth2TowerSumEt_.clear();
  patElecDr03HcalTowerSumEt_.clear();
  patElecDr03TkSumPt_.clear();
   
  isPassVeto_.clear();
  isPassLoose_.clear();
  isPassMedium_.clear();
  isPassTight_.clear();
  isPassHEEP_.clear();
  isPassMVAMedium_.clear();
  isPassMVATight_.clear();

  mvaValue_.clear();
  mvaCategory_.clear();
    
}

