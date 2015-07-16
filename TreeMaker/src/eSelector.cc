#include "DelPanj/TreeMaker/interface/eSelector.h"


eSelector::eSelector(const edm::ParameterSet ps):      
  ptX_  (ps.getParameter<double>("ptx")),
  etaX_ (ps.getParameter<double>("etax")),
  //identification parameters
  idBrl_(ps.getParameter<edm::ParameterSet> ("idBrl")),   
  idEcp_(ps.getParameter<edm::ParameterSet> ("idEcp"))
{     
  detainBrlX_= (idBrl_.getParameter<double>("detain"));
  delphiBrlX_= (idBrl_.getParameter<double>("delphi"));  
  sieieBrlX_ = (idBrl_.getParameter<double>("sieie"));
  hoeBrlX_   = (idBrl_.getParameter<double>("hoe"));
  d0vtxBrlX_ = (idBrl_.getParameter<double>("d0vtx"));
  dzvtxBrlX_ = (idBrl_.getParameter<double>("dzvtx"));
  ooemoopBrlX_= (idBrl_.getParameter<double>("ooemoop"));
  passConvBrlX_= (idBrl_.getParameter<double>("passConv"));
  nmisHitBrlX_= (idBrl_.getParameter<double>("nmisHit"));
  isoRelBrlX_ = (idBrl_.getParameter<double>("isoRel"));


  detainEcpX_= (idEcp_.getParameter<double>("detain"));
  delphiEcpX_= (idEcp_.getParameter<double>("delphi"));  
  sieieEcpX_ = (idEcp_.getParameter<double>("sieie"));
  hoeEcpX_   = (idEcp_.getParameter<double>("hoe"));
  d0vtxEcpX_ = (idEcp_.getParameter<double>("d0vtx"));
  dzvtxEcpX_ = (idEcp_.getParameter<double>("dzvtx"));
  ooemoopEcpX_= (idEcp_.getParameter<double>("ooemoop"));
  passConvEcpX_= (idEcp_.getParameter<double>("passConv"));
  nmisHitEcpX_= (idEcp_.getParameter<double>("nmisHit"));
  isoRelEcpX_ = (idEcp_.getParameter<double>("isoRel"));
  
  
  isData_ = true;
  rho_    = 0;

}


std::map<std::string, bool> 
eSelector::CutRecord(const pat::Electron& e){

  std::map<std::string, bool> cuts;

       
  //good idea to call the eta and pt methods for once.
  double pt = e.pt();
  double eta = e.superCluster()->eta();
  bool ingap= fabs(eta)>1.4442 && fabs(eta)< 1.566;


  if(ingap)
    {
      cuts["ptx"]    = 0;
      cuts["etax"]   = 0;
      cuts["detain"] = 0;
      cuts["delphi"] = 0;
      cuts["sieie"]  = 0;
      cuts["hoe"]    = 0;
      cuts["d0vtx"]  = 0;
      cuts["dzvtx"]  = 0;
      cuts["ooemoop"] = 0;
      cuts["passConv"] = 0;
      cuts["nmshits"] = 0;
      cuts["isoRel"]   = 0;
      return cuts;
    }
  
  
 
  double iso4 = 999.;
  iso4 = GetCorrElecPfIso(e)/TMath::Max((double)0.1,pt);
 



  cuts["ptx"]    = pt > ptX_;
  cuts["etax"]   = !(ingap)&&fabs(eta) < etaX_;

  double ooemoop = fabs(
			  1.0/TMath::Max((double)1e-3,(double)e.ecalEnergy()) -
			  1.0/TMath::Max((double)1e-3,(double)sqrt(e.trackMomentumAtVtx().mag2()))
			  );

  if(e.isEB()){

    cuts["detain"]  = fabs(e.deltaEtaSuperClusterTrackAtVtx()) < detainBrlX_;
    cuts["delphi"]  = fabs(e.deltaPhiSuperClusterTrackAtVtx()) < delphiBrlX_;
    cuts["sieie"]   = e.sigmaIetaIeta() < sieieBrlX_;
    cuts["hoe"]     = e.hadronicOverEm() < hoeBrlX_;

    cuts["d0vtx"]   = fabs(e.gsfTrack().get()->dxy(pv_.position())) < d0vtxBrlX_;
    cuts["dzvtx"]   = fabs(e.gsfTrack().get()->dz(pv_.position())) < dzvtxBrlX_;
    cuts["ooemoop"] = ooemoop < ooemoopBrlX_;
    
    cuts["passConv"] = passConvBrlX_>1e-6 ? e.passConversionVeto() : 1;
    
    // cuts["nmshits"] = e.gsfTrack().get()->trackerExpectedHitsInner().numberOfHits() <= nmisHitBrlX_;
	 
    cuts["isoRel"]   = iso4 < isoRelBrlX_ ;
    
   }
       
  else if(e.isEE()){

     cuts["detain"] = fabs(e.deltaEtaSuperClusterTrackAtVtx()) < detainEcpX_;
     cuts["delphi"] = fabs(e.deltaPhiSuperClusterTrackAtVtx()) < delphiEcpX_;
     cuts["sieie"]  = e.sigmaIetaIeta() < sieieEcpX_;
     cuts["hoe"]    = e.hadronicOverEm() < hoeEcpX_;

     cuts["d0vtx"]  = fabs(e.gsfTrack().get()->dxy(pv_.position())) < d0vtxEcpX_;
     cuts["dzvtx"]  = fabs(e.gsfTrack().get()->dz(pv_.position())) < dzvtxEcpX_;

     cuts["ooemoop"] = ooemoop < ooemoopEcpX_;
     cuts["passConv"] = passConvEcpX_>1e-6? e.passConversionVeto() : 1;
     // cuts["nmshits"] = e.gsfTrack().get()->trackerExpectedHitsInner().numberOfHits()<=nmisHitEcpX_;

     cuts["isoRel"]  = iso4 < isoRelEcpX_;
    }
  else
    {
      cuts["ptx"]    = 1;
      cuts["etax"]   = 0;
      cuts["detain"] = 0;
      cuts["delphi"] = 0;
      cuts["sieie"]  = 0;
      cuts["hoe"]    = 0;
      cuts["d0vtx"]  = 0;
      cuts["dzvtx"]  = 0;
      cuts["ooemoop"] = 0;
      cuts["passConv"] = 0;
      cuts["nmshits"] = 0;
      cuts["isoRel"]   = 0;
    }

     
  return cuts;
}
