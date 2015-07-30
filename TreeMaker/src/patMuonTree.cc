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



patMuonTree::patMuonTree(std::string name, TTree* tree, 
			 const edm::ParameterSet& iConfig):
  baseTree(name,tree),  
  pvSrc_    (iConfig.getParameter<edm::InputTag>("pvSrc") ),   
  patMuonLabel_(iConfig.getParameter<edm::InputTag>("patMuons")),
  pfCandLabel_(iConfig.getParameter<edm::InputTag>("pfForMiniIso")),
  r_iso_min_(iConfig.getParameter<double>("r_iso_min")),
  r_iso_max_(iConfig.getParameter<double>("r_iso_max")),
  kt_scale_(iConfig.getParameter<double>("kt_scale")),
  charged_only_(iConfig.getParameter<bool>("charged_only"))
{
  patMuonP4 =   new TClonesArray("TLorentzVector");
  SetBranches();

}

patMuonTree::~patMuonTree()
{
  delete patMuonP4;
}

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
  iEvent.getByLabel(pvSrc_, vtxHandle);
  reco::VertexCollection FilteredVertexCollection;
  FilteredVertexCollection.clear();
  
  //best-known primary vertex coordinates
  math::XYZPoint pv(0, 0, 0);
  for (vector<reco::Vertex>::const_iterator v = vtxHandle->begin(); v != vtxHandle->end(); ++v) {
    bool isFake = isAOD ? v->isFake() : (v->chi2() == 0 && v->ndof() == 0);
    if (!isFake && v->ndof() > 4 && TMath::Abs(v->z())<24. && v->position().rho() <2. ) {
      FilteredVertexCollection.push_back(*v); // save non-fake vertex in new vertex coll
    }
  }

  reco::Vertex vtx;
  if(FilteredVertexCollection.size()>0) vtx = FilteredVertexCollection[0];
  pv.SetXYZ(vtx.x(), vtx.y(), vtx.z());

  const reco::Vertex& vertex = FilteredVertexCollection[0];
  // handle pfcandidates
  Handle<pat::PackedCandidateCollection> pfcands;
  iEvent.getByLabel(pfCandLabel_, pfcands);  
  
  pat::MuonCollection::const_iterator mu;
  
  for(mu=muColl.begin(); mu!=muColl.end(); mu++){

    if(mu->pt() < 10.) continue;
    if(TMath::Abs(mu->eta()) > 2.5) continue;
    nMu++;
    patMuonType.push_back(mu->type());
    patMuonCharge.push_back(mu->charge());

    new( (*patMuonP4)[nMu-1]) TLorentzVector(
					       mu->p4().px(),
					       mu->p4().py(),
					       mu->p4().pz(),
					       mu->p4().energy()
					       );

    //id global tracker and PF
    isGlobalMuon.push_back(mu->isGlobalMuon());
    isTrackerMuon.push_back(mu->isTrackerMuon());
    isPFMuon.push_back(mu->isPFMuon());
    isTightMuon.push_back(mu->isTightMuon(vertex));
    isLooseMuon.push_back(mu->isLooseMuon());
    isMediumMuon.push_back(mu->isMediumMuon());
    isSoftMuon.push_back(mu->isSoftMuon(vertex));
    isHighPtMuon.push_back(mu->isHighPtMuon(vertex));
    
    isCustomTrackerMuon.push_back(CustisTrackerMuon(&(*mu),vertex));
    
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
    
    patMuonITrkIndex.push_back(tempTrackIndex);
    patMuonSegIndex.push_back(tempSegIndex);
    patMuonNSeg.push_back(nSegments);
    patMuonGood.push_back(muon::isGoodMuon(*mu, muon::TrackerMuonArbitrated)? 1:0);
    patMuonIsGood.push_back(mu->isGood("TMOneStationTight"));
    
    patMuonTrkPt.push_back(mu->muonBestTrack()->pt());  
    patMuonTrkPtErr.push_back(mu->muonBestTrack()->ptError());
    patMuondxy.push_back(mu->muonBestTrack()->dxy(pv));
    patMuondz.push_back(mu->muonBestTrack()->dz(pv));
     
    patMuonsegmentCompatibility.push_back(mu->segmentCompatibility());
    patMuonchi2LocalPosition.push_back(mu->combinedQuality().chi2LocalPosition);
    patMuontrkKink.push_back(mu->combinedQuality().trkKink);
    
    const reco::TrackRef glbmu = mu->globalTrack();
    const reco::TrackRef trkmu = mu->innerTrack();
    
    if(trkmu.isNull()){
      patMuonInnerdxy.push_back(-999);
      patMuonInnerdz.push_back(-999);
      patMuonTrkLayers.push_back(-999);
      patMuonPixelLayers.push_back(-999);
      patMuonPixelHits.push_back(-999);
      patMuonTrkQuality.push_back(-999);
      patMuonInnervalidFraction.push_back(-999);
    }else{ 
      patMuonInnerdxy.push_back(trkmu->dxy(pv));
      patMuonInnerdz.push_back(trkmu->dz(pv));
      patMuonTrkLayers.push_back(trkmu->hitPattern().trackerLayersWithMeasurement());
      patMuonPixelLayers.push_back(trkmu->hitPattern().pixelLayersWithMeasurement());
      patMuonPixelHits.push_back(trkmu->hitPattern().numberOfValidPixelHits());
      patMuonTrkQuality.push_back(trkmu->quality(reco::TrackBase::highPurity));
      patMuonInnervalidFraction.push_back(trkmu->validFraction());
    }
    
    if(glbmu.isNull()){
      patMuonHits.push_back(-999);
      patMuonChi2NDF.push_back(-999);
    }else{
      patMuonHits.push_back(glbmu->hitPattern().numberOfValidMuonHits());
      patMuonChi2NDF.push_back(glbmu->normalizedChi2());
    } 
    
    patMuonMatches.push_back(mu->numberOfMatchedStations());
    
    //isolation variables //
    
    patMuonTrkIso.push_back(mu->trackIso());
    patMuonHcalIso.push_back(mu->hcalIso());
    patMuonEcalIso.push_back(mu->ecalIso());
    float iso1   = -999.;
    float iso2   = -999.;
    float iso3   = -999.;
    float isoPU  = -999.;
    iso1 = mu->pfIsolationR04().sumChargedHadronPt;
    iso2 = mu->pfIsolationR04().sumNeutralHadronEt;
    iso3 = mu->pfIsolationR04().sumPhotonEt;
    isoPU = mu->pfIsolationR04().sumPUPt;
    patMuonChHadIso.push_back(iso1);
    patMuonNeHadIso.push_back(iso2);
    patMuonGamIso.push_back(iso3);
    patMuonPUPt.push_back(isoPU); 
   
    double miniIso = getPFIsolation(pfcands, dynamic_cast<const reco::Candidate *>(&(*mu)), r_iso_min_, r_iso_max_, kt_scale_, charged_only_);
    patMuonMiniIso.push_back(miniIso);

  }
}
void
patMuonTree::SetBranches(){

  AddBranch(&nMu,"nMu");
  AddBranch(&patMuonP4,"muP4");
  AddBranch(&patMuonType, "muType");
  AddBranch(&patMuonCharge, "muCharge");

  AddBranch(&isGlobalMuon, "isGlobalMuon");
  AddBranch(&isTrackerMuon, "isTrackerMuon");
  AddBranch(&isPFMuon,"isPFMuon");
  AddBranch(&isTightMuon,"isTightMuon");
  AddBranch(&isLooseMuon,"isLooseMuon");
  AddBranch(&isMediumMuon,"isMediumMuon");
  AddBranch(&isSoftMuon,"isSoftMuon");
  AddBranch(&isHighPtMuon,"isHighPtMuon");
  AddBranch(&isCustomTrackerMuon,"isCustomTrackerMuon");
  
  
  AddBranch(&patMuonITrkIndex, "muITrkID");
  AddBranch(&patMuonSegIndex, "muSegID");
  AddBranch(&patMuonNSeg, "muNSegs");
  AddBranch(&patMuonGood, "muGood");
  AddBranch(&patMuonIsGood,"muIsGood");

  AddBranch(&patMuonTrkPt,"muTrkPt");
  AddBranch(&patMuonTrkPtErr, "muTrkPtErr");
  AddBranch(&patMuondxy, "mudxy");
  AddBranch(&patMuondz, "mudz");
  AddBranch(&patMuonsegmentCompatibility, "musegmentCompatibility");
  AddBranch(&patMuonchi2LocalPosition, "muchi2LocalPosition"); 
  AddBranch(&patMuontrkKink, "mutrkKink");
 
  AddBranch(&patMuonInnerdxy, "muInnerdxy"); 
  AddBranch(&patMuonInnerdz, "muInnerdz");
  AddBranch(&patMuonTrkLayers, "muTrkLayers");
  AddBranch(&patMuonPixelLayers,"muPixelLayers");
  AddBranch(&patMuonPixelHits, "muPixelHits");
  AddBranch(&patMuonTrkQuality,"muTrkQuality");
  AddBranch(&patMuonHits, "muHits");
  AddBranch(&patMuonChi2NDF,"muChi2NDF");
  AddBranch(&patMuonInnervalidFraction,"muInnervalidFraction");
  AddBranch(&patMuonMatches, "muMatches");

  AddBranch(&patMuonTrkIso, "muTrkIso");
  AddBranch(&patMuonHcalIso, "muHcalIso");
  AddBranch(&patMuonEcalIso, "muEcalIso");
  AddBranch(&patMuonChHadIso, "muChHadIso");
  AddBranch(&patMuonNeHadIso, "muNeHadIso");
  AddBranch(&patMuonGamIso, "muGamIso");
  AddBranch(&patMuonPUPt, "muPUPt");
  AddBranch(&patMuonMiniIso, "muMiniIso");




}
void
patMuonTree::Clear(){

  nMu =0;
  patMuonP4->Clear();

  patMuonType.clear();
  patMuonCharge.clear();

  isGlobalMuon.clear();
  isTrackerMuon.clear();
  isPFMuon.clear();
  isTightMuon.clear();
  isLooseMuon.clear();
  isMediumMuon.clear();
  isSoftMuon.clear();
  isHighPtMuon.clear();
  isCustomTrackerMuon.clear();
  patMuonITrkIndex.clear();
  patMuonSegIndex.clear();
  patMuonNSeg.clear();
  patMuonGood.clear();
  patMuonIsGood.clear();


  patMuonTrkPt.clear();
  patMuonTrkPtErr.clear();
  patMuondxy.clear();
  patMuondz.clear();
  patMuonsegmentCompatibility.clear();
  patMuonchi2LocalPosition.clear();
  patMuontrkKink.clear();
 
  patMuonInnerdxy.clear();
  patMuonInnerdz.clear();
  patMuonTrkLayers.clear();
  patMuonPixelLayers.clear();
  patMuonPixelHits.clear();
  patMuonTrkQuality.clear();
  patMuonHits.clear();
  patMuonChi2NDF.clear();
  patMuonInnervalidFraction.clear();
  patMuonMatches.clear();


  patMuonTrkIso.clear();
  patMuonHcalIso.clear();
  patMuonEcalIso.clear();
  patMuonChHadIso.clear();
  patMuonNeHadIso.clear();
  patMuonGamIso.clear();
  patMuonPUPt.clear();
  patMuonMiniIso.clear();


}
