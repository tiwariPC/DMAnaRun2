/* 
-- added CA15 double b-tagger 
-- added ECFs
-- including many files from the external packages of CMSSW (fastjet) to recluster the jet. 
 */
#include "FWCore/Framework/interface/EDConsumerBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "DelPanj/TreeMaker/interface/jetTree.h"
#include <CLHEP/Vector/LorentzVector.h>
#include "TMath.h"
#include "Math/VectorUtil.h"
#include <algorithm>


// Fastjet 
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/contrib/SoftDrop.hh"
#include "fastjet/contrib/NjettinessPlugin.hh"
#include "fastjet/contrib/MeasureDefinition.hh"
#include "fastjet/contrib/EnergyCorrelator.hh"

const double DUMMY=-99999.;


typedef math::XYZTLorentzVector LorentzVector;
const math::XYZPoint & position(const reco::Vertex & sv) {return sv.position();}
const math::XYZPoint & position(const reco::VertexCompositePtrCandidate & sv) {return sv.vertex();}

jetTree::jetTree(std::string desc, TTree* tree, const edm::ParameterSet& iConfig):
  baseTree(desc, tree),
  isTHINJet_(false),
  useJECText_(iConfig.getParameter<bool>("useJECText")),
  jecUncPayLoadName_(iConfig.getParameter<std::string>(Form("%sjecUncPayLoad",desc.data()))),
  jecNames_(iConfig.getParameter<std::vector<std::string> >(Form("%sjecNames",desc.data()) )), 
  jecUncName_(iConfig.getParameter<std::string>(Form("%sjecUncName",desc.data())) ),	
  jet2012ID_()
{
  
  if (desc.find("THIN")!=std::string::npos)
    isTHINJet_=true;

  std::cout << desc << std::endl;
  
  
  genjetP4_    = new TClonesArray("TLorentzVector");
  jetP4_       = new TClonesArray("TLorentzVector");
  unCorrJetP4_ = new TClonesArray("TLorentzVector");

  SetBranches();


  if(useJECText_)
    {
      
      std::vector<JetCorrectorParameters> vPar;

      for ( std::vector<std::string>::const_iterator payloadBegin = 
	      jecNames_.begin(),
	      payloadEnd = jecNames_.end(), ipayload = payloadBegin; 
	    ipayload != payloadEnd; ++ipayload ) 
	{
	  JetCorrectorParameters pars(*ipayload);
	  vPar.push_back(pars);
	}
      jecText_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );

      jecUncText_ = boost::shared_ptr<JetCorrectionUncertainty>( new JetCorrectionUncertainty(jecUncName_) );
    }

  


}


jetTree::~jetTree(){

  delete genjetP4_;
  delete jetP4_;
  delete unCorrJetP4_;
  
  /* EFC: starts here */
  delete areaDef;
  delete activeArea;
  delete jetDefCA;
  /* EFC: ends here */

  //delete htt;
  //delete mMCJetCorrector;
  //delete mDataJetCorrector;

}


void
jetTree::Fill(const edm::Event& iEvent, edm::EventSetup const& iSetup){
  Clear();
 
  // // Get the rho collection
  edm::Handle< double > h_rho;
  if(not iEvent.getByToken(rhoForJetToken, h_rho ))
    {
      std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
	       <<"fixedGridRhoFastjetAll" <<std::endl; 
      exit(0);
    }

  jetRho_ = *(h_rho.product());

  // // Get the primary vertex collection                                         
  edm::Handle<reco::VertexCollection>  h_pv;
  if(not iEvent.getByToken(vertexToken,h_pv))
    {
      std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
	       <<"vertexToken"<<std::endl; 
      exit(0);
    }

  if (h_pv->empty()) return; // skip the event if no PV found

  jetNPV_=  h_pv->size();


 
  edm::Handle<pat::JetCollection> JetHandle;
  if(not iEvent.getByToken(jetToken,JetHandle))
    {
      std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
	       <<"jetToken"<<std::endl; 
      exit(0);
    }


  // for jet energy uncertainty, using global tag
  JetCorrectionUncertainty *jecUnc_=0;
  // fat jet uncertainty does not exist yet
  if(!useJECText_){
    edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
    iSetup.get<JetCorrectionsRecord>().get(jecUncPayLoadName_.data(),JetCorParColl); 
    JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
    jecUnc_ = new JetCorrectionUncertainty(JetCorPar);
  }

  // now start looping over jets
  pat::JetCollection jets(*(JetHandle.product()));
  std::sort(jets.begin(),jets.end(),PtGreater());
  std::vector<pat::Jet>::const_iterator jet =jets.begin();   


  for(;jet!=jets.end();jet++){

    if(jet->pt() < 10.) continue;
  
    nJet_++;
    //Stuff common for all jets.
    

    if (jet->genJet()){

      double DR =0 ;
      double a = ( jet->p4().eta() - jet->genJet()->p4().eta()) * ( jet->p4().eta() - jet->genJet()->p4().eta());
      double b = ( jet->p4().phi() - jet->genJet()->p4().phi()) * ( jet->p4().phi() - jet->genJet()->p4().phi());
      DR = sqrt(a+b);
    
      new( (*genjetP4_)[nJet_-1]) TLorentzVector(
   						 jet->genJet()->p4().px(),
   						 jet->genJet()->p4().py(),
   						 jet->genJet()->p4().pz(),
   						 jet->genJet()->p4().energy()
   						 );
    
      genjetEM_.push_back(jet->genJet()->emEnergy());
      genjetHAD_.push_back(jet->genJet()->hadEnergy());
      genjetINV_.push_back(jet->genJet()->invisibleEnergy());
      genjetAUX_.push_back(jet->genJet()->auxiliaryEnergy());
      matchedDR_.push_back(DR);
      if(false) std::cout<<" jet E = "<<jet->energy()
   			 <<" genjet E = "<<jet->genJet()->energy()
   			 <<" genjet em = "<<jet->genJet()->emEnergy()
   			 <<" genjet had= "<<jet->genJet()->hadEnergy()
   			 <<" genjet inv= "<<jet->genJet()->invisibleEnergy()
   			 <<" genjet aux= "<<jet->genJet()->auxiliaryEnergy()
   			 <<" recojet pho = "<<jet->photonEnergyFraction()*jet->energy()
   			 <<" recojet ele = "<<jet->chargedEmEnergyFraction()*jet->energy()
   			 <<" dr = "<<DR
   			 <<std::endl;
    }
    else{
      genjetEM_.push_back(DUMMY);
      genjetHAD_.push_back(DUMMY);
      genjetINV_.push_back(DUMMY);
      genjetAUX_.push_back(DUMMY);
      matchedDR_.push_back(DUMMY);
      new( (*genjetP4_)[nJet_-1]) TLorentzVector(DUMMY,DUMMY,DUMMY,DUMMY);
    }
  
    jetRawFactor_.push_back(jet->jecFactor("Uncorrected"));
    reco::Candidate::LorentzVector uncorrJet;
    uncorrJet = jet->correctedP4(0);
    new( (*unCorrJetP4_)[nJet_-1]) TLorentzVector(	
						  uncorrJet.px(),
						  uncorrJet.py(),
						  uncorrJet.pz(),
						  uncorrJet.energy()
   							);
   
    jetArea_.push_back(jet->jetArea());
  
    // if reading text files, set jet 4-momentum
    // make correction using jecText files
    if(useJECText_){
      jecText_->setJetEta( uncorrJet.eta() );
      jecText_->setJetPt ( uncorrJet.pt() );
      jecText_->setJetE  ( uncorrJet.energy() );
      jecText_->setJetA  ( jet->jetArea() );
      jecText_->setRho   ( *(h_rho.product()) );
      jecText_->setNPV   ( h_pv->size() );
      Float_t corr_jet = jecText_->getCorrection();

      new( (*jetP4_)[nJet_-1]) TLorentzVector(uncorrJet.px()*corr_jet,
					      uncorrJet.py()*corr_jet,
					      uncorrJet.pz()*corr_jet,
					      uncorrJet.energy()*corr_jet);
      jecUncText_->setJetEta( uncorrJet.eta() );
      jecUncText_->setJetPt( corr_jet * uncorrJet.pt() );
      jetCorrUncUp_.push_back(jecUncText_->getUncertainty(true));

      jecUncText_->setJetEta( uncorrJet.eta() );
      jecUncText_->setJetPt( corr_jet * uncorrJet.pt() );
      jetCorrUncDown_.push_back(jecUncText_->getUncertainty(false));

    }
    else
      new( (*jetP4_)[nJet_-1]) TLorentzVector(jet->p4().px(),
					      jet->p4().py(),
					      jet->p4().pz(),
					      jet->p4().energy());


    // get jet energy scale uncertainty and related input variables
    // fat jet uncertainty does not exist yet, if using database
    if(!useJECText_){

      jecUnc_->setJetEta(jet->eta());
      jecUnc_->setJetPt(jet->pt()); 
      jetCorrUncUp_.push_back(jecUnc_->getUncertainty(true));

      jecUnc_->setJetEta(jet->eta());
      jecUnc_->setJetPt(jet->pt()); 
      jetCorrUncDown_.push_back(jecUnc_->getUncertainty(false));
    }


    jetCharge_.push_back(jet->charge());
    jetPartonFlavor_.push_back(jet->partonFlavour());
    jetHadronFlavor_.push_back(jet->hadronFlavour());


  
    std::map<std::string, bool> Pass = jet2012ID_.LooseJetCut(*jet);
    bool passOrNot = PassAll(Pass); 
    jetPassIDLoose_.push_back(passOrNot);


    std::map<std::string, bool> PassT = jet2012ID_.TightJetCut(*jet);
    bool passOrNotT = PassAll(PassT); 
    jetPassIDTight_.push_back(passOrNotT);


    if(isTHINJet_){
      float jpumva=0.;
      jpumva= jet->userFloat("pileupJetId:fullDiscriminant");
      //std::cout<<" jpumva = "<<jpumva<<std::endl;
      PUJetID_.push_back(jpumva);
          
      // float jpt = jet->pt();
      // float jeta = jet->eta();

      isPUJetIDLoose_.push_back(
				bool(jet->userInt("pileupJetId:fullId") & (1 << 2)));
      isPUJetIDMedium_.push_back(
				 bool(jet->userInt("pileupJetId:fullId") & (1 << 1)));
      isPUJetIDTight_.push_back(
				bool(jet->userInt("pileupJetId:fullId") & (1 << 0)));
    }
        
    jetCEmEF_.push_back(jet->chargedEmEnergyFraction());
    jetCHadEF_.push_back(jet->chargedHadronEnergyFraction());
    jetPhoEF_.push_back(jet->photonEnergyFraction());
    jetNEmEF_.push_back(jet->neutralEmEnergyFraction());
    jetNHadEF_.push_back(jet->neutralHadronEnergyFraction());

    jetEleEF_.push_back(jet->electronEnergyFraction());
    jetMuoEF_.push_back(jet->muonEnergyFraction());
    jetChMuEF_.push_back(jet->chargedMuEnergyFraction());
      
    jetHFHadEF_.push_back(jet->HFHadronEnergyFraction());
    jetHFEMEF_.push_back(jet->HFEMEnergyFraction());
    jetHOEnergy_.push_back(jet->hoEnergy());
    jetHOEF_.push_back(jet->hoEnergyFraction());
      
    if(false) std::cout<<"jetHFHadEF_ = "<<(jet->HFHadronEnergyFraction())
		       <<"  jetHFEMEF_ = "<<(jet->HFEMEnergyFraction())
		       <<"  jetCHHadMultiplicity_ = "<<(jet->chargedHadronMultiplicity())
		       <<"  jetNHadMulplicity_ = "<<(jet->neutralHadronMultiplicity())
		       <<"  jetPhMultiplicity_ = "<<(jet->photonMultiplicity())
		       <<"  jetEleMultiplicity_ = "<<(jet->electronMultiplicity())
		       <<"  jetHFHadMultiplicity_ = "<<(jet->HFHadronMultiplicity())
		       <<"  jetHFEMMultiplicity_ = "<<(jet->HFEMMultiplicity())
		       <<"  jetChMuEF_ = "<<(jet->chargedMuEnergyFraction())
		       <<"  jetNMultiplicity_ = "<<(jet->neutralMultiplicity())
		       <<"  jetHOEnergy_ = "<<(jet->hoEnergy())
		       <<"  jetHOEF_ = "<<(jet->hoEnergyFraction())
		       <<std::endl;

    jetCMulti_.push_back(jet->chargedMultiplicity());      
    jetEleMultiplicity_.push_back(jet->electronMultiplicity());
    jetMuoMultiplicity_.push_back(jet->muonMultiplicity());

    jetCHHadMultiplicity_.push_back(jet->chargedHadronMultiplicity());
    jetPhMultiplicity_.push_back(jet->photonMultiplicity());
    jetNMultiplicity_.push_back(jet->neutralMultiplicity());
    jetNHadMulplicity_.push_back(jet->neutralHadronMultiplicity());
    jetHFHadMultiplicity_.push_back(jet->HFHadronMultiplicity());
    jetHFEMMultiplicity_.push_back(jet->HFEMMultiplicity());

    


    // b-tagging

    jetSSV_.push_back(jet->bDiscriminator("pfSimpleSecondaryVertexHighPurBJetTags"));
    jetCSV_.push_back(jet->bDiscriminator("combinedSecondaryVertexBJetTags"));
    jetDeepCSV_.push_back(jet->bDiscriminator("deepFlavourJetTags:probb"));
    jetSSVHE_.push_back(jet->bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags"));      
    jetCISVV2_.push_back(jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
    jetTCHP_.push_back(jet->bDiscriminator("pfTrackCountingHighPurBJetTags"));
    jetTCHE_.push_back(jet->bDiscriminator("pfTrackCountingHighEffBJetTags"));
    jetJP_.push_back(jet->bDiscriminator("pfJetProbabilityBJetTags"));
    jetJBP_.push_back(jet->bDiscriminator("pfJetBProbabilityBJetTags"));


  }//jet loop
    

  // fat jet uncertainty does not exist yet
  if(!useJECText_)
    delete jecUnc_;


}



void
jetTree::SetBranches(){
  
  AddBranch(&nJet_,   "nJet");
  AddBranch(&jetP4_,       "jetP4");

  AddBranch(&jetRho_, "jetRho");
  AddBranch(&jetNPV_, "jetNPV");

  AddBranch(&genjetP4_,   "genjetP4");
  AddBranch(&genjetEM_ ,  "genjetEM");
  AddBranch(&genjetHAD_ , "genjetHAD");
  AddBranch(&genjetINV_ , "genjetINV");
  AddBranch(&genjetAUX_ , "genjetAUX");
  AddBranch(&matchedDR_ , "matchedDR");

  AddBranch(&jetRawFactor_, "jetRawFactor");
  AddBranch(&unCorrJetP4_, "unCorrJetP4");

  AddBranch(&jetArea_,        "jetArea");
  AddBranch(&jetCorrUncUp_,   "jetCorrUncUp");
  AddBranch(&jetCorrUncDown_, "jetCorrUncDown");

  AddBranch(&jetCharge_,       "jetCharge");
  AddBranch(&jetPartonFlavor_, "jetPartonFlavor");
  AddBranch(&jetHadronFlavor_, "jetHadronFlavor");
  AddBranch(&jetPassIDLoose_,  "jetPassIDLoose");
  AddBranch(&jetPassIDTight_,  "jetPassIDTight");

  AddBranch(&jetCEmEF_,  "jetCEmEF");
  AddBranch(&jetCHadEF_, "jetCHadEF");
  AddBranch(&jetPhoEF_,  "jetPhoEF");
  AddBranch(&jetNEmEF_,  "jetNEmEF");
  AddBranch(&jetNHadEF_, "jetNHadEF");
  AddBranch(&jetEleEF_,  "jetEleEF");
  AddBranch(&jetMuoEF_,  "jetMuoEF");

  AddBranch(&jetCMulti_, "jetCMulti");
  AddBranch(&jetEleMultiplicity_,"jetEleMulti");
  AddBranch(&jetMuoMultiplicity_,"jetMuoMulti");
  
  AddBranch(&jetSSV_,   "jetSSV");
  AddBranch(&jetCSV_,   "jetCSV");        
  AddBranch(&jetSSVHE_, "jetSSVHE");
  AddBranch(&jetCISVV2_,"jetCISVV2");
  AddBranch(&jetTCHP_,  "jetTCHP");
  AddBranch(&jetTCHE_,  "jetTCHE");
  AddBranch(&jetJP_,    "jetJP");
  AddBranch(&jetJBP_,   "jetJBP");


  if(isTHINJet_){
    AddBranch(&PUJetID_,   "PUJetID");
    AddBranch(&jetDeepCSV_,  "jetDeepCSV");
    AddBranch(&isPUJetIDLoose_,  "isPUJetIDLoose");
    AddBranch(&isPUJetIDMedium_, "isPUJetIDMedium");
    AddBranch(&isPUJetIDTight_,  "isPUJetIDTight");
  }



}


void
jetTree::Clear(){


  nJet_ =0;
  jetRho_=0;
  jetNPV_=0;

  genjetP4_->Clear();
  genjetEM_.clear();
  genjetHAD_.clear();
  genjetINV_.clear();
  genjetAUX_.clear();
  matchedDR_.clear();

  jetRawFactor_.clear();

  jetP4_->Clear();  
  unCorrJetP4_->Clear();

  jetArea_.clear();
  jetCorrUncUp_.clear();
  jetCorrUncDown_.clear();
  jetCharge_.clear();
  jetPartonFlavor_.clear();
  jetHadronFlavor_.clear();
  jetPassIDLoose_.clear();
  jetPassIDTight_.clear();
  PUJetID_.clear();
  isPUJetIDLoose_.clear();
  isPUJetIDMedium_.clear();
  isPUJetIDTight_.clear();

  //Energy Fraction and Multiplicity 

  jetCEmEF_.clear();
  jetCHadEF_.clear();
  jetPhoEF_.clear();
  jetNEmEF_.clear();
  jetNHadEF_.clear();

  jetMuoEF_.clear();
  jetEleEF_.clear();
  jetChMuEF_.clear();

  jetHFHadEF_.clear();
  jetHFEMEF_.clear();
  jetHOEnergy_.clear();
  jetHOEF_.clear();


  jetCMulti_.clear();
  jetEleMultiplicity_.clear();
  jetMuoMultiplicity_.clear();
  jetCHHadMultiplicity_.clear();
  jetPhMultiplicity_.clear();
  jetNMultiplicity_.clear();
  jetNHadMulplicity_.clear();
  jetHFHadMultiplicity_.clear();
  jetHFEMMultiplicity_.clear();


  // btag information
  jetSSV_.clear();
  jetCSV_.clear();
  jetDeepCSV_.clear();
  jetSSVHE_.clear();
  jetCISVV2_.clear();
  jetTCHP_.clear();
  jetTCHE_.clear();
  jetJP_.clear();
  jetJBP_.clear();


}
