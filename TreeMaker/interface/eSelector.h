#ifndef __ESELECTOR_H__
#define __ESELECTOR_H__

/*
  ElectronSelector
  Optimal Usage: pat Electrons.

  Anil Singh,
  Panjab University.
  Shin-Shan Eiko Yu,
  National Central University
*/



#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h"
#include "TMath.h"

class eSelector{
   
 public:
  eSelector(const edm::ParameterSet ps);  
  std::map<std::string, bool> CutRecord(const pat::Electron& e);
  
  void SetRho(double rho){rho_ = rho;}
  void SetVertex(reco::Vertex pv){pv_ = pv;}
  void SetData(bool isData){isData_ = isData;}

  // remove the contribution of nearby-tau
  double GetTauCorrElecPfIso(const pat::Electron & e)
  {
    double iso1 = e.userIsolation(pat::PfChargedHadronIso);
    double iso2 = e.userIsolation(pat::PfNeutralHadronIso);
    double iso3 = e.userIsolation(pat::PfGammaIso);
    double iso4 = iso1 + iso2 + iso3;
    return iso4;
  }

  // remove the contribution of pileups using the official recipe
  double GetCorrElecPfIso(const pat::Electron &e)
  {
    double iso1 = 999.;
    double iso2 = 999.;
    double iso3 = 999.;
    double iso4 = 999.;

    iso1  =  e.chargedHadronIso();                                                                                                                                     
    iso2  =  e.neutralHadronIso();                                                                                                                                     
    iso3  =  e.photonIso();                                                                                                                                            

    ElectronEffectiveArea::ElectronEffectiveAreaTarget effAreaTarget_ = 
      ElectronEffectiveArea::kEleEAData2012;
      
    ElectronEffectiveArea::ElectronEffectiveAreaType effAreaType_ =
      ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03;      
    
    double eta = e.superCluster()->eta();
    double AEff = ElectronEffectiveArea::GetElectronEffectiveArea
      (effAreaType_, fabs(eta), effAreaTarget_);
    iso4 = iso1 + TMath::Max((double)0.0, iso2+iso3-rho_*AEff);
    
    //Calculate combined and beta corrected Iso                                                                                                   
    
    return iso4;
  }  
  ~eSelector(){}
  
  double ptX_;
  double etaX_;

  double detainBrlX_;
  double delphiBrlX_;  
  double sieieBrlX_;
  double hoeBrlX_;
  double d0vtxBrlX_;
  double dzvtxBrlX_;
  double ooemoopBrlX_;
  double passConvBrlX_;
  double nmisHitBrlX_;

  double detainEcpX_;
  double delphiEcpX_;  
  double sieieEcpX_;
  double hoeEcpX_;
  double d0vtxEcpX_;
  double dzvtxEcpX_;
  double ooemoopEcpX_;
  double passConvEcpX_;
  double nmisHitEcpX_;

  double isoRelBrlX_;
  double isoRelEcpX_;
  
  
  //some pset declaration
  edm::ParameterSet idBrl_;
  edm::ParameterSet idEcp_;

  double rho_;
  double isData_;
  reco::Vertex pv_;

};

#endif
