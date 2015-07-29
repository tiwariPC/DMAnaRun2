// -*- C++ -*-
//
// Package:    TreeMaker
// Class:      TreeMaker
// Original Author:  Anil Singh, Ashok Kumar
//                   Panjab University, Delhi University
//         Created:  Tue Jul  6 21:04:59 CEST 2010
//


// system include files
#include <memory>
#include <string>

#include "DelPanj/TreeMaker/interface/TreeMaker.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
TreeMaker::TreeMaker(const edm::ParameterSet& iConfig)

{
  fillPUweightInfo_=false;
  fillEventInfo_=false;
  fillGenInfo_=false;
  fillTrigInfo_=false;
  fillFilterInfo_=false;
  fillElecInfo_=false;
  fillMuonInfo_=false;
  fillTauInfo_=false;
  fillPhotInfo_=false; 
  fillMetInfo_=false;
  fillJetInfo_=false;
  fillFATJetInfo_=false;
  fillAddJetInfo_=false;


  fillPUweightInfo_ = iConfig.getParameter<bool>("fillPUweightInfo_");
  fillEventInfo_ = iConfig.getParameter<bool>("fillEventInfo_");
  fillGenInfo_   = iConfig.getParameter<bool>("fillGenInfo_");
  fillTrigInfo_  = iConfig.getParameter<bool>("fillTrigInfo_");
  fillFilterInfo_ = iConfig.getParameter<bool>("fillFilterInfo_");
  fillElecInfo_  = iConfig.getParameter<bool>("fillElecInfo_");
  fillMuonInfo_  = iConfig.getParameter<bool>("fillMuonInfo_");
  fillTauInfo_   = iConfig.getParameter<bool>("fillTauInfo_");
  fillPhotInfo_  = iConfig.getParameter<bool>("fillPhotInfo_");
  fillMetInfo_   = iConfig.getParameter<bool>("fillMetInfo_");
  fillJetInfo_   = iConfig.getParameter<bool>("fillJetInfo_");
  fillFATJetInfo_= iConfig.getParameter<bool>("fillFATJetInfo_"); 
  fillAddJetInfo_=iConfig.getParameter<bool>("fillAddJetInfo_");
   
  
  edm::Service<TFileService> fs;


  tree_ = fs->make<TTree>("treeMaker","tree");
  if( fillPUweightInfo_) puweight_=new puweight("pu_",tree_);
  if( fillEventInfo_ )   eventInfo_=new eventInfo("",tree_); 

  if( fillGenInfo_ )     genInfoTree_ = new genInfoTree("",tree_,iConfig);

  if( fillTrigInfo_ )    patHltTree_ = new patHltTree("hlt_",tree_,iConfig); 
  if( fillFilterInfo_ )  patFilterTree_ = new patFilters("hlt_",tree_,iConfig); 
  
  if( fillElecInfo_ )    patElecTree_ = new patElecTree("",tree_,iConfig);
  if( fillMuonInfo_ )    patMuTree_= new patMuonTree("",tree_,iConfig);
  if( fillTauInfo_ )     tauTree_ = new hpstauInfo("",tree_, false, iConfig);
  if( fillPhotInfo_)     photonTree_ = new photonTree("", tree_, iConfig); 

  if( fillMetInfo_ )     patMetTree_= new patMetTree("pf",tree_,iConfig);

  if( fillJetInfo_ )     THINjetTree_=new jetTree("THIN",tree_,iConfig);
  if( fillFATJetInfo_ )  FATjetTree_=new jetTree("FAT",tree_,iConfig);
  if( fillAddJetInfo_)   ADDjetTree_=new jetTree("ADD",tree_,iConfig); 
 
  
  
}


TreeMaker::~TreeMaker()
{
  //delete tree_;
}

void
TreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  using namespace edm;

  if( fillPUweightInfo_) puweight_  ->Fill(iEvent);
  if( fillEventInfo_) eventInfo_   ->Fill(iEvent);
  if( fillGenInfo_)   genInfoTree_ ->Fill(iEvent);
  if( fillElecInfo_)  patElecTree_ ->Fill(iEvent);
  
  if( fillMuonInfo_)  patMuTree_   ->Fill(iEvent);
  if( fillFATJetInfo_)   FATjetTree_  ->Fill(iEvent, iSetup);
  if( fillJetInfo_)   THINjetTree_  ->Fill(iEvent, iSetup);
  if( fillAddJetInfo_)   ADDjetTree_  ->Fill(iEvent, iSetup);  
 
  if( fillMetInfo_)   patMetTree_  ->Fill(iEvent);
  if( fillTrigInfo_)  patHltTree_  ->Fill(iEvent);
  if( fillFilterInfo_) patFilterTree_->Fill(iEvent);
  if( fillPhotInfo_)  photonTree_  ->Fill(iEvent);
  if( fillTauInfo_)   tauTree_     ->Fill(iEvent, iSetup);
  tree_->Fill();
}

void
TreeMaker::beginJob(){
}




void
TreeMaker::endJob() {

}

DEFINE_FWK_MODULE(TreeMaker);
