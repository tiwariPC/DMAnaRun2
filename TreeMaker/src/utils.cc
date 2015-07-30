#include "DelPanj/TreeMaker/interface/utils.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <bitset>
//#include "TLorentzVector.h"
//#include <vector>
//#include <string>
//#include <map>

/*
class PtGreater {
  public:
  template <typename T> bool operator () (const T& i, const T& j) {
    return (i.pt() > j.pt());
  }
};
*/
TLorentzVector Part2LorVec(reco::Candidate& cand){
  TLorentzVector* l = new TLorentzVector();
  l->SetPtEtaPhiM(cand.pt(),cand.eta(),cand.phi(),cand.mass());
  return (*l);
}

//When Selectors are fully developed,this must move to baseSelector class.
bool PassAll(std::map<std::string, bool> cutrecd){
  std::map<std::string, bool>::iterator iter= cutrecd.begin();
  bool decision =true ;
  for(;iter!=cutrecd.end();iter++){
    //    std::cout<<"-->"<<iter->first<<"\t"<<iter->second<<std::endl;	   
    decision = decision&&iter->second;     
  }
  return decision;
}


bool PassAllBut(std::string tag, std::map<std::string, bool> cutrecd){
  std::map<std::string, bool>::iterator iter= cutrecd.begin();
  bool decision =true ;
  for(;iter!=cutrecd.end();iter++){
    if(iter->first==tag)continue;
    decision = decision&&iter->second;
  }
 return decision;  
}



 double getPFIsolation(edm::Handle<pat::PackedCandidateCollection> pfcands,
                        const reco::Candidate* ptcl,  
                        double r_iso_min, double r_iso_max, double kt_scale,
                        bool charged_only) {

    if (ptcl->pt()<5.) return 99999.;

    double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
    if(ptcl->isElectron()) {
      if (fabs(ptcl->eta())>1.479) {deadcone_ch = 0.015; deadcone_pu = 0.015; deadcone_ph = 0.08;}
    } else if(ptcl->isMuon()) {
      deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;  
    } else {
      //deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01; // maybe use muon cones??
    }

    double iso_nh(0.); double iso_ch(0.); 
    double iso_ph(0.); double iso_pu(0.);
    double ptThresh(0.5);
    if(ptcl->isElectron()) ptThresh = 0;
    double r_iso = TMath::Max(r_iso_min,TMath::Min(r_iso_max, kt_scale/ptcl->pt()));
    for (const pat::PackedCandidate &pfc : *pfcands) {
      if (abs(pfc.pdgId())<7) continue;

      double dr = deltaR(pfc, *ptcl);
      if (dr > r_iso) continue;
      
      //////////////////  NEUTRALS  /////////////////////////
      if (pfc.charge()==0){
        if (pfc.pt()>ptThresh) {
          /////////// PHOTONS ////////////
          if (abs(pfc.pdgId())==22) {
            if(dr < deadcone_ph) continue;
            iso_ph += pfc.pt();
	    /////////// NEUTRAL HADRONS ////////////
          } else if (abs(pfc.pdgId())==130) {
            if(dr < deadcone_nh) continue;
            iso_nh += pfc.pt();
          }
        }
        //////////////////  CHARGED from PV  /////////////////////////
      } else if (pfc.fromPV()>1){
        if (abs(pfc.pdgId())==211) {
          if(dr < deadcone_ch) continue;
          iso_ch += pfc.pt();
        }
        //////////////////  CHARGED from PU  /////////////////////////
      } else {
        if (pfc.pt()>ptThresh){
          if(dr < deadcone_pu) continue;
          iso_pu += pfc.pt();
        }
      }
    }
    double iso(0.);
    if (charged_only){
      iso = iso_ch;
    } else {
      iso = iso_ph + iso_nh;
      iso -= 0.5*iso_pu;
      if (iso>0) iso += iso_ch;
      else iso = iso_ch;
    }
    iso = iso/ptcl->pt();

    return iso;
  }








/// Auxiliary function to select muons in the context of high-pt muon
/// analysis. Could be extended to other analyses, of course.
bool CustisTrackerMuon (const reco::Muon* recoMu, const reco::Vertex& vertex) {
  
  //bool isGlobal = false;
  bool isTracker = false;
  //bool muonChamberHit = false;
  bool matchedStations = false;
  bool relativeError = false;
  bool dBCut = false;
  bool longiCut = false;
  bool pixelHit = false;
  bool trackerLayers = false;
  
  //isGlobal = recoMu->isGlobalMuon();
  isTracker = recoMu->isTrackerMuon();
  
  matchedStations = (recoMu->numberOfMatchedStations() > 1);
	
  const reco::TrackRef& bestTrackRef = recoMu->muonBestTrack();
  const reco::TrackRef& innerTrackRef = recoMu->innerTrack();
  const reco::TrackRef& trackRef = recoMu->track();
  
  relativeError = (bestTrackRef->ptError()/bestTrackRef->pt()) < 0.3;
  dBCut         = (fabs(bestTrackRef->dxy(vertex.position())) < 0.2);  
  longiCut      = (fabs(bestTrackRef->dz(vertex.position())) < 0.5);
  
  if(innerTrackRef.isNonnull())
    pixelHit = (innerTrackRef->hitPattern().numberOfValidPixelHits() > 0);
  
  if(trackRef.isNonnull())
    trackerLayers = (trackRef->hitPattern().trackerLayersWithMeasurement() > 5);
  
  bool passed = (isTracker and 
		 matchedStations and 
		 relativeError and 
		       dBCut and 
		 longiCut and 
		       pixelHit and 
		 trackerLayers);
  
  
  return passed;
}
