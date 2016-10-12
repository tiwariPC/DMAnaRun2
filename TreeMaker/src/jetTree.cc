
// This class need a new version
// Take care of gen Jets from configuation 
// take care of subjets
// Tau variables now Fixed
// take care of tau variables : access using new methods. 
// add additional info related to soft drop (??) 
// remove branches which are not needed. 
// Replace pt, eta, phi, E, px. py, pz by TLorentzVector to check size of the tuple. :: do this for all the classes. 

#include "FWCore/Framework/interface/EDConsumerBase.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "DelPanj/TreeMaker/interface/jetTree.h"
#include <CLHEP/Vector/LorentzVector.h>
#include "TMath.h"
#include "Math/VectorUtil.h"

const double DUMMY=-99999.;


typedef math::XYZTLorentzVector LorentzVector;
const math::XYZPoint & position(const reco::Vertex & sv) {return sv.position();}
const math::XYZPoint & position(const reco::VertexCompositePtrCandidate & sv) {return sv.vertex();}

jetTree::jetTree(std::string desc, TTree* tree, const edm::ParameterSet& iConfig):
  baseTree(desc, tree),
  isTHINJet_(false),
  isFATJet_(false),
  isADDJet_(false),
  isAK4PuppiJet_(false),
  isAK8PuppiJet_(false),
  isCA15PuppiJet_(false),
  useJECText_(iConfig.getParameter<bool>("useJECText")),
  svTagInfosCstr_(iConfig.getParameter<std::string>("svTagInfosPY")),
  jecUncPayLoadName_(iConfig.getParameter<std::string>(Form("%sjecUncPayLoad",desc.data()))),
  jecNames_(iConfig.getParameter<std::vector<std::string> >(Form("%sjecNames",desc.data()) )), 
  jecUncName_(iConfig.getParameter<std::string>(Form("%sjecUncName",desc.data())) ),	
  jet2012ID_()
{
  
  if (desc.find("THIN")!=std::string::npos)
    isTHINJet_=true;
  if (desc.find("FAT")!=std::string::npos)
    isFATJet_=true;
  if (desc.find("ADD")!=std::string::npos)
    isADDJet_=true; 
  if (desc.find("AK4Puppi")!=std::string::npos)
    isAK4PuppiJet_=true; 
  if (desc.find("AK8Puppi")!=std::string::npos)
    isAK8PuppiJet_=true; 
  if (desc.find("CA15Puppi")!=std::string::npos)
    isCA15PuppiJet_=true; 

  std::cout << desc << std::endl;

  genjetP4_    = new TClonesArray("TLorentzVector");
  jetP4_       = new TClonesArray("TLorentzVector");
  unCorrJetP4_ = new TClonesArray("TLorentzVector");
  jetPuppiP4_  = new TClonesArray("TLorentzVector");
  jetPuppiSDRawP4_  = new TClonesArray("TLorentzVector");

  SetBranches();


  if(isFATJet_)
    {
      prunedMassJecNames_          = iConfig.getParameter<std::vector<std::string> >(Form("%sprunedMassJecNames",desc.data()));
      softdropMassJecNames_          = iConfig.getParameter<std::vector<std::string> >(Form("%ssoftdropMassJecNames",desc.data()));

      if(useJECText_){

	std::vector<JetCorrectorParameters> vPar;

	// pruned mass

	for ( std::vector<std::string>::const_iterator payloadBegin = 
		prunedMassJecNames_.begin(),
		payloadEnd = prunedMassJecNames_.end(), ipayload = payloadBegin; 
	      ipayload != payloadEnd; ++ipayload ) 
	  {
	    JetCorrectorParameters pars(*ipayload);
	    vPar.push_back(pars);
	  }
	prunedjecText_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );



	// softdrop mass
	vPar.clear();
	for ( std::vector<std::string>::const_iterator payloadBegin = 
		softdropMassJecNames_.begin(),
		payloadEnd = softdropMassJecNames_.end(), ipayload = payloadBegin; 
	      ipayload != payloadEnd; ++ipayload ) 
	  {
	    JetCorrectorParameters pars(*ipayload);
	    vPar.push_back(pars);
	  }
	softdropjecText_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );


      }

    } // if it's FATjet

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
  delete jetPuppiP4_;
  delete jetPuppiSDRawP4_;
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


  // for getting the L2+L3 correction factor of pruned jet mass
  edm::Handle<pat::JetCollection> JetHandleForPrunedMass;
  pat::JetCollection jetsForPrunedMass;

  if(isFATJet_ && not iEvent.getByToken(prunedMToken,JetHandleForPrunedMass))
    {
      std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
    	       <<"PrunedMassJet"<<std::endl; 
      exit(0);
    }
  else if(isFATJet_ && iEvent.getByToken(prunedMToken,JetHandleForPrunedMass))
    jetsForPrunedMass       = *(JetHandleForPrunedMass.product());


  // for getting the L2+L3 correction factor of softdrop jet mass
  edm::Handle<pat::JetCollection> JetHandleForSoftDropMass;
  pat::JetCollection jetsForSoftDropMass;

  if(isFATJet_ && not iEvent.getByToken(softdropMToken,JetHandleForSoftDropMass))
    {
      std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
    	       <<"SoftDropMassJet"<<std::endl; 
      exit(0);
    }
  else if(isFATJet_ && iEvent.getByToken(softdropMToken,JetHandleForSoftDropMass))
    jetsForSoftDropMass       = *(JetHandleForSoftDropMass.product());



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
    jetSSVHE_.push_back(jet->bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags"));      
    jetCISVV2_.push_back(jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
    jetTCHP_.push_back(jet->bDiscriminator("pfTrackCountingHighPurBJetTags"));
    jetTCHE_.push_back(jet->bDiscriminator("pfTrackCountingHighEffBJetTags"));
    jetJP_.push_back(jet->bDiscriminator("pfJetProbabilityBJetTags"));
    jetJBP_.push_back(jet->bDiscriminator("pfJetBProbabilityBJetTags"));


    if(isAK8PuppiJet_){
      jetTau1_.push_back(jet->userFloat("NjettinessAK8Puppi:tau1"));
      jetTau2_.push_back(jet->userFloat("NjettinessAK8Puppi:tau2"));
      jetTau3_.push_back(jet->userFloat("NjettinessAK8Puppi:tau3"));
      jetTau21_.push_back(jet->userFloat("NjettinessAK8Puppi:tau2")/jet->userFloat("NjettinessAK8Puppi:tau1"));
      jetSDmass_.push_back(jet->userFloat("ak8PFJetsPuppiSoftDropMass"));
    }


    if(isCA15PuppiJet_){
      jetTau1_.push_back(jet->userFloat("NjettinessCA15Puppi:tau1"));
      jetTau2_.push_back(jet->userFloat("NjettinessCA15Puppi:tau2"));
      jetTau3_.push_back(jet->userFloat("NjettinessCA15Puppi:tau3"));
      jetTau21_.push_back(jet->userFloat("NjettinessCA15Puppi:tau2")/jet->userFloat("NjettinessCA15Puppi:tau1"));
      jetSDmass_.push_back(jet->userFloat("ca15PFJetsPuppiSoftDropMass"));
    }

    if(isFATJet_){


      jetTau1_.push_back(jet->userFloat("NjettinessAK8:tau1"));
      jetTau2_.push_back(jet->userFloat("NjettinessAK8:tau2"));
      jetTau3_.push_back(jet->userFloat("NjettinessAK8:tau3"));
      jetTau21_.push_back(jet->userFloat("NjettinessAK8:tau2")/jet->userFloat("NjettinessAK8:tau1"));
      

      //Puppi related information
      jetPuppiTau1_.push_back(jet->userFloat("ak8PFJetsPuppiValueMap:NjettinessAK8PuppiTau1"));
      jetPuppiTau2_.push_back(jet->userFloat("ak8PFJetsPuppiValueMap:NjettinessAK8PuppiTau2"));
      jetPuppiTau3_.push_back(jet->userFloat("ak8PFJetsPuppiValueMap:NjettinessAK8PuppiTau3"));


      TLorentzVector temp_puppi;
      temp_puppi.SetPtEtaPhiM(jet->userFloat("ak8PFJetsPuppiValueMap:pt"),
			      jet->userFloat("ak8PFJetsPuppiValueMap:eta"),
			      jet->userFloat("ak8PFJetsPuppiValueMap:phi"),
			      jet->userFloat("ak8PFJetsPuppiValueMap:mass"));
			      

      new( (*jetPuppiP4_)[nJet_-1]) TLorentzVector(temp_puppi);



      unsigned int nSubSoftDropjets_puppi=0;	
  	
      std::vector<int>   subjetSDFatJetIndex_puppi;
      std::vector<float> subjetSDPx_puppi; 
      std::vector<float> subjetSDPy_puppi; 
      std::vector<float> subjetSDPz_puppi; 
      std::vector<float> subjetSDE_puppi; 	
      std::vector<float> subjetSDCSV_puppi; 	

      subjetSDFatJetIndex_puppi.clear();
      subjetSDPx_puppi.clear();
      subjetSDPy_puppi.clear();
      subjetSDPz_puppi.clear();
      subjetSDE_puppi.clear();
      subjetSDCSV_puppi.clear();

      TLorentzVector puppi_softdrop(0,0,0,0);
      TLorentzVector puppi_softdrop_raw(0,0,0,0);
      auto const & sdSubjetsPuppi = jet->subjets("SoftDropPuppi");
      for ( auto const & it : sdSubjetsPuppi ) {
	nSubSoftDropjets_puppi++;

	subjetSDFatJetIndex_puppi.push_back(nJet_-1);
	subjetSDPx_puppi.push_back(it->px());
	subjetSDPy_puppi.push_back(it->py());
	subjetSDPz_puppi.push_back(it->pz());
	subjetSDE_puppi.push_back(it->energy());	
	subjetSDCSV_puppi.push_back(it->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));	

	puppi_softdrop += TLorentzVector(it->px(),
					 it->py(),
					 it->pz(),
					 it->energy());

	puppi_softdrop_raw += TLorentzVector(it->correctedP4(0).px(),
					     it->correctedP4(0).py(),
					     it->correctedP4(0).pz(),
					     it->correctedP4(0).energy());
      } //subjet loop

      new( (*jetPuppiSDRawP4_)[nJet_-1]) TLorentzVector(puppi_softdrop_raw);


      //      using a different way to get corrected pruned/softdrop mass
      // if reading global tag
      float corr=-1;
      float corrSD=-1;

      if(!useJECText_){	
	// pruned mass: CHS
	std::vector<pat::Jet>::const_iterator jetForPrunedMass = 
	  find_if(jetsForPrunedMass.begin(),
		  jetsForPrunedMass.end(),
		  [&jet](const pat::Jet& item)->bool{return fabs(jet->correctedP4(0).pt()-item.correctedP4(0).pt())<1e-3;});	
    
	if(jetForPrunedMass!=jetsForPrunedMass.end())
	  corr = jetForPrunedMass->pt()/jetForPrunedMass->correctedP4(0).pt();

	// softdrop mass: Puppi
	std::vector<pat::Jet>::const_iterator jetForSoftDropMass = 
	  find_if(jetsForSoftDropMass.begin(),
		  jetsForSoftDropMass.end(),
		  [&jet](const pat::Jet& item)->bool{return fabs(jet->correctedP4(0).pt()-item.correctedP4(0).pt())<1e-3;});	
    
	if(jetForSoftDropMass!=jetsForSoftDropMass.end())
	  corrSD = jetForSoftDropMass->pt()/jetForSoftDropMass->correctedP4(0).pt();

      }
      else if(useJECText_){
	// pruned mass: CHS
	prunedjecText_->setJetEta( uncorrJet.eta() );
	prunedjecText_->setJetPt ( uncorrJet.pt() );
	prunedjecText_->setJetE  ( uncorrJet.energy() );
	prunedjecText_->setJetA  ( jet->jetArea() );
	prunedjecText_->setRho   ( *(h_rho.product()) );
	prunedjecText_->setNPV   ( h_pv->size() );
	corr = prunedjecText_->getCorrection();

	//softdrop mass: puppi
	softdropjecText_->setJetEta( puppi_softdrop_raw.Eta() );
	softdropjecText_->setJetPt ( puppi_softdrop_raw.Pt() );
	softdropjecText_->setJetE  ( puppi_softdrop_raw.E() );
	softdropjecText_->setJetA  ( jet->jetArea() );
	softdropjecText_->setRho   ( *(h_rho.product()) );
	softdropjecText_->setNPV   ( h_pv->size() );
	corrSD = softdropjecText_->getCorrection();

      }

      if(corr<0)
	jetPRmassL2L3Corr_.push_back(DUMMY);
      else
	jetPRmassL2L3Corr_.push_back(corr*jet->userFloat("ak8PFJetsCHSPrunedMass"));


      jetSDmass_.push_back(jet->userFloat("ak8PFJetsCHSSoftDropMass"));
      jetPRmass_.push_back(jet->userFloat("ak8PFJetsCHSPrunedMass"));
 


      if(nSubSoftDropjets_puppi==0)
	{
	  subjetSDFatJetIndex_puppi.push_back(DUMMY);
	  subjetSDPx_puppi.push_back(DUMMY);
	  subjetSDPy_puppi.push_back(DUMMY);
	  subjetSDPz_puppi.push_back(DUMMY);
	  subjetSDE_puppi.push_back(DUMMY);	
	  subjetSDCSV_puppi.push_back(DUMMY);	
	  jetPuppiSDmass_.push_back(DUMMY);
	  jetPuppiSDmassL2L3Corr_.push_back(DUMMY);
	}
      else
	{
	  jetPuppiSDmass_.push_back(puppi_softdrop_raw.M());
	  if(corrSD<0)
	    jetPuppiSDmassL2L3Corr_.push_back(DUMMY);
	  else
	    jetPuppiSDmassL2L3Corr_.push_back(corrSD*puppi_softdrop_raw.M());
	}

           
      nSubSDPuppiJet_.push_back(nSubSoftDropjets_puppi); 
      subjetSDPuppiFatJetIndex_.push_back(subjetSDFatJetIndex_puppi);
      subjetSDPuppiPx_.push_back(subjetSDPx_puppi);
      subjetSDPuppiPy_.push_back(subjetSDPy_puppi);
      subjetSDPuppiPz_.push_back(subjetSDPz_puppi);
      subjetSDPuppiE_.push_back(subjetSDE_puppi);
      subjetSDPuppiCSV_.push_back(subjetSDCSV_puppi);



    } // only for AK8CHS jets

  
    // if this is a AK8 jet
    if(!isTHINJet_ && !isAK4PuppiJet_){
      //HBB tagger
      if(isCA15PuppiJet_)
	jet_DoubleSV_.push_back(jet->bDiscriminator("pfBoostedDoubleSecondaryVertexCA15BJetTags"));
      else
	jet_DoubleSV_.push_back(jet->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags"));
        
      std::vector<float> jet_SVMass_float;
  	  
      jet_SVMass_float.clear();
      unsigned int nSV=0;  

      if(jet->hasTagInfo(svTagInfosCstr_.data()))
	{
	  const reco::CandSecondaryVertexTagInfo *candSVTagInfo = jet->tagInfoCandSecondaryVertex("pfInclusiveSecondaryVertexFinder");
	  nSV = candSVTagInfo->nVertices();                       
  	  
	  for(unsigned int n_2ndvtx=0;n_2ndvtx< nSV;n_2ndvtx++)
	    jet_SVMass_float.push_back(candSVTagInfo->secondaryVertex(n_2ndvtx).p4().mass());
  	  
	} // if there is tagging information
      if(nSV==0)
	jet_SVMass_float.push_back(DUMMY);	    
      jet_nSV_.push_back(nSV);  
      jet_SVMass_.push_back(jet_SVMass_float);
    
    } /// if its ADDJET or FATjet or AK8PuppiJet or CA15PuppiJet
 
    // softdrop subjets
    if(isFATJet_ || isAK8PuppiJet_ || isCA15PuppiJet_){
      // the following lines are common to FAT Jet and AK8PuppiJets
    
      // std::vector<reco::Candidate const *> constituents;
      // for ( unsigned ida = 0; ida < jet->numberOfDaughters(); ++ida ) 
      // 	{
      // 	  reco::Candidate const * cand = jet->daughter(ida);
      // 	  if ( cand->numberOfDaughters() == 0 ) constituents.push_back( cand ) ;
      // 	  else 
      // 	    {
      // 	      for ( unsigned jda = 0; jda < cand->numberOfDaughters(); ++jda ) 
      // 		{
      // 		  reco::Candidate const * cand2 = cand->daughter(jda);
      // 		  constituents.push_back( cand2 );
      // 		}
      // 	    }
      // 	}

      // std::sort( constituents.begin(), constituents.end(), [] (reco::Candidate const * ida, reco::Candidate const * jda){return ida->pt() > jda->pt();} );

      //turn off subjet for not patified now 
      pat::Jet const *jetptr = &*jet;  
      
      //	std::cout<<" working before SoftDrop "<<std::endl;
  	
      auto wSubjets = jetptr->subjets("SoftDrop");
      //	std::cout<<" working after SoftDrop "<<std::endl;
      int nSubSoftDropjets=0;	
  	
      std::vector<int>   subjetSDFatJetIndex;
      std::vector<float> subjetSDPx; 
      std::vector<float> subjetSDPy; 
      std::vector<float> subjetSDPz; 
      std::vector<float> subjetSDE; 	
      std::vector<int>   subjetSDCharge;
      std::vector<int>   subjetSDPartonFlavor;
      std::vector<int>   subjetSDHadronFlavor;
      std::vector<float> subjetSDCSV; 

      subjetSDFatJetIndex.clear();
      subjetSDPx.clear();
      subjetSDPy.clear();
      subjetSDPz.clear();
      subjetSDE.clear();
      subjetSDCharge.clear();
      subjetSDPartonFlavor.clear();
      subjetSDHadronFlavor.clear();
      subjetSDCSV.clear(); 


      for ( auto const & iw : wSubjets ) 
	{

	  nSubSoftDropjets++;
  	      
	  subjetSDFatJetIndex.push_back(nJet_-1);
	  // subjetSDPx.push_back(iw->correctedP4(0).px());
	  // subjetSDPy.push_back(iw->correctedP4(0).py());
	  // subjetSDPz.push_back(iw->correctedP4(0).pz());
	  // subjetSDE.push_back(iw->correctedP4(0).energy());	
	  subjetSDPx.push_back(iw->px());
	  subjetSDPy.push_back(iw->py());
	  subjetSDPz.push_back(iw->pz());
	  subjetSDE.push_back(iw->energy());	
	  subjetSDCharge.push_back(iw->charge());
	  subjetSDPartonFlavor.push_back(iw->partonFlavour());
	  subjetSDHadronFlavor.push_back(iw->hadronFlavour());	      
	  subjetSDCSV.push_back(iw->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));   

	}//subjet loop
      if(nSubSoftDropjets==0)
	{
	  subjetSDFatJetIndex.push_back(DUMMY);
	  subjetSDPx.push_back(DUMMY);
	  subjetSDPy.push_back(DUMMY);
	  subjetSDPz.push_back(DUMMY);
	  subjetSDE.push_back(DUMMY);	
	  subjetSDCharge.push_back(DUMMY);
	  subjetSDPartonFlavor.push_back(DUMMY);
	  subjetSDHadronFlavor.push_back(DUMMY);	      
	  subjetSDCSV.push_back(DUMMY);   
	}
  	    
      nSubSDJet_.push_back(nSubSoftDropjets); 
      subjetSDFatJetIndex_.push_back(subjetSDFatJetIndex);
      subjetSDPx_.push_back(subjetSDPx);
      subjetSDPy_.push_back(subjetSDPy);
      subjetSDPz_.push_back(subjetSDPz);
      subjetSDE_.push_back(subjetSDE);
      subjetSDCharge_.push_back(subjetSDCharge);
      subjetSDPartonFlavor_.push_back(subjetSDPartonFlavor);
      subjetSDHadronFlavor_.push_back(subjetSDHadronFlavor);	      
      subjetSDCSV_.push_back(subjetSDCSV); 
     
  	  

    }//if is Fat jet  or AK8Puppijet or CA15Puppijet


  }//jet loop
    

  // fat jet uncertainty does not exist yet
  if(!useJECText_)
    delete jecUnc_;


}



void
jetTree::SetBranches(){
  
  AddBranch(&nJet_,   "nJet");
  AddBranch(&jetP4_,       "jetP4");

  if(!isADDJet_){
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


  }

  if(isTHINJet_){
    AddBranch(&PUJetID_,   "PUJetID");
    AddBranch(&isPUJetIDLoose_,  "isPUJetIDLoose");
    AddBranch(&isPUJetIDMedium_, "isPUJetIDMedium");
    AddBranch(&isPUJetIDTight_,  "isPUJetIDTight");
  }

  if(isFATJet_ || isAK8PuppiJet_ || isCA15PuppiJet_){

    AddBranch(&jetTau1_,  "jetTau1");
    AddBranch(&jetTau2_,  "jetTau2");
    AddBranch(&jetTau3_,  "jetTau3");
    AddBranch(&jetTau21_, "jetTau21");
    AddBranch(&jetSDmass_,         "jetSDmass");

    // subjet information
    AddBranch(&nSubSDJet_,            "nSubSDJet");
    AddBranch(&subjetSDFatJetIndex_,  "subjetSDFatJetIndex");
    AddBranch(&subjetSDPx_,           "subjetSDPx");     
    AddBranch(&subjetSDPy_,           "subjetSDPy");     
    AddBranch(&subjetSDPz_,           "subjetSDPz");     
    AddBranch(&subjetSDE_,            "subjetSDE");     
    AddBranch(&subjetSDPartonFlavor_, "subjetSDPartonFlavor");
    AddBranch(&subjetSDHadronFlavor_, "subjetSDHadronFlavor");
    AddBranch(&subjetSDCSV_,          "subjetSDCSV");     

  }
  
  if(isFATJet_){

    
    AddBranch(&jetPRmass_,         "jetPRmass");
    AddBranch(&jetPRmassL2L3Corr_, "jetPRmassL2L3Corr");


    // puppi information
    AddBranch(&jetPuppiTau1_,   "jetPuppiTau1");
    AddBranch(&jetPuppiTau2_,   "jetPuppiTau2");
    AddBranch(&jetPuppiTau3_,   "jetPuppiTau3");

    AddBranch(&jetPuppiSDmass_,         "jetPuppiSDmass");
    AddBranch(&jetPuppiSDmassL2L3Corr_, "jetPuppiSDmassL2L3Corr");

    AddBranch(&jetPuppiP4_, "jetPuppiP4");
    AddBranch(&jetPuppiSDRawP4_, "jetPuppiSDRawP4");

    AddBranch(&nSubSDPuppiJet_,           "nSubSDPuppiJet");
    AddBranch(&subjetSDPuppiFatJetIndex_, "subjetSDPuppiFatJetIndex");
    AddBranch(&subjetSDPuppiPx_,          "subjetSDPuppiPx");     
    AddBranch(&subjetSDPuppiPy_,          "subjetSDPuppiPy");     
    AddBranch(&subjetSDPuppiPz_,          "subjetSDPuppiPz");     
    AddBranch(&subjetSDPuppiE_,           "subjetSDPuppiE");     
    AddBranch(&subjetSDPuppiCSV_,           "subjetSDPuppiCSV");     

  }


  if(!isTHINJet_ && !isAK4PuppiJet_)
    {

      AddBranch(&jet_DoubleSV_,"jet_DoubleSV");
      AddBranch(&jet_nSV_,     "jet_nSV");
      AddBranch(&jet_SVMass_,  "jet_SVMass");
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
  jetSSVHE_.clear();
  jetCISVV2_.clear();
  jetTCHP_.clear();
  jetTCHE_.clear();
  jetJP_.clear();
  jetJBP_.clear();


  jetTau1_.clear();
  jetTau2_.clear();
  jetTau3_.clear();
  jetTau21_.clear();


  //ak8jet mass
 
  jetSDmass_.clear(); 
  jetPRmass_.clear();  
  jetPRmassL2L3Corr_.clear();

  // puppi related stuff
  
  jetPuppiTau1_.clear();
  jetPuppiTau2_.clear();
  jetPuppiTau3_.clear();
  jetPuppiSDmass_.clear();
  jetPuppiSDmassL2L3Corr_.clear();

  jetPuppiP4_->Clear();
  jetPuppiSDRawP4_->Clear();
  nSubSDPuppiJet_.clear();
  subjetSDPuppiFatJetIndex_.clear(); 
  subjetSDPuppiPx_.clear();
  subjetSDPuppiPy_.clear();
  subjetSDPuppiPz_.clear();
  subjetSDPuppiE_.clear();
  subjetSDPuppiCSV_.clear();


  //jet  Hbb tagger for fat and add jet

  jet_DoubleSV_.clear();


  //jet secondary vtx

  jet_nSV_.clear();
  jet_SVMass_.clear();
  jet_SVEnergyRatio_.clear();




  // subjet of jets

  nSubSDJet_.clear();
  subjetSDPx_.clear();
  subjetSDPy_.clear();
  subjetSDPz_.clear();
  subjetSDE_.clear();
  subjetSDCharge_.clear();
  subjetSDFatJetIndex_.clear();
  subjetSDPartonFlavor_.clear();
  subjetSDHadronFlavor_.clear();
  subjetSDCSV_.clear();        



}
