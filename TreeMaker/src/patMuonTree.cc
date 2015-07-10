//****************************************//
//  Code for Muons for Run II    //
//    Raman Khurana              //
//    Monika Mittal Khurana      // 
// Wed May 27 09:51:24 CEST 2015 //
//***************************************//

#include "DelPanj/TreeMaker/interface/patMuonTree.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "Math/VectorUtil.h"

patMuonTree::patMuonTree(std::string name, TTree* tree, const edm::ParameterSet& iConfig):
  baseTree(name,tree),
  
  patMuonLabel_ (iConfig.getParameter<edm::InputTag>("patMuons")){
  SetBranches();}

patMuonTree::~patMuonTree(){ }
void

patMuonTree::Fill(const edm::Event& iEvent){
  Clear();
  
  edm::Handle<pat::MuonCollection> patMuonHandle;
  if(not iEvent.getByLabel(patMuonLabel_,patMuonHandle)){
    std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "
	     <<patMuonLabel_<<std::endl; exit(0);}
  
  pat::MuonCollection muColl(*(patMuonHandle.product()));
  std::sort(muColl.begin(),muColl.end(),PtGreater());
  
  bool isAOD = 0; 
 
  edm::Handle<reco::VertexCollection> vtxHandle;
  iEvent.getByLabel("offlineSlimmedPrimaryVertices", vtxHandle);
  //best-known primary vertex coordinates
    math::XYZPoint pv(0, 0, 0);
  for (vector<reco::Vertex>::const_iterator v = vtxHandle->begin(); v != vtxHandle->end(); ++v) {
    //replace isFake() for miniAOD since it requires tracks while miniAOD vertices don't have tracks:
    // Vertex.h: bool isFake() const {return (chi2_==0 && ndof_==0 && tracks_.empty());}
    bool isFake = isAOD ? v->isFake() : (v->chi2() == 0 && v->ndof() == 0);
    if (!isFake) {
      pv.SetXYZ(v->x(), v->y(), v->z());
      break;
    }
  }
  
  
  pat::MuonCollection::const_iterator mu;
  
  for(mu=muColl.begin(); mu!=muColl.end(); mu++){
    // for finding shared segments reason not known
    int muonIndex=-1;
    int tempTrackIndex=-1;
    int tempSegIndex=-1;
    int nSegments=-1;
    for(pat::MuonCollection::const_iterator jm=muColl.begin(); jm!=muColl.end(); jm++){
      muonIndex++;
      if(mu->innerTrack()==jm->innerTrack() && tempTrackIndex==-1)
	tempTrackIndex=muonIndex;
      if(jm->isTrackerMuon() && mu->isTrackerMuon() &&
	 muon::sharedSegments(*jm,*mu)>0 && tempSegIndex==-1)
	{
	  tempSegIndex =muonIndex;
	  nSegments =muon::sharedSegments(*jm,*mu);
	}
    } // loop over jmuon
    if(mu->pt() < 5.) continue;
    nMu_++;
    patMuonType_.push_back(mu->type());
    patMuonPt_.push_back(mu->pt());
    patMuonEta_.push_back(mu->eta());
    patMuonPhi_.push_back(mu->phi());
    patMuonM_.push_back(mu->mass());
    patMuonCharge_.push_back(mu->charge());
    

    //id global tracker and PF
    isGlobalMuon_.push_back(mu->isGlobalMuon());
    isTrackerMuon_.push_back(mu->isTrackerMuon());
    isPFMuon_.push_back(mu->isPFMuon());
    
    patMuonITrkIndex_.push_back(tempTrackIndex);
    patMuonSegIndex_.push_back(tempSegIndex);
    patMuonNSeg_.push_back(nSegments);
    patMuonGood_.push_back(muon::isGoodMuon(*mu, muon::TrackerMuonArbitrated)? 1:0);
    patMuonIsGood_.push_back((int) mu->isGood("TMOneStationTight"));
    
    patMuonTrkPt_.push_back(mu->muonBestTrack()->pt());  
    patMuonTrkPtErr_.push_back(mu->muonBestTrack()->ptError());
    patMuondxy_.push_back(mu->muonBestTrack()->dxy(pv));
    patMuondz_.push_back(mu->muonBestTrack()->dz(pv));
     
    patMuonsegmentCompatibility_.push_back(mu->segmentCompatibility());
    patMuonchi2LocalPosition_.push_back(mu->combinedQuality().chi2LocalPosition);
    patMuontrkKink_.push_back(mu->combinedQuality().trkKink);
    
    const reco::TrackRef glbmu = mu->globalTrack();
    const reco::TrackRef trkmu = mu->innerTrack();
    
    if(trkmu.isNull()){
      patMuonInnerdxy_.push_back(-999);
      patMuonInnerdz_.push_back(-999);
      patMuonTrkLayers_.push_back(-999);
      patMuonPixelLayers_.push_back(-999);
      patMuonPixelHits_.push_back(-999);
      patMuonTrkQuality_.push_back(-999);
      patMuonInnervalidFraction_.push_back(-999);
    }else{ 
      patMuonInnerdxy_.push_back(trkmu->dxy(pv));
      patMuonInnerdz_.push_back(trkmu->dz(pv));
      patMuonTrkLayers_.push_back(trkmu->hitPattern().trackerLayersWithMeasurement());
      patMuonPixelLayers_.push_back(trkmu->hitPattern().pixelLayersWithMeasurement());
      patMuonPixelHits_.push_back(trkmu->hitPattern().numberOfValidPixelHits());
      patMuonTrkQuality_.push_back(trkmu->quality(reco::TrackBase::highPurity));
      patMuonInnervalidFraction_.push_back(trkmu->validFraction());
    }
    
    if(glbmu.isNull()){
      patMuonHits_.push_back(-999);
      patMuonChi2NDF_.push_back(-999);
    }else{
      patMuonHits_.push_back(glbmu->hitPattern().numberOfValidMuonHits());
      patMuonChi2NDF_.push_back(glbmu->normalizedChi2());
    } 
    
    patMuonMatches_.push_back(mu->numberOfMatchedStations());
    
 //isolation variables //
    
    patMuonTrkIso_.push_back(mu->trackIso());
    patMuonHcalIso_.push_back(mu->hcalIso());
    patMuonEcalIso_.push_back(mu->ecalIso());
    Float_t iso1   = -999.;
    Float_t iso2   = -999.;
    Float_t iso3   = -999.;
    Float_t isoPU  = -999.;
    iso1 = mu->pfIsolationR04().sumChargedHadronPt;
    iso2 = mu->pfIsolationR04().sumNeutralHadronEt;
    iso3 = mu->pfIsolationR04().sumPhotonEt;
    isoPU = mu->pfIsolationR04().sumPUPt;
    patMuonChHadIso_.push_back(iso1);
    patMuonNeHadIso_.push_back(iso2);
    patMuonGamIso_.push_back(iso3);
    patMuonPUPt_.push_back(isoPU); 
    
  }
}
void
patMuonTree::SetBranches(){
 AddBranch(&nMu_,"nMu");
 AddBranch(&patMuonType_, "muType");
 AddBranch(&patMuonPt_, "muPt");
 AddBranch(&patMuonEta_, "muEta");
 AddBranch(&patMuonPhi_, "muPhi");
 AddBranch(&patMuonM_, "muM");
 AddBranch(&patMuonCharge_, "muCharge");
 AddBranch(&isGlobalMuon_, "isGlobalMuon");
 AddBranch(&isTrackerMuon_, "isTrackerMuon");
 AddBranch(&isPFMuon_,"isPFMuon");

 AddBranch(&patMuonTrkPt_,"muTrkPt");
 AddBranch(&patMuonTrkPtErr_, "muTrkPtErr");
 AddBranch(&patMuondxy_, "mudxy");
 AddBranch(&patMuondz_, "mudz");
 AddBranch(&patMuonsegmentCompatibility_, "musegmentCompatibility");
 AddBranch(&patMuonchi2LocalPosition_, "muchi2LocalPosition"); 
 AddBranch(&patMuontrkKink_, "mutrkKink");
 
 AddBranch(&patMuonInnerdxy_, "muInnerdxy_"); 
 AddBranch(&patMuonInnerdz_, "muInnerdz_");
 AddBranch(&patMuonTrkLayers_, "muTrkLayers");
 AddBranch(&patMuonPixelLayers_,"muPixelLayers");
 AddBranch(&patMuonPixelHits_, "muPixelHits");
 AddBranch(&patMuonTrkQuality_,"muTrkQuality_");
 AddBranch(&patMuonHits_, "muHits");
 AddBranch(&patMuonChi2NDF_,"muChi2NDF");
 AddBranch(&patMuonInnervalidFraction_,"MuInnervalidFraction_");
 AddBranch(&patMuonMatches_, "muMatches");

 AddBranch(&patMuonITrkIndex_, "muITrkID");
 AddBranch(&patMuonSegIndex_, "muSegID");
 AddBranch(&patMuonNSeg_, "muNSegs");
 AddBranch(&patMuonGood_, "muGood");
 AddBranch(&patMuonIsGood_,"patMuonIsGood");

 AddBranch(&patMuonTrkIso_, "muTrkIso");
 AddBranch(&patMuonHcalIso_, "muHcalIso");
 AddBranch(&patMuonEcalIso_, "muEcalIso");
 AddBranch(&patMuonChHadIso_, "muChHadIso");
 AddBranch(&patMuonNeHadIso_, "muNeHadIso");
 AddBranch(&patMuonGamIso_, "muGamIso");
 AddBranch(&patMuonPUPt_, "muPUPt");
}
void
patMuonTree::Clear(){
  nMu_ =0;
  patMuonType_.clear();
  patMuonPt_.clear();
  patMuonEta_.clear();
  patMuonPhi_.clear();
  patMuonM_.clear();
  patMuonCharge_.clear();
  
  patMuonTrkIso_.clear();
  patMuonHcalIso_.clear();
  patMuonEcalIso_.clear();
  patMuonChHadIso_.clear();
  patMuonNeHadIso_.clear();
  patMuonGamIso_.clear();
  patMuonPUPt_.clear();
  
  isGlobalMuon_.clear();
  isTrackerMuon_.clear();
  isPFMuon_.clear();
  
  patMuonTrkPt_.clear();
  patMuonTrkPtErr_.clear();
  patMuondxy_.clear();
  patMuondz_.clear();
  patMuonsegmentCompatibility_.clear();
  patMuonchi2LocalPosition_.clear();
  patMuontrkKink_.clear();
  
  patMuonInnerdxy_.clear();
  patMuonInnerdz_.clear();
  patMuonTrkLayers_.clear();
  patMuonPixelLayers_.clear();
  patMuonPixelHits_.clear();
  patMuonTrkQuality_.clear();
  patMuonHits_.clear();
  patMuonChi2NDF_.clear();
  patMuonInnervalidFraction_.clear();
  patMuonMatches_.clear();
  patMuonITrkIndex_.clear();
  patMuonSegIndex_.clear();
  patMuonNSeg_.clear();
  patMuonGood_.clear();
  patMuonIsGood_.clear();
}
