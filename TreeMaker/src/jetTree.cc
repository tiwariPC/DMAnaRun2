
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
  isFATJet_(false),
  isADDJet_(false),
  hasJECInfo_(false),
  useJECText_(iConfig.getParameter<bool>("useJECText")),
  JetLabel_(iConfig.getParameter<edm::InputTag>(Form("%sJets",desc.data()))),
  pvSrc_    (iConfig.getParameter<edm::InputTag>("pvSrc") ),                      
  svTagInfosCstr_(iConfig.getParameter<std::string>("svTagInfosPY")),
  jecUncPayLoadName_(iConfig.getParameter<std::string>(Form("%sjecUncPayLoad",desc.data()))),
  jecNames_(iConfig.getParameter<std::vector<std::string> >(Form("%sjecNames",desc.data()) )), 
  jecUncName_(iConfig.getParameter<std::string>(Form("%sjecUncName",desc.data())) ),	
  jet2012ID_()
{
  
  if (desc.find("FAT")!=std::string::npos)
    isFATJet_=true;
  if (desc.find("ADD")!=std::string::npos)
    isADDJet_=true; 

  // temporary switch because now only AK4 jets have uncertainties in both data and MC
  // need to be removed when AK8jet uncertainties are available
  if(!isFATJet_ && !isADDJet_)hasJECInfo_=true;

  genjetP4_    = new TClonesArray("TLorentzVector");
  jetP4_       = new TClonesArray("TLorentzVector");
  unCorrJetP4_ = new TClonesArray("TLorentzVector");
  SetBranches();


  if(useJECText_)
    {
      
      std::vector<JetCorrectorParameters> vPar;

      for ( std::vector<std::string>::const_iterator payloadBegin = 
	      prunedMassJecNames_.begin(),
	      payloadEnd = prunedMassJecNames_.end(), ipayload = payloadBegin; 
	    ipayload != payloadEnd; ++ipayload ) 
	{
	  JetCorrectorParameters pars(*ipayload);
	  vPar.push_back(pars);
	}
      prunedjecText_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );
      
      vPar.clear();
      for ( std::vector<std::string>::const_iterator payloadBegin = 
	      jecNames_.begin(),
	      payloadEnd = jecNames_.end(), ipayload = payloadBegin; 
	    ipayload != payloadEnd; ++ipayload ) 
	{
	  JetCorrectorParameters pars(*ipayload);
	  vPar.push_back(pars);
	}
      jecText_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );

      if(hasJECInfo_)
	jecUncText_ = boost::shared_ptr<JetCorrectionUncertainty>( new JetCorrectionUncertainty(jecUncName_) );
    }


  if(isFATJet_)
    {
      prunedMassJecNames_          = iConfig.getParameter<std::vector<std::string> >(Form("%sprunedMassJecNames",desc.data()));
      PrunedMassJetLabel_          = iConfig.getParameter<edm::InputTag>(Form("%sJetsForPrunedMass",desc.data()));
     // puppiPrunedMassJetLabel_   = iConfig.getParameter<edm::InputTag>("puppiPrunedMassJet");
      // puppiSoftDropMassJetLabel_ = iConfig.getParameter<edm::InputTag>("puppiSoftDropMassJet");
      // ATLASTrimMassJetLabel_     = iConfig.getParameter<edm::InputTag>("ATLASTrimMassJetLabel");
   }

}


jetTree::~jetTree(){

  delete genjetP4_;
  delete jetP4_;
  delete unCorrJetP4_;

}


void
jetTree::Fill(const edm::Event& iEvent, edm::EventSetup const& iSetup){
  Clear();
 
  // // Get the rho collection
  edm::Handle< double > h_rho;
  if(not iEvent.getByLabel("fixedGridRhoFastjetAll", h_rho ))
    {
      std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
	       <<"fixedGridRhoFastjetAll" <<std::endl; 
      exit(0);
    }

  jetRho_ = *(h_rho.product());

  // // Get the primary vertex collection                                         
  edm::Handle<reco::VertexCollection>  h_pv;
  if(not iEvent.getByLabel(pvSrc_,h_pv))
    {
      std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
	       <<pvSrc_<<std::endl; 
      exit(0);
    }

  if (h_pv->empty()) return; // skip the event if no PV found
  const reco::Vertex &PV = h_pv->front();

  jetNPV_=  h_pv->size();

 
  edm::Handle<pat::JetCollection> JetHandle;
  if(not iEvent.getByLabel(JetLabel_,JetHandle))
    {
      std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
	       <<JetLabel_<<std::endl; 
      exit(0);
    }

  // for getting the L2+L3 correction factor of pruned jet mass
  edm::Handle<pat::JetCollection> JetHandleForPrunedMass;
  pat::JetCollection jetsForPrunedMass;

  // edm::Handle<pat::JetCollection> JetHandleForPuppiPrunedMass;
  // pat::JetCollection jetsForPuppiPrunedMass;

  // edm::Handle<pat::JetCollection> JetHandleForPuppiSoftDropMass;
  // pat::JetCollection jetsForPuppiSoftDropMass;

  // edm::Handle<pat::JetCollection> JetHandleForATLASTrimMass;
  // pat::JetCollection jetsForATLASTrimMass;

  if(isFATJet_ && 
     not iEvent.getByLabel(PrunedMassJetLabel_,JetHandleForPrunedMass))
    {
      std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
    	       <<PrunedMassJetLabel_<<std::endl; 
      exit(0);
    }
  // else if(isFATJet_ && 
  //    not iEvent.getByLabel(puppiPrunedMassJetLabel_,JetHandleForPuppiPrunedMass))
  //   {
  //     std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
  //   	       <<puppiPrunedMassJetLabel_<<std::endl; 
  //     exit(0);
  //   }
  // else if(isFATJet_ && 
  //    not iEvent.getByLabel(puppiSoftDropMassJetLabel_,JetHandleForPuppiSoftDropMass))
  //   {
  //     std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
  //   	       <<puppiSoftDropMassJetLabel_<<std::endl; 
  //     exit(0);
  //   }
  // else if(isFATJet_ && 
  //    not iEvent.getByLabel(ATLASTrimMassJetLabel_,JetHandleForATLASTrimMass))
  //   {
  //     std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
  //   	       << ATLASTrimMassJetLabel_<<std::endl; 
  //     exit(0);
  //   }
  else if(isFATJet_ && 
	  iEvent.getByLabel(PrunedMassJetLabel_,JetHandleForPrunedMass) //&&
	  // iEvent.getByLabel(puppiPrunedMassJetLabel_,JetHandleForPuppiPrunedMass) && 
	  // iEvent.getByLabel(puppiSoftDropMassJetLabel_,JetHandleForPuppiSoftDropMass) &&
	  // iEvent.getByLabel(ATLASTrimMassJetLabel_,JetHandleForATLASTrimMass)
	  )
    {
      jetsForPrunedMass       = *(JetHandleForPrunedMass.product());
      // jetsForPuppiPrunedMass  = *(JetHandleForPuppiPrunedMass.product());
      // jetsForPuppiSoftDropMass= *(JetHandleForPuppiSoftDropMass.product());
      // jetsForATLASTrimMass    = *(JetHandleForATLASTrimMass.product());
    }

  // for jet energy uncertainty, using global tag
  JetCorrectionUncertainty *jecUnc_=0;
  // fat jet uncertainty does not exist yet
  if(hasJECInfo_ && !useJECText_){
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
    if(jet->pt() < 20.) continue;
  
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
      if(hasJECInfo_)
	{
	  jecUncText_->setJetEta( uncorrJet.eta() );
	  jecUncText_->setJetPt( corr_jet * uncorrJet.pt() );
	  jetCorrUncUp_.push_back(jecUncText_->getUncertainty(true));

	  jecUncText_->setJetEta( uncorrJet.eta() );
	  jecUncText_->setJetPt( corr_jet * uncorrJet.pt() );
	  jetCorrUncDown_.push_back(jecUncText_->getUncertainty(false));

	}

    }
    else
      new( (*jetP4_)[nJet_-1]) TLorentzVector(jet->p4().px(),
					      jet->p4().py(),
					      jet->p4().pz(),
					      jet->p4().energy());


    // get jet energy scale uncertainty and related input variables
    // fat jet uncertainty does not exist yet, if using database
    if(hasJECInfo_  && !useJECText_){

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



    float jpumva=0.;
    jpumva= jet->userFloat("pileupJetId:fullDiscriminant");
    //std::cout<<" jpumva = "<<jpumva<<std::endl;
    PUJetID_.push_back(jpumva);
  
    float jpt = jet->pt();
    float jeta = jet->eta();
  
    bool passPU = true;
    if(jpt>20){
      if(jeta>3.){
   	if(jpumva<=-0.45)passPU=false;
      }else if(jeta>2.75){
   	if(jpumva<=-0.55)passPU=false;
      }else if(jeta>2.5){
   	if(jpumva<=-0.6)passPU=false;
      }else if(jpumva<=-0.63)passPU=false;
    }else{
      if(jeta>3.){
   	if(jpumva<=-0.95)passPU=false;
      }else if(jeta>2.75){
   	if(jpumva<=-0.94)passPU=false;
      }else if(jeta>2.5){
   	if(jpumva<=-0.96)passPU=false;
      }else if(jpumva<=-0.95)passPU=false;
    }
  
    isPUJetID_.push_back(passPU);
  
    jetCEmEF_.push_back(jet->chargedEmEnergyFraction());
    jetCHadEF_.push_back(jet->chargedHadronEnergyFraction());
    jetPhoEF_.push_back(jet->photonEnergyFraction());
    jetNEmEF_.push_back(jet->neutralEmEnergyFraction());
    jetNHadEF_.push_back(jet->neutralHadronEnergyFraction());

    jetMuEF_.push_back(jet->muonEnergyFraction());
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

    jetTau1_.push_back(jet->userFloat("NjettinessAK8:tau1"));
    jetTau2_.push_back(jet->userFloat("NjettinessAK8:tau2"));
    jetTau3_.push_back(jet->userFloat("NjettinessAK8:tau3"));
    jetTau4_.push_back(jet->userFloat("NjettinessAK8:tau2")/jet->userFloat("NjettinessAK8:tau1"));
  
    

    if(isFATJet_){

      //      using a different way to get corrected pruned mass
      // if reading global tag
      float corr=-1;
      if(!useJECText_){
	std::vector<pat::Jet>::const_iterator jetForPrunedMass = 
	  find_if(jetsForPrunedMass.begin(),
		  jetsForPrunedMass.end(),
		  [&jet](const pat::Jet& item)->bool{return fabs(jet->correctedP4(0).pt()-item.correctedP4(0).pt())<1e-3;});	
    
	if(jetForPrunedMass!=jetsForPrunedMass.end())
	  corr = jetForPrunedMass->pt()/jetForPrunedMass->correctedP4(0).pt();
	
      }
      else if(useJECText_){
	prunedjecText_->setJetEta( uncorrJet.eta() );
	prunedjecText_->setJetPt ( uncorrJet.pt() );
	prunedjecText_->setJetE  ( uncorrJet.energy() );
	prunedjecText_->setJetA  ( jet->jetArea() );
	prunedjecText_->setRho   ( *(h_rho.product()) );
	prunedjecText_->setNPV   ( h_pv->size() );
	corr = prunedjecText_->getCorrection();
      }

      if(corr<0)
	jetPRmassL2L3Corr_.push_back(DUMMY);
      else
	jetPRmassL2L3Corr_.push_back(corr*jet->userFloat("ak8PFJetsCHSPrunedMass"));

      

      // std::vector<pat::Jet>::const_iterator jetForPuppiPrunedMass= find_if(jetsForPuppiPrunedMass.begin(),
      // 									   jetsForPuppiPrunedMass.end(),
      // 									   [&jet](const pat::Jet& item)->bool{return deltaR(jet->eta(),jet->phi(),item.eta(),item.phi())<0.4;});

      // if(jetForPuppiPrunedMass!=jetsForPuppiPrunedMass.end())
      // 	{
      // 	  jetPRmassPuppiL2L3Corr_.push_back(jetForPuppiPrunedMass->mass()); 
      // 	}
      // else
      // 	jetPRmassPuppiL2L3Corr_.push_back(DUMMY);



      // std::vector<pat::Jet>::const_iterator jetForPuppiSoftDropMass= find_if(jetsForPuppiSoftDropMass.begin(),
      // 									   jetsForPuppiSoftDropMass.end(),
      // 									   [&jet](const pat::Jet& item)->bool{return deltaR(jet->eta(),jet->phi(),item.eta(),item.phi())<0.4;});
      
      // if(jetForPuppiSoftDropMass!=jetsForPuppiSoftDropMass.end())
      // 	{
      // 	  jetSDmassPuppiL2L3Corr_.push_back(jetForPuppiSoftDropMass->mass()); 
      // 	}
      // else
      // 	jetSDmassPuppiL2L3Corr_.push_back(DUMMY); 



      // std::vector<pat::Jet>::const_iterator jetForATLASTrimMass= find_if(jetsForATLASTrimMass.begin(),
      // 									 jetsForATLASTrimMass.end(),
      // 									 [&jet](const pat::Jet& item)->bool{return deltaR(jet->eta(),jet->phi(),item.eta(),item.phi())<0.4;});
      
      // if(jetForATLASTrimMass!=jetsForATLASTrimMass.end())
      // 	{
      // 	  jetATLASmassL2L3Corr_.push_back(jetForATLASTrimMass->mass()); 
      // 	}
      // else
      // 	jetATLASmassL2L3Corr_.push_back(DUMMY); 

      jetSDmass_.push_back(jet->userFloat("ak8PFJetsCHSSoftDropMass"));
      jetTRmass_.push_back(jet->userFloat("ak8PFJetsCHSTrimmedMass")); 
      jetPRmass_.push_back(jet->userFloat("ak8PFJetsCHSPrunedMass"));
      jetFimass_.push_back(jet->userFloat("ak8PFJetsCHSFilteredMass"));
      

    }

  

    if(isADDJet_){
      //HBB tagger
      jet_DoubleSV_.push_back(jet->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags"));
    
    
      // fill discriminator histograms
      //     const reco::VertexCompositePtrCandidate *svTagInfo =   jet->tagInfoSecondaryVertex( svTagInfosCstr_.data());        
    
      //   const reco::Vertex *svTagInfo =   jet->tagInfoSecondaryVertex( svTagInfosCstr_.data());
      //   cout<<"numbers of 2nd vtx "<<svTagInfo->nVertices()<<endl;               
    
      //         const reco::TemplatedSecondaryVertexTagInfo<reco::TrackIPTagInfo,reco::Vertex> *SecondaryVertexTagInfo=jet->tagInfoSecondaryVertex( svTagInfosCstr_.data());
      //       const reco::SecondaryVertexTagInfo &svTagInfo =*jet->tagInfoSecondaryVertex("secondaryVertex");  
    
      //cout<<" has tag info: "<<jet->hasTagInfo("pfInclusiveSecondaryVertexFinder") <<endl;
    
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
    
    } /// if its ADDJET
 
    if(isFATJet_ || isADDJet_){
      // the following lines are common to FAT Jet and ADDJets
    
      std::vector<reco::Candidate const *> constituents;
      for ( unsigned ida = 0; ida < jet->numberOfDaughters(); ++ida ) 
   	{
   	  reco::Candidate const * cand = jet->daughter(ida);
   	  if ( cand->numberOfDaughters() == 0 ) constituents.push_back( cand ) ;
   	  else 
   	    {
   	      for ( unsigned jda = 0; jda < cand->numberOfDaughters(); ++jda ) 
   		{
   		  reco::Candidate const * cand2 = cand->daughter(jda);
   		  constituents.push_back( cand2 );
   		}
   	    }
   	}

      std::sort( constituents.begin(), constituents.end(), [] (reco::Candidate const * ida, reco::Candidate const * jda){return ida->pt() > jda->pt();} );

      for ( unsigned int ida = 0; ida < constituents.size(); ++ida ) {
   	const pat::PackedCandidate &cand = dynamic_cast<const pat::PackedCandidate &>(*constituents[ida]);
   	//	  printf(" constituent %3d: pt %6.2f, dz(pv) %+.3f, pdgId %+3d\n", ida,cand.pt(),cand.dz(PV.position()),cand.pdgId());
      }


      //   const reco::SecondaryVertexTagInfo *svTagInfo =   jet->tagInfoSecondaryVertex( svTagInfosCstr_.data());        
    
      //   const reco::SecondaryVertexTagInfo *svTagInfo =   jet->tagInfoSecondaryVertex("secondaryVertex");
      //  cout<<"numbers of 2nd vtx "<<svTagInfo->nVertices()<<endl;               

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
     
  	  

    }//if is Fat jet or add jets


  }//jet loop


  // fat jet uncertainty does not exist yet
  if(hasJECInfo_)
    delete jecUnc_;


}



void
jetTree::SetBranches(){
  
  AddBranch(&nJet_, "nJet");
  AddBranch(&jetRho_, "jetRho");
  AddBranch(&jetNPV_, "jetNPV");

  AddBranch(&genjetP4_,"genjetP4");
  AddBranch(&genjetEM_ ,"genjetEM");
  AddBranch(&genjetHAD_ ,"genjetHAD");
  AddBranch(&genjetINV_ ,"genjetINV");
  AddBranch(&genjetAUX_ ,"genjetAUX");
  AddBranch(&matchedDR_ ,"matchedDR");

  AddBranch(&jetRawFactor_,"jetRawFactor");

  AddBranch(&jetP4_, "jetP4");
  AddBranch(&unCorrJetP4_, "unCorrJetP4");

  AddBranch(&jetArea_,"jetArea");
  AddBranch(&jetCorrUncUp_,"jetCorrUncUp");
  AddBranch(&jetCorrUncDown_,"jetCorrUncDown");

  AddBranch(&jetCharge_, "jetCharge");
  AddBranch(&jetPartonFlavor_, "jetPartonFlavor");
  AddBranch(&jetHadronFlavor_, "jetHadronFlavor");
  AddBranch(&jetPassIDLoose_, "jetPassIDLoose");
  AddBranch(&jetPassIDTight_, "jetPassIDTight");
  AddBranch(&PUJetID_,"PUJetID");
  AddBranch(&isPUJetID_,"isPUJetID");


  AddBranch(&jetCEmEF_, "jetCEmEF");
  AddBranch(&jetCHadEF_, "jetCHadEF");
  AddBranch(&jetPhoEF_, "jetPhoEF");
  AddBranch(&jetNEmEF_, "jetNEmEF");
  AddBranch(&jetNHadEF_, "jetNHadEF");

  AddBranch(&jetMuEF_, "jetMuEF");
  
  AddBranch(&jetCMulti_, "jetCMulti");

  AddBranch(&jetSSV_, "jetSSV");
  AddBranch(&jetCSV_, "jetCSV");        
  AddBranch(&jetSSVHE_,"jetSSVHE");
  AddBranch(&jetCISVV2_,"jetCISVV2");
  AddBranch(&jetTCHP_, "jetTCHP");
  AddBranch(&jetTCHE_, "jetTCHE");
  AddBranch(&jetJP_, "jetJP");
  AddBranch(&jetJBP_, "jetJBP");


  
  if(isFATJet_){

    AddBranch(&jetSDmass_, "jetSDmass");
    AddBranch(&jetTRmass_, "jetTRmass");
    AddBranch(&jetPRmass_, "jetPRmass");
    AddBranch(&jetFimass_, "jetFimass");
    AddBranch(&jetPRmassL2L3Corr_, "jetPRmassL2L3Corr");
    // AddBranch(&jetSDmassPuppiL2L3Corr_, "jetSDmassPuppiL2L3Corr");
    // AddBranch(&jetPRmassPuppiL2L3Corr_, "jetPRmassPuppiL2L3Corr");
    // AddBranch(&jetATLASmassL2L3Corr_, "jetATLASmassL2L3Corr");

  }


  if(isADDJet_)
    {
      AddBranch(&jet_DoubleSV_,"jet_DoubleSV");
      AddBranch(&jet_nSV_, "jet_nSV");
      AddBranch(&jet_SVMass_, "jet_SVMass");
      // AddBranch(&jet_SVEnergyRatio_, "jet_SVEnergyRatio");
    }

  if(isFATJet_ || isADDJet_)
    {
      AddBranch(&jetTau1_, "jetTau1");
      AddBranch(&jetTau2_, "jetTau2");
      AddBranch(&jetTau3_, "jetTau3");
      AddBranch(&jetTau4_, "jetTau4");

      AddBranch(&nSubSDJet_,"nSubSDJet");
      AddBranch(&subjetSDFatJetIndex_,"subjetSDFatJetIndex");
      AddBranch(&subjetSDPx_, "subjetSDPx");     
      AddBranch(&subjetSDPy_, "subjetSDPy");     
      AddBranch(&subjetSDPz_, "subjetSDPz");     
      AddBranch(&subjetSDE_, "subjetSDE");     
      AddBranch(&subjetSDPartonFlavor_,"subjetSDPartonFlavor");
      AddBranch(&subjetSDHadronFlavor_,"subjetSDHadronFlavor");
      AddBranch(&subjetSDCSV_, "subjetSDCSV");     

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
  isPUJetID_.clear();

  //Energy Fraction and Multiplicity 

  jetCEmEF_.clear();
  jetCHadEF_.clear();
  jetPhoEF_.clear();
  jetNEmEF_.clear();
  jetNHadEF_.clear();

  jetMuEF_.clear();
  jetChMuEF_.clear();

  jetHFHadEF_.clear();
  jetHFEMEF_.clear();
  jetHOEnergy_.clear();
  jetHOEF_.clear();


  jetCMulti_.clear();
  jetEleMultiplicity_.clear();
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
  jetTau4_.clear();


  //ak8jet mass
 
  jetSDmass_.clear(); 
  jetTRmass_.clear();
  jetPRmass_.clear();
  jetFimass_.clear();
  
  jetPRmassL2L3Corr_.clear();
  // jetSDmassPuppiL2L3Corr_.clear();
  // jetPRmassPuppiL2L3Corr_.clear();
  // jetATLASmassL2L3Corr_.clear();


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
