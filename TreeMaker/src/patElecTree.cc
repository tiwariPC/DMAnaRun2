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
  pvSrc_ (iConfig.getParameter<edm::InputTag>("pvSrc") ),
  patElecLabel_ ( iConfig.getParameter<edm::InputTag>("patElectrons"))
{
  SetBranches();
}
patElecTree::~patElecTree(){
}

void
patElecTree::Fill(const edm::Event& iEvent){
  Clear();
  edm::Handle<pat::ElectronCollection> patElecHandle;
    if(not iEvent.getByLabel(patElecLabel_,patElecHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "
	     <<patElecLabel_<<std::endl; exit(0);}
  
  edm::Handle<reco::VertexCollection> recVtxs_;
  reco::Vertex myPv;
  Int_t nVtx=0;
  Int_t ngoodVtx=0;
  if ( iEvent.getByLabel( pvSrc_, recVtxs_ )) {
    for (size_t i=0; i<recVtxs_->size(); ++i) {
      if (!((*recVtxs_)[i].isFake())){
	nVtx++;
	if(nVtx==1)myPv = (*recVtxs_)[i]; // minimum requirement, not a fake
	// further quality requirement
	if(((*recVtxs_)[i].ndof() >= 4 && fabs((*recVtxs_)[i].z()) <= 24 && fabs((*recVtxs_)[i].position().rho()) <= 2) )
	  {
	    ngoodVtx++;
	    if(ngoodVtx==1)myPv = (*recVtxs_)[i];
	  }
      }
      if(ngoodVtx<1 && nVtx<1 && recVtxs_->size()>0)
	myPv = (*recVtxs_)[0];
    } // loop over vertex collections
  } // if vertex handle is valid
  
  
  // Get rho value
  //
  //

  edm::Handle<double> rhoH;
  iEvent.getByLabel("fixedGridRhoAll",rhoH);
  //patElecRho_ = *rhoH;
  std::cout<<" ------------ rho value = "<<(*rhoH)<<std::endl;

  pat::ElectronCollection eleColl(*(patElecHandle.product()));
  std::sort(eleColl.begin(),eleColl.end(),PtGreater());

  pat::ElectronCollection::const_iterator ele;
  for(ele=eleColl.begin(); ele!=eleColl.end(); ele++){
    nEle_++;
    
    patElecCharge_.push_back(ele->charge());
    patElecChargeConsistent_.push_back(ele->isGsfCtfScPixChargeConsistent());
    
    // Add TClonesArray instead of this
    patElecEt_.push_back(ele->et());
    patElecEnergy_.push_back(ele->energy());
    patElecPt_.push_back(ele->pt());
    patElecEta_.push_back(ele->eta());
    patElecPhi_.push_back(ele->phi());
    patElecM_.push_back(ele->mass());
    
    patElecR9_.push_back(ele->r9());
    patElecHoverE_.push_back(ele->hcalOverEcal());
    
    // fix this 
    //patElecD0_.push_back(ele->gsfTrack()->dxy(myPv));
    //patElecDz_.push_back(ele->gsfTrack()->dz(myPv));
    patElecScEn_.push_back(ele->superCluster()->energy());
    patElecScPreEn_.push_back(ele->superCluster()->preshowerEnergy());
    patElecScEta_.push_back(ele->superCluster()->eta());
    patElecScPhi_.push_back(ele->superCluster()->phi());
    patElecScRawEn_.push_back(ele->superCluster()->rawEnergy());
    patElecScEtaWidth_.push_back(ele->superCluster()->etaWidth());
    patElecScPhiWidth_.push_back(ele->superCluster()->phiWidth());
    patElecEoverP_.push_back(ele->eSuperClusterOverP());
    patElecBrem_.push_back(ele->fbrem());
    patElecdEtaAtVtx_.push_back(ele->deltaEtaSuperClusterTrackAtVtx());
    patElecdPhiAtVtx_.push_back(ele->deltaPhiSuperClusterTrackAtVtx());
    patElecSigmaIEtaIEta_.push_back(ele->sigmaIetaIeta()); ///new sigmaietaieta
    patElecSigmaIEtaIPhi_.push_back(ele->sigmaIetaIphi());
    patElecSigmaIPhiIPhi_.push_back(ele->sigmaIphiIphi());
    // make this boolean
    patElecConvVeto_.push_back((Int_t)ele->passConversionVeto()); // ConvVtxFit || missHit == 0
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
    
    //fix the initial value
    Float_t iso1 = 999.;
    Float_t iso2 = 999.;
    Float_t iso3 = 999.;
    Float_t isoPU = -999.;
    iso1 = ele->pfIsolationVariables().sumChargedHadronPt;
    iso2 = ele->pfIsolationVariables().sumNeutralHadronEt;
    iso3 = ele->pfIsolationVariables().sumPhotonEt;
    isoPU = ele->pfIsolationVariables().sumPUPt;
    Float_t corrPfIso = iso1 + std::max(0.0, iso2 + iso3 - 0.5*isoPU );
    patElecChHadIso_.push_back(iso1);
    patElecNeHadIso_.push_back(iso2);
    patElecGamIso_.push_back(iso3);
    patElecPUPt_.push_back(isoPU);
    patElecCorrPfIso_.push_back(corrPfIso);
    // this variable is for debugging
    patElecaloEnergy_.push_back(ele->caloEnergy());

    patElecSigmaIEtaIEtaFull5x5_.push_back(ele->full5x5_sigmaIetaIeta());
    patElecE1x5Full5x5_.push_back(ele->full5x5_e1x5());
    patElecE2x5Full5x5_.push_back(ele->full5x5_e2x5Max());
    patElecE5x5Full5x5_.push_back(ele->full5x5_e5x5());
    patElecR9Full5x5_.push_back(ele->full5x5_r9());
    ///For HEEP ID
    patElecEcalDrivenSeed_.push_back(ele->ecalDrivenSeed());
    patElecDr03EcalRecHitSumEt_.push_back(ele->dr03EcalRecHitSumEt());
    patElecDr03HcalDepth1TowerSumEt_.push_back(ele->dr03HcalDepth1TowerSumEt());
    patElecDr03HcalDepth2TowerSumEt_.push_back(ele->dr03HcalDepth2TowerSumEt());
    patElecDr03HcalTowerSumEt_ .push_back(ele->dr03HcalTowerSumEt());
    patElecDr03TkSumPt_ .push_back(ele->dr03TkSumPt());
    reco::GsfTrackRef trackref = ele->gsfTrack();
    // Fix this impact parameter
    /* if (ele->gsfTrack().isNonnull()) {
       if (recVtxs->size() > 0){
       patElecTrkdz_.push_back(trackref->dz(recVtxs->front().position())); 
       patElecTrkdxy_.push_back(trackref->dxy(recVtxs->front().position()));
       }
       }*/
    patElecInBarrel_.push_back(ele->isEB());
    patElecInEndcap_.push_back(ele->isEE());
  }
}

  
  void
    patElecTree::SetBranches(){
    AddBranch(&nEle_, "nEle");
    AddBranch(&patElecRho_, "eleRho");
    AddBranch(&patElecCharge_, "eleCharge");
    AddBranch(&patElecChargeConsistent_,"eleChargeConsistent");
    AddBranch(&patElecEt_, "eleEt");
    AddBranch(&patElecEnergy_, "eleEnergy");
    AddBranch(&patElecPt_, "elePt");
    AddBranch(&patElecEta_, "eleEta");
    AddBranch(&patElecPhi_, "elePhi");
    AddBranch(&patElecM_, "eleM");
    AddBranch(&patElecR9_,"eleR9");
    AddBranch(&patElecHoverE_,"eleHoverE");
    AddBranch(&patElecD0_,"eleD0");
    AddBranch(&patElecDz_,"eleDz");
    AddBranch(&patElecScEn_,"eleScEn");
    AddBranch(&patElecScPreEn_,"eleScPreEn");
    AddBranch(&patElecScEta_,"eleScEta");
    AddBranch(&patElecScPhi_,"eleScPhi");
    AddBranch(&patElecScRawEn_,"eleScRawEn");
    AddBranch(&patElecScEtaWidth_,"eleScEtaWidth");
    AddBranch(&patElecScPhiWidth_,"eleScPhiWidth");
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
    AddBranch(&patElecChHadIso_, "eleChHadIso");
    AddBranch(&patElecNeHadIso_, "eleNeHadIso");
    AddBranch(&patElecGamIso_, "eleGamIso");
    AddBranch(&patElecPUPt_, "elePUPt");
    AddBranch(&patElecCorrPfIso_, "eleCorrPfIso");
    AddBranch(&patElecaloEnergy_,"elecaloEnergy");
    AddBranch(&patElecSigmaIEtaIEtaFull5x5_,"eleSigmaIEtaIEtaFull5x5");
    AddBranch(&patElecE1x5Full5x5_,"eleE1x5Full5x5");
    AddBranch(&patElecE2x5Full5x5_,"eleE2x5Full5x5");
    AddBranch(&patElecE5x5Full5x5_,"eleE5x5Full5x5");
    AddBranch(&patElecR9Full5x5_,"eleR9Full5x5");
    AddBranch(&patElecEcalDrivenSeed_,"eleEcalDrivenSeed");
    AddBranch(&patElecDr03EcalRecHitSumEt_,"eleDr03EcalRecHitSumEt");
    AddBranch(&patElecDr03HcalDepth1TowerSumEt_,"eleDr03HcalDepth1TowerSumEt");
    AddBranch(&patElecDr03HcalDepth2TowerSumEt_,"eleDr03HcalDepth2TowerSumEt");
    AddBranch(&patElecDr03HcalTowerSumEt_,"eleDr03HcalTowerSumEt");
    AddBranch(&patElecDr03TkSumPt_,"eleDr03TkSumPt");
    AddBranch(&patElecTrkdztrackref_,"eleTrkdztrackref");
    AddBranch(&patElecTrkdxytrackref_,"eleTrkdxytrackref");
    AddBranch(&patElecInBarrel_,"eleInBarrel");
    AddBranch(&patElecInEndcap_,"eleInEndcap");
  }
  void
    patElecTree::Clear(){
    nEle_ =0;
    patElecRho_ =-999;
    patElecCharge_.clear();
    patElecChargeConsistent_.clear();
    patElecEt_.clear();
    patElecEnergy_.clear();
    patElecPt_.clear();
    patElecEta_.clear();
    patElecPhi_.clear();
    patElecM_.clear();
    patElecR9_.clear();
    patElecHoverE_.clear();
    patElecD0_.clear();
    patElecDz_.clear();
    patElecScEn_.clear();
    patElecScPreEn_.clear();
    patElecScEta_.clear();
    patElecScPhi_.clear();
    patElecScRawEn_.clear();
    patElecScEtaWidth_.clear();
    patElecScPhiWidth_.clear();
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
    patElecNeHadIso_.clear();
    patElecGamIso_.clear();
    patElecPUPt_.clear();

    patElecSigmaIEtaIEtaFull5x5_.clear();
    patElecE1x5Full5x5_.clear();
    patElecE2x5Full5x5_.clear();
    patElecE5x5Full5x5_.clear();
    patElecR9Full5x5_.clear();
    patElecEcalDrivenSeed_.clear();
    patElecDr03EcalRecHitSumEt_.clear();
    patElecDr03HcalDepth1TowerSumEt_.clear();
    patElecDr03HcalDepth2TowerSumEt_.clear();
    patElecDr03HcalTowerSumEt_.clear();
    patElecDr03TkSumPt_.clear();
    patElecTrkdztrackref_.clear();
    patElecTrkdxytrackref_.clear();
    patElecInBarrel_.clear();
    patElecInEndcap_.clear();
    patElecE1x5Full5x5_.clear();
    patElecE2x5Full5x5_.clear();
    patElecE5x5Full5x5_.clear();
    patElecR9Full5x5_.clear();
    patElecEcalDrivenSeed_.clear();
    patElecDr03EcalRecHitSumEt_.clear();
    patElecDr03HcalDepth1TowerSumEt_.clear();
    patElecDr03HcalDepth2TowerSumEt_.clear();
    patElecDr03HcalTowerSumEt_.clear();
    patElecDr03TkSumPt_.clear();
    patElecTrkdztrackref_.clear();
    patElecTrkdxytrackref_.clear();
    patElecInBarrel_.clear();
    patElecInEndcap_.clear();

    
 }

