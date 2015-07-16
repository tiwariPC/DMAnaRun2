#ifndef __MUSELECTOR_H__
#define __MUSELECTOR_H__

/*
MuonSelector
Optimal Usage: patMuons.

Lovedeep Kaur,
Panjab University.

Shin-Shan Eiko Yu,
National Central University

*/

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
//#include "DataFormats/Math/interface/LorentzVector.h"
// for tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/MuonCocktails.h" 
// for vertexing
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
// for making histograms
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

class muSelector{
   
 public:
  muSelector(const edm::ParameterSet ps);
  std::map<std::string, bool> CutRecord(const pat::Muon& mu);


  void SetVertex(reco::Vertex primaryVertex){pv_ = primaryVertex;}

  // remove the contribution of nearby-tau
  double GetTauCorrMuonPfIso(const pat::Muon& mu)
  {
    double iso1 = mu.userIsolation(pat::PfChargedHadronIso);
    double iso2 = mu.userIsolation(pat::PfNeutralHadronIso);
    double iso3 = mu.userIsolation(pat::PfGammaIso);
    double iso4 = iso1 + iso2 + iso3;
    return iso4;
  }

  // remove the contribution of pileups using the official recipe
  double GetCorrMuonPfIso(const pat::Muon& mu)
  {
    double iso1 = 999.;
    double iso2 = 999.;
    double iso3 = 999.;
    double iso4 = 999.;
    double isoPU = -999.;

    iso1  =  mu.pfIsolationR04().sumChargedHadronPt;
    iso2  =  mu.pfIsolationR04().sumNeutralHadronEt;
    iso3  =  mu.pfIsolationR04().sumPhotonEt;
    isoPU =  mu.pfIsolationR04().sumPUPt;

    //Calculate combined and beta corrected Iso                                                                                                   
    iso4 = iso1 + TMath::Max((double)(iso3+iso2-0.5*isoPU),(double)0.);
    return iso4;
  }

  double GetBestMuonPt(const pat::Muon& mu)
  {
    return highPtID_ && mu.isGlobalMuon()? 
      ((muon::tevOptimized(mu, 200, 17., 40., 0.25)).first)->pt():
      mu.pt();

  }

  reco::TrackRef GetBestTrack(const pat::Muon& mu)
  {
    return highPtID_ && mu.isGlobalMuon()? 
      (muon::tevOptimized(mu, 200, 17., 40., 0.25)).first:
      mu.muonBestTrack();
  }
  
  ~muSelector(){}
  
  double ptX_;
  double ptErrX_;
  double etaX_;
  double phiX_;

  // in ID parameters idPar_
  double dxyX_;
  double dzX_;
  double normalizedChi2X_;
  double trackerHitsX_;
  double pixelHitsX_;
  double muonHitsX_;
  double nMatchesX_;
  double isoX_;
  

  bool reqTrigMatch_;
  bool highPtID_;
  bool trackerMuID_;
  edm::ParameterSet idPar_;



  reco::Vertex pv_;

};

#endif
