
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

#include "DelPanj/TreeMaker/interface/jetTree.h"
#include <CLHEP/Vector/LorentzVector.h>
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "TMath.h"
#include "Math/VectorUtil.h"

#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"


typedef math::XYZTLorentzVector LorentzVector;
const math::XYZPoint & position(const reco::Vertex & sv) {return sv.position();}
const math::XYZPoint & position(const reco::VertexCompositePtrCandidate & sv) {return sv.vertex();}

jetTree::jetTree(std::string desc, TTree* tree, const edm::ParameterSet& iConfig):
  baseTree(desc, tree),
  isFATJet_(true),
  JetLabel_(iConfig.getParameter<edm::InputTag>(Form("%sJets",desc.data()))),
//  PrunedJetLabel_ ( iConfig.getParameter<edm::InputTag>("PrunedJets")),
  AddjetlabelC_  (iConfig.getParameter<edm::InputTag>("AddjetlabelPY") ),
  rhoSrc_   (iConfig.getParameter<edm::InputTag>("rhoSrc") ),                     
  pvSrc_    (iConfig.getParameter<edm::InputTag>("pvSrc") ),                      
  SubJetCollectionC_ ( iConfig.getParameter<edm::InputTag>("SubJetsPY") ),  
  svTagInfosCstr_(iConfig.getParameter<std::string>("svTagInfosPY")),
//jecPayloadNames_( iConfig.getParameter<std::vector<std::string> >(Form("%sjecPayloadNames",desc.data()) )), 
 // jecUncName_( iConfig.getParameter<std::string>(Form("%sjecUncName",desc.data())) ),	
  jet2012ID_()
{
  
  std::size_t found = desc.find("FAT");
  if (found!=std::string::npos)
    isFATJet_=true;
  else
    isFATJet_=false;
  if(strcmp(desc.data(),"ADD")==0) isADDJet_=true;
  else isADDJet_=false; 
  runAddJet_ = iConfig.getParameter<Bool_t>("runAddjetPY");
  SetBranches();


  //Get the factorized jet corrector parameters.                                                                                                                        
  // std::vector<JetCorrectorParameters> vPar;
  // for ( std::vector<std::string>::const_iterator payloadBegin = 
  //  	  jecPayloadNames_.begin(),
  // 	  payloadEnd = jecPayloadNames_.end(), ipayload = payloadBegin; 
  //  	ipayload != payloadEnd; ++ipayload ) 
  //   {
  //     JetCorrectorParameters pars(*ipayload);
  //     vPar.push_back(pars);
  //   }

  // // Make the FactorizedJetCorrector and Uncertainty                                                                                                                    
  // jec_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );
  // jecUnc_ = boost::shared_ptr<JetCorrectionUncertainty>( new JetCorrectionUncertainty(jecUncName_) );


}


jetTree::~jetTree(){

}


void
jetTree::Fill(const edm::Event& iEvent, edm::EventSetup const& iSetup){
  Clear();

  cout<<isFATJet_<<"----------------- jet Event loop start------------------"<<endl;
  cout<<"mini AOD Lable:"<<JetLabel_<<"............................................"<<endl; 
  cout<<"add jet  Lable:"<<AddjetlabelC_<<"............................................"<<endl;  
 


  // // Get the mean pt per unit area ("rho")                                     
  // edm::Handle< double > h_rho;
  // iEvent.getByLabel( rhoSrc_, h_rho );

  // // Get the primary vertex collection                                         
  edm::Handle<reco::VertexCollection>  h_pv;
  iEvent.getByLabel( pvSrc_, h_pv );
  if(not iEvent.getByLabel(pvSrc_,h_pv)){
    std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
	     <<pvSrc_<<std::endl; exit(0);}
  


  if (h_pv->empty()) return; // skip the event if no PV found
  const reco::Vertex &PV = h_pv->front();
 

   edm::Handle<pat::JetCollection> JetHandle;
  iEvent.getByLabel(JetLabel_,JetHandle);
  //iEvent.getByLabel("selectedPatJetsAK8",JetHandle);

  if(not iEvent.getByLabel(JetLabel_,JetHandle)){
    std::cout<<"FATAL EXCEPTION: in beginging "<<"Following Not Found: "
	     <<JetLabel_<<std::endl; exit(0);}
  
  
   

  

//  edm::Handle<std::vector<pat::Jet> > PrunedJetHandle;
//  iEvent.getByLabel(PrunedJetLabel_,PrunedJetHandle);

//  if(not iEvent.getByLabel(PrunedJetLabel_,PrunedJetHandle)){
//   std::cout<<"FATAL EXCEPTION: "<<"Following Not Found: "
//  	     <<PrunedJetLabel_<<std::endl; exit(0);}

  pat::JetCollection jets(*(JetHandle.product()));
  std::sort(jets.begin(),jets.end(),PtGreater());

  std::vector<pat::Jet>::const_iterator jet =jets.begin();   

  for(;jet!=jets.end();jet++){
    nJet_++;
    //Stuff common for all jets.
    if(isADDJet_) continue;

    jetTau1_.push_back(jet->userFloat("NjettinessAK8:tau1"));
    jetTau2_.push_back(jet->userFloat("NjettinessAK8:tau2"));
    jetTau3_.push_back(jet->userFloat("NjettinessAK8:tau3"));
    jetTau4_.push_back(jet->userFloat("NjettinessAK8:tau2")/jet->userFloat("NjettinessAK8:tau1"));
 
/*
    std::cout<<" i jet = "<< nJet_
	     <<std::endl;
    std::cout<<" tau 1 = "<<jet->userFloat("NjettinessAK8:tau1")
	     <<" tau 2 = "<<jet->userFloat("NjettinessAK8:tau2")
	     <<" tau 3 = "<<jet->userFloat("NjettinessAK8:tau3")
	     <<" nsubjets= "<<jet->nSubjetCollections()
              <<std::endl;
*/












    
    // now making correction of jet energy
    // reco::Candidate::LorentzVector uncorrJet;
    // uncorrJet = jet->correctedP4(0);


    // // Get the correction itself. This needs the jet area,                     
    // // the rho value, and the number of primary vertices to                    
    // // run the correction.                                                     
    // jec_->setJetEta( uncorrJet.eta() );
    // jec_->setJetPt ( uncorrJet.pt() );
    // jec_->setJetE  ( uncorrJet.energy() );
    // jec_->setJetA  ( jet->jetArea() );
    // jec_->setRho   ( *(h_rho.product()) );
    // jec_->setNPV   ( h_pv->size() );

    // Float_t corr = jec_->getCorrection();

    // // Now access the uncertainty on the jet energy correction.                
    // // Pass the corrected jet pt to the "setJetPt" method.                     

    // // Access the "scale up" uncertainty (+1)                                  
    // jecUnc_->setJetEta( uncorrJet.eta() );
    // jecUnc_->setJetPt( corr * uncorrJet.pt() );
    // jetCorrUncUp_.push_back(jecUnc_->getUncertainty(1));

    // jecUnc_->setJetEta( uncorrJet.eta() );
    // jecUnc_->setJetPt( corr * uncorrJet.pt() );
    // jetCorrUncDown_.push_back(jecUnc_->getUncertainty(-1));
    
    if (jet->genJet()){
      double DR =0 ;
      double a = ( jet->p4().eta() - jet->genJet()->p4().eta()) * ( jet->p4().eta() - jet->genJet()->p4().eta());
      double b = ( jet->p4().phi() - jet->genJet()->p4().phi()) * ( jet->p4().phi() - jet->genJet()->p4().phi());
      DR = sqrt(a+b);
      
      
      genjetPx_.push_back(jet->genJet()->p4().px());
      genjetPy_.push_back(jet->genJet()->p4().py());
      genjetPz_.push_back(jet->genJet()->p4().pz());
      genjetEn_.push_back(jet->genJet()->p4().energy());
      
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
		  //<< " genjet tot = "<<jet->genJet()->emEnergy()
		  //+jet->genJet()->hadEnergy()
		  //+jet->genJet()->invisibleEnergy()
		  //+jet->genJet()->auxiliaryEnergy()
			 <<" recojet pho = "<<jet->photonEnergyFraction()*jet->energy()
			 <<" recojet ele = "<<jet->chargedEmEnergyFraction()*jet->energy()
			 <<" dr = "<<DR
			 <<std::endl;
    }
    else{
      genjetEM_.push_back(-999.0);
      genjetHAD_.push_back(-999.0);
      genjetINV_.push_back(-999.0);
      genjetAUX_.push_back(-999.0);
      matchedDR_.push_back(-999.0);
      genjetPx_.push_back(-999.9);
      genjetPy_.push_back(-999.9);
      genjetPz_.push_back(-999.9);
      genjetEn_.push_back(-999.9);
    }
    jetRawFactor_.push_back(jet->jecFactor("Uncorrected"));
    jetPt_.push_back(jet->pt());
    jetEta_.push_back(jet->eta());
    jetPhi_.push_back(jet->phi());
    jetM_.push_back(jet->mass());
    jetEn_.push_back(jet->energy());


    jetCharge_.push_back(jet->charge());
    jetPartonFlavor_.push_back(jet->partonFlavour());
    
    std::map<std::string, bool> Pass = jet2012ID_.MergedJetCut(*jet);
    Int_t passOrNot = PassAll(Pass); 
    jetPassID_.push_back(passOrNot);


    jetSSV_.push_back(jet->bDiscriminator("pfSimpleSecondaryVertexHighPurBJetTags"));
    jetSSVHE_.push_back(jet->bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags"));
    jetCSV_.push_back(jet->bDiscriminator("combinedSecondaryVertexBJetTags"));        
    jetTCHP_.push_back(jet->bDiscriminator("pfTrackCountingHighPurBJetTags"));
    jetTCHE_.push_back(jet->bDiscriminator("pfTrackCountingHighEffBJetTags"));
    jetJP_.push_back(jet->bDiscriminator("pfJetProbabilityBJetTags"));
    jetJBP_.push_back(jet->bDiscriminator("pfJetBProbabilityBJetTags"));
    jetCISVV2_.push_back(jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
   /* 
    std::cout<<" pt of jet = "<<jet->pt()
	     <<" pfSimpleSecondaryVertexHighPurBJetTags = "<<jet->bDiscriminator("pfSimpleSecondaryVertexHighPurBJetTags")
             
             <<"pfSimpleSecondaryVertexHighEffBJetTags "<<jet->bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags")
	     
             <<" csv  = "<<jet->bDiscriminator("combinedSecondaryVertexBJetTags")
	     <<" pfTrackCountingHighPurBJetTags = "<<jet->bDiscriminator("pfTrackCountingHighPurBJetTags")
	     <<" pfTrackCountingHighEffBJetTags = "<<jet->bDiscriminator("pfTrackCountingHighEffBJetTags")
	     <<" pfJetProbabilityBJetTags = "<<jet->bDiscriminator("pfJetProbabilityBJetTags")
	     <<" pfJetBProbabilityBJetTags = "<<jet->bDiscriminator("pfJetBProbabilityBJetTags")
	     <<" pfCombinedInclusiveSecondaryVertexV2BJetTags = "<<jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags")

	     <<std::endl;
    */
    
    jetMuEF_.push_back(jet->muonEnergyFraction());
    jetPhoEF_.push_back(jet->photonEnergyFraction());
    jetCEmEF_.push_back(jet->chargedEmEnergyFraction());
    jetCHadEF_.push_back(jet->chargedHadronEnergyFraction());
    jetNEmEF_.push_back(jet->neutralEmEnergyFraction());
    jetNHadEF_.push_back(jet->neutralHadronEnergyFraction());
    jetCMulti_.push_back(jet->chargedMultiplicity());
 

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


    jetHFHadEF_.push_back(jet->HFHadronEnergyFraction());
    jetHFEMEF_.push_back(jet->HFEMEnergyFraction());
    jetCHHadMultiplicity_.push_back(jet->chargedHadronMultiplicity());
    jetNHadMulplicity_.push_back(jet->neutralHadronMultiplicity());
    jetPhMultiplicity_.push_back(jet->photonMultiplicity());
    jetEleMultiplicity_.push_back(jet->electronMultiplicity());
    jetHFHadMultiplicity_.push_back(jet->HFHadronMultiplicity());
    jetHFEMMultiplicity_.push_back(jet->HFEMMultiplicity());
    jetChMuEF_.push_back(jet->chargedMuEnergyFraction());
    jetNMultiplicity_.push_back(jet->neutralMultiplicity());
    jetHOEnergy_.push_back(jet->hoEnergy());
    jetHOEF_.push_back(jet->hoEnergyFraction());

















   
    // Raman Testing starts 
    //jetPrunedM_.push_back(jet->userFloat("ak8PFJetsCHSPrunedLinks"));
    //jetPrunedM_.push_back(jet->userFloat("ak8PFJetsCHSPrunedMass"));
    //std::cout<<" pruned mass = "<<jet->userFloat("ak8PFJetsCHSPrunedMass")
    //	     <<" taus1 "<< jet->userFloat("NjettinessAK8:tau1")
    //	     <<" taus2 "<< jet->userFloat("NjettinessAK8:tau2")
    //	     <<" taus3 "<< jet->userFloat("NjettinessAK8:tau3")
    //	     <<std::endl;
    
    //std::cout<<" number of daughters = "<<jet->numberOfDaughters()<<std::endl;
    

    // Raman testing ends
    
    /*
      pat::Jet const *subjet = dynamic_cast <pat::Jet const *> (jet->daughter(0) );
      std::cout<<"subjet prop = "
      <<" pT = "<<subjet->pt()
      <<" eta = "<<subjet->eta()
      <<std::endl;
    */
    // look for pruned jet
    
    if(isFATJet_) // only run with FATjet
    {
    jetSDmass_.push_back(jet->userFloat("ak8PFJetsCHSSoftDropMass"));
    jetTRmass_.push_back(jet->userFloat("ak8PFJetsCHSTrimmedMass")); 
    jetPRmass_.push_back(jet->userFloat("ak8PFJetsCHSPrunedMass"));
    jetFimass_.push_back(jet->userFloat("ak8PFJetsCHSFilteredMass"));

// for miniAOD with AK8 jets, the pruned mass and subjets information are included  
// no dr check is make now, leave the code in case we need more subjet information
// in the future   #yunju 20150427

/*    
     Float_t dRmin = 99999.0;
     Float_t prunedJetArea=0;
    
     pat::JetCollection::const_iterator pj;
     bool findPrunedJet=false;

    
    reco::Candidate::LorentzVector uncorrPrunedJet(0,0,0,0);
    if(iEvent.getByLabel(PrunedJetLabel_,PrunedJetHandle)){
      for(pat::JetCollection::const_iterator jetPruned = PrunedJetHandle->begin(); 
     	  jetPruned != PrunedJetHandle->end(); ++jetPruned) {
	Float_t dRtmp = ROOT::Math::VectorUtil::DeltaR(jet->p4(),jetPruned->p4());
	std::cout<<" drtmp = "<<dRtmp<<std::endl;
      
    
     	if(dRtmp<dRmin && dRtmp<0.8 ){//matching failed if greater than jet radius
     	  dRmin=dRtmp;
     	  findPrunedJet=true;
	  
     	  pj = jetPruned;

     	  uncorrPrunedJet = jetPruned->correctedP4(0);
     	  prunedJetArea = jetPruned->jetArea();


     	}
       } // end of loop over PrunedJets
     } // if one could find CA8PrunedJet
  


  
    cout<<findPrunedJet<<" area  "<<prunedJetArea<<endl;
    // // now making correction for pruned jets
     if(findPrunedJet){
*/
    //   jec_->setJetEta( uncorrPrunedJet.eta() );
    //   jec_->setJetPt ( uncorrPrunedJet.pt() );
    //   jec_->setJetE  ( uncorrPrunedJet.energy() );
    //   jec_->setJetA  ( prunedJetArea );
    //   jec_->setRho   ( *(h_rho.product()) );
    //   jec_->setNPV   ( h_pv->size() );
      

    //   Float_t corrPruned   = jec_->getCorrection();
      
    //   jecUnc_->setJetEta( uncorrPrunedJet.eta() );
    //   jecUnc_->setJetPt( corrPruned *uncorrPrunedJet.pt() );
    //   jetPrunedCorrUncUp_.push_back(jecUnc_->getUncertainty(+1));

    //   jecUnc_->setJetEta( uncorrPrunedJet.eta() );
    //   jecUnc_->setJetPt( corrPruned *uncorrPrunedJet.pt() );
    //   jetPrunedCorrUncDown_.push_back(jecUnc_->getUncertainty(-1));

    //   jetPrunedPt_.push_back(pj->pt());
    //   jetPrunedEta_.push_back(pj->eta());
    //   jetPrunedPhi_.push_back(pj->phi());
    //   jetPrunedM_.push_back(pj->mass());
    //   jetPrunedEn_.push_back(pj->energy());
    //   jetPrunedCharge_.push_back(pj->charge());
    //   jetPrunedPartonFlavor_.push_back(pj->partonFlavour());


    //   jetPrunedSSV_.push_back(pj->bDiscriminator("simpleSecondaryVertexHighPurBJetTags"));
    //   jetPrunedCSV_.push_back(pj->bDiscriminator("combinedSecondaryVertexBJetTags"));        
    //   jetPrunedTCHP_.push_back(pj->bDiscriminator("trackCountingHighPurBJetTags"));
    //   jetPrunedTCHE_.push_back(pj->bDiscriminator("trackCountingHighEffBJetTags"));
    //   jetPrunedJP_.push_back(pj->bDiscriminator("jetProbabilityBJetTags"));
//    //   jetPrunedJBP_.push_back(pj->bDiscriminator("jetBProbabilityBJetTags"));
  
//}//has pruned jet




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
        
      
        auto wSubjets = jetptr->subjets("SoftDrop");
 
        int nSubSoftDropjets=0;	
       
      std::vector<Float_t> subjetSDPt;
      std::vector<Float_t> subjetSDEta;
      std::vector<Float_t> subjetSDPhi;
      std::vector<Float_t> subjetSDM;
      std::vector<Float_t> subjetSDEn;
      std::vector<Int_t>   subjetSDCharge;
      std::vector<Int_t>   subjetSDPartonFlavor;
      std::vector<Float_t> subjetSDCSV; 

 //     subjetMotherIndex.clear();
      subjetSDPt.clear();
      subjetSDEta.clear();
      subjetSDPhi.clear();
      subjetSDM.clear();
      subjetSDEn.clear();
      subjetSDCharge.clear();
      subjetSDPartonFlavor.clear();
      subjetSDCSV.clear(); 




       for ( auto const & iw : wSubjets ) 
       {
	 // printf("   w subjet with pt %5.1f (raw pt %5.1f), eta %+4.2f, mass %5.1f ungroomed\n",
	//	 iw->pt(), iw->pt()*iw->jecFactor("Uncorrected"), iw->eta(), iw->mass() );
	  nSubSoftDropjets++;

          






        //  float thept=0.0;
        //  thept= float(iw->pt()); 
        //  cout<<thept<<endl;
        subjetSDPt.push_back(iw->pt());
        subjetSDEta.push_back(iw->eta());
        subjetSDPhi.push_back(iw->phi());
        subjetSDM.push_back(iw->mass());
        subjetSDCharge.push_back(iw->charge());
          subjetSDCSV.push_back(iw->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));   
          // cout<<"Test Subjet:"<<iw->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags")<<endl;

		}

       nSubSDJet_.push_back(nSubSoftDropjets); 
       subjetSDPt_.push_back(subjetSDPt); 
       subjetSDEta_.push_back(subjetSDEta);
       subjetSDPhi_.push_back(subjetSDPhi);
       subjetSDM_.push_back(subjetSDM);
      // subjetSDEn_.push_back(subjetSDEn);
       subjetSDCSV_.push_back(subjetSDCSV); 
       subjetSDCharge_.push_back(subjetSDCharge);

      }//if is Fat jet


//	auto tSubjets = jetptr->subjets("CMSTopTag");
//	for ( auto const & it : tSubjets ) {
//	  printf("   t subjet with pt %5.1f (raw pt %5.1f), eta %+4.2f, mass %5.1f ungroomed\n",
//		 it->pt(), it->pt()*it->jecFactor("Uncorrected"), it->eta(), it->mass() );
	
//	}

//turn off subjet for not patified now

 

}//jet loop




// take remake jets


   // define a jet handle
//   edm::Handle<std::vector<pat::Jet> > rejets;




    if(runAddJet_&&isADDJet_)
    {
     cout<<"star"<<endl; 
     edm::Handle<pat::JetCollection> rejets; 
     // get jets from the event
     iEvent.getByLabel(JetLabel_, rejets);

     //get subjet

     edm::Handle<pat::JetCollection>  subjetColls;
     iEvent.getByLabel(SubJetCollectionC_, subjetColls);

         // loop over jets
         
         
         
      for( auto jet = rejets->begin(); jet != rejets->end(); ++jet )
      {
             

           cout<<"selectedPatJetsPFCHSPFlow: "<< jet->pt()<<endl;  

           jetPt_.push_back(jet->pt());
           jetEta_.push_back(jet->eta());
           jetPhi_.push_back(jet->phi());
           jetM_.push_back(jet->mass());
           jetEn_.push_back(jet->energy());
           jetCharge_.push_back(jet->charge());
           jetPartonFlavor_.push_back(jet->partonFlavour()); 
           std::map<std::string, bool> Pass = jet2012ID_.MergedJetCut(*jet);
           Int_t passOrNot = PassAll(Pass); 
           jetPassID_.push_back(passOrNot);

           jetSSV_.push_back(jet->bDiscriminator("pfSimpleSecondaryVertexHighPurBJetTags"));
           jetSSVHE_.push_back(jet->bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags"));
           jetCSV_.push_back(jet->bDiscriminator("combinedSecondaryVertexBJetTags"));        
           jetTCHP_.push_back(jet->bDiscriminator("pfTrackCountingHighPurBJetTags"));
           jetTCHE_.push_back(jet->bDiscriminator("pfTrackCountingHighEffBJetTags"));
           jetJP_.push_back(jet->bDiscriminator("pfJetProbabilityBJetTags"));
           jetJBP_.push_back(jet->bDiscriminator("pfJetBProbabilityBJetTags"));
           jetCISVV2_.push_back(jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));

           jetMuEF_.push_back(jet->muonEnergyFraction());
           jetPhoEF_.push_back(jet->photonEnergyFraction());
           jetCEmEF_.push_back(jet->chargedEmEnergyFraction());
           jetCHadEF_.push_back(jet->chargedHadronEnergyFraction());
           jetNEmEF_.push_back(jet->neutralEmEnergyFraction());
           jetNHadEF_.push_back(jet->neutralHadronEnergyFraction());
           jetCMulti_.push_back(jet->chargedMultiplicity());
 

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


           jetHFHadEF_.push_back(jet->HFHadronEnergyFraction());
           jetHFEMEF_.push_back(jet->HFEMEnergyFraction());
           jetCHHadMultiplicity_.push_back(jet->chargedHadronMultiplicity());
           jetNHadMulplicity_.push_back(jet->neutralHadronMultiplicity());
           jetPhMultiplicity_.push_back(jet->photonMultiplicity());
           jetEleMultiplicity_.push_back(jet->electronMultiplicity());
           jetHFHadMultiplicity_.push_back(jet->HFHadronMultiplicity());
           jetHFEMMultiplicity_.push_back(jet->HFEMMultiplicity());
           jetChMuEF_.push_back(jet->chargedMuEnergyFraction());
           jetNMultiplicity_.push_back(jet->neutralMultiplicity());
           jetHOEnergy_.push_back(jet->hoEnergy());
           jetHOEF_.push_back(jet->hoEnergyFraction());

          
          //HBB tagger
           jet_DoubleSV_.push_back(jet->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags"));
          // cout<<"pfBoostedDoubleSecondaryVertexAK8BJetTags:"<<jet->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags")<<endl;
           


                     // fill discriminator histograms
     //     const reco::VertexCompositePtrCandidate *svTagInfo =   jet->tagInfoSecondaryVertex( svTagInfosCstr_.data());        
         
       //   const reco::Vertex *svTagInfo =   jet->tagInfoSecondaryVertex( svTagInfosCstr_.data());
       //   cout<<"numbers of 2nd vtx "<<svTagInfo->nVertices()<<endl;               
  
//         const reco::TemplatedSecondaryVertexTagInfo<reco::TrackIPTagInfo,reco::Vertex> *SecondaryVertexTagInfo=jet->tagInfoSecondaryVertex( svTagInfosCstr_.data());
  //       const reco::SecondaryVertexTagInfo &svTagInfo =*jet->tagInfoSecondaryVertex("secondaryVertex");  
    
         //cout<<" has tag info: "<<jet->hasTagInfo("pfInclusiveSecondaryVertexFinder") <<endl;
         if(jet->hasTagInfo(svTagInfosCstr_.data()))
         {
         const reco::CandSecondaryVertexTagInfo *candSVTagInfo = jet->tagInfoCandSecondaryVertex("pfInclusiveSecondaryVertexFinder");
         //const reco::SecondaryVertexTagInfo &svTagInfo =*jet->tagInfoSecondaryVertex(svTagInfosCstr_.data());
         
           cout<<"numbers of 2nd vtx "<<candSVTagInfo->nVertices()<<endl;
           
            jet_nSV_.push_back(candSVTagInfo->nVertices());  
                       

            std::vector<Float_t> jet_SVMass_float;
            std::vector<Float_t> jet_SVEnergyRatio_float;

           //     subjetMotherIndex.clear();
            jet_SVMass_float.clear();
            jet_SVEnergyRatio_float.clear();
            

            for(unsigned int n_2ndvtx=0;n_2ndvtx<candSVTagInfo->nVertices();n_2ndvtx++)
            {

              jet_SVMass_float.push_back(candSVTagInfo->secondaryVertex(n_2ndvtx).p4().mass());
              cout<<"2nd vtx mass :"<<" "<<candSVTagInfo->secondaryVertex(n_2ndvtx).p4().mass()<<endl;  

            }
            jet_SVMass_.push_back(jet_SVMass_float);
 

 
            if(candSVTagInfo->nVertices()>0)
            {
              cout<<"mass:"<< candSVTagInfo->secondaryVertex(0).p4().mass()<<endl;
              cout<<"1 th 2nd vtx x :"<<" "<<position(candSVTagInfo->secondaryVertex(0)).x()<<endl;
             
            }
            else
            {


            }



         }
         else
         {
         cout<<svTagInfosCstr_.data()<<" : "<<jet->hasTagInfo(svTagInfosCstr_.data()) <<endl;
         }


      std::vector<Float_t> subjetSDPt;
      std::vector<Float_t> subjetSDEta;
      std::vector<Float_t> subjetSDPhi;
      std::vector<Float_t> subjetSDM;
      std::vector<Float_t> subjetSDEn;
      std::vector<Int_t>   subjetSDCharge;
      std::vector<Int_t>   subjetSDPartonFlavor;
      std::vector<Float_t> subjetSDCSV; 

 //     subjetMotherIndex.clear();
      subjetSDPt.clear();
      subjetSDEta.clear();
      subjetSDPhi.clear();
      subjetSDM.clear();
      subjetSDEn.clear();
      subjetSDCharge.clear();
      subjetSDPartonFlavor.clear();
      subjetSDCSV.clear(); 


          int nSubSoftDropjets=0;

 
          auto ReSubjets  = jet->subjets("Pruned");
          cout<<"Here"<<endl; 
          for ( auto const & resub : ReSubjets ) 
          {
            cout<<"Subjets : "<<resub->pt()<<endl;
            cout<<"Subjets btag: "<<resub->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags")<<endl;

        subjetSDPt.push_back(resub->pt());
        subjetSDEta.push_back(resub->eta());
        subjetSDPhi.push_back(resub->phi());
        subjetSDM.push_back(resub->mass());
        subjetSDCharge.push_back(resub->charge());
          subjetSDCSV.push_back(resub->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));   
 

        
       subjetSDPt_.push_back(subjetSDPt); 
       subjetSDEta_.push_back(subjetSDEta);
       subjetSDPhi_.push_back(subjetSDPhi);
       subjetSDM_.push_back(subjetSDM);
      // subjetSDEn_.push_back(subjetSDEn);
       subjetSDCSV_.push_back(subjetSDCSV); 
       subjetSDCharge_.push_back(subjetSDCharge);





          nSubSoftDropjets++;

          }
nSubSDJet_.push_back(nSubSoftDropjets);
        
//         for( int sj = 0; sj < (int)subjets.size(); ++sj )
//          {
//           cout<<"Subjets : "<<subjets->pt()<<endl;
 
//          }  
               
      }//add jet loop

 }//if run addjet























//  Int_t nsubjets= pj->numberOfDaughters();

    //   nSubPrunedJet_ = pj->numberOfDaughters();

 //      cout<<"nDaughter "<<nsubjets<<endl;
 //      for(int sj=0; sj<nsubjets  ; ++sj ){
 //      cout<<"nth subj:"<<sj<<endl;
 //      const pat::Jet* subjet = dynamic_cast<const pat::Jet*>(pj->daughter(sj));
          
  
 //    	subjetPrunedPt_.push_back(subjet->pt()); 
/*  
   	subjetPrunedEta_.push_back(subjet->eta()); 
     	subjetPrunedPhi_.push_back(subjet->phi());
     	subjetPrunedM_.push_back(subjet->mass());
     	subjetPrunedEn_.push_back(subjet->energy());
     	subjetPrunedCharge_.push_back(subjet->charge());
     	subjetPrunedPartonFlavor_.push_back(subjet->partonFlavour());

        subjetPrunedSSV_.push_back(subjet->bDiscriminator("simpleSecondaryVertexHighPurBJetTags")); 
     	subjetPrunedCSV_.push_back(subjet->bDiscriminator("combinedSecondaryVertexBJetTags"     )); 
        subjetPrunedTCHP_.push_back(subjet->bDiscriminator("trackCountingHighPurBJetTags"        )); 
        subjetPrunedTCHE_.push_back(subjet->bDiscriminator("trackCountingHighEffBJetTags"        ));
        subjetPrunedJP_.push_back(subjet->bDiscriminator("jetProbabilityBJetTags"              ));
        subjetPrunedJBP_.push_back(subjet->bDiscriminator("jetBProbabilityBJetTags"             ));       
    */  
   //    }// end of loop over prunedjets
    
/*
       else // if pruned jet is not found
       {

     	jetPrunedPt_.push_back(-9999.);
     	jetPrunedEta_.push_back(-9999.);
     	jetPrunedPhi_.push_back(-9999.);
     	jetPrunedM_.push_back(-9999.);
     	jetPrunedEn_.push_back(-9999.);
     	jetPrunedCharge_.push_back(-9999);
     	jetPrunedPartonFlavor_.push_back(-9999);

     	jetPrunedCorrUncUp_.push_back(-9999.);
     	jetPrunedCorrUncDown_.push_back(-9999.);

     	jetPrunedSSV_.push_back(-9999.);
     	jetPrunedCSV_.push_back(-9999.);        
     	jetPrunedTCHP_.push_back(-9999.);
     	jetPrunedTCHE_.push_back(-9999.);
     	jetPrunedJP_.push_back(-9999.);
     	jetPrunedJBP_.push_back(-9999.);

    

       }
*/
  














  
  //  } // end of loop over jets


/* 
     for (const pat::Jet &j : *fatjets) 
     {
         std::vector<reco::Candidate const *> constituents;
         for ( unsigned ida = 0; ida < j.numberOfDaughters(); ++ida ) {
	  reco::Candidate const * cand = j.daughter(ida);
	  if ( cand->numberOfDaughters() == 0 )
	    constituents.push_back( cand ) ;
	  else {
	    for ( unsigned jda = 0; jda < cand->numberOfDaughters(); ++jda ) {
	      reco::Candidate const * cand2 = cand->daughter(jda);
	      constituents.push_back( cand2 );
	    }
	  }
	}

     }

*/
/*
     std::sort( constituents.begin(), constituents.end(), [] (reco::Candidate const * ida, reco::Candidate const * jda){return ida->pt() > jda->pt();} );

for ( unsigned int ida = 0; ida < constituents.size(); ++ida ) {
	  const pat::PackedCandidate &cand = dynamic_cast<const pat::PackedCandidate &>(*constituents[ida]);
	  printf("         constituent %3d: pt %6.2f, dz(pv) %+.3f, pdgId %+3d\n", ida,cand.pt(),cand.dz(PV.position()),cand.pdgId());
	}


	auto wSubjets = j.subjets("SoftDrop");
	for ( auto const & iw : wSubjets ) {
	  printf("   w subjet with pt %5.1f (raw pt %5.1f), eta %+4.2f, mass %5.1f ungroomed\n",
		 iw->pt(), iw->pt()*iw->jecFactor("Uncorrected"), iw->eta(), iw->mass() );
	
	}

*/

  cout<<"------------- jettree Event loop end ----------------"<<endl;




  }



void
jetTree::SetBranches(){
  
  AddBranch(&nJet_, "nJet");
  AddBranch(&jetPt_, "jetPt");
  AddBranch(&jetEta_, "jetEta");
  AddBranch(&jetPhi_, "jetPhi");
  AddBranch(&jetM_, "jetMass");
  AddBranch(&jetEn_, "jetEn");

  AddBranch(&genjetPx_,"genjetPx");
  AddBranch(&genjetPy_,"genjetPy");
  AddBranch(&genjetPz_,"genjetPz");
  AddBranch(&genjetEn_,"genjetEn");
  
  AddBranch(&genjetEM_ ,"genjetEM");
  AddBranch(&genjetHAD_ ,"genjetHAD");
  AddBranch(&genjetINV_ ,"genjetINV");
  AddBranch(&genjetAUX_ ,"genjetAUX");
  AddBranch(&matchedDR_ ,"matchedDR");
  
  AddBranch(&jetCorrUncUp_, "jetCorrUncUp");
  AddBranch(&jetCorrUncDown_, "jetCorrUncDown");
  AddBranch(&jetCharge_, "jetCharge");
  AddBranch(&jetPartonFlavor_, "jetPartonFlavor");
  AddBranch(&jetPassID_, "jetPassID");


  AddBranch(&jet_nSV_, "jet_nSV");
  AddBranch(&jet_SVMass_, "jet_SVMass");
  AddBranch(&jet_SVEnergyRatio_, "jet_SVEnergyRatio");









  AddBranch(&jetSSV_, "jetSSV");
  AddBranch(&jetSSVHE_,"jetSSVHE");
  AddBranch(&jetCSV_, "jetCSV");        
  AddBranch(&jetCISVV2_,"jetCISVV2");
  AddBranch(&jetTCHP_, "jetTCHP");
  AddBranch(&jetTCHE_, "jetTCHE");
  AddBranch(&jetJP_, "jetJP");
  AddBranch(&jetJBP_, "jetJBP");

  AddBranch(&jetTau1_, "jetTau1");
  AddBranch(&jetTau2_, "jetTau2");
  AddBranch(&jetTau3_, "jetTau3");
  AddBranch(&jetTau4_, "jetTau4");

  AddBranch(&jetMuEF_, "jetMuEF");
  AddBranch(&jetPhoEF_, "jetPhoEF");
  AddBranch(&jetCEmEF_, "jetCEmEF");
  AddBranch(&jetCHadEF_, "jetCHadEF");
  AddBranch(&jetNEmEF_, "jetNEmEF");
  AddBranch(&jetNHadEF_, "jetNHadEF");
  AddBranch(&jetCMulti_, "jetCMulti");
  
  if(isFATJet_||isADDJet_){
    AddBranch(&jetPrunedPt_, "jetPrunedPt");
    AddBranch(&jetPrunedEta_, "jetPrunedEta");
    AddBranch(&jetPrunedPhi_, "jetPrunedPhi");
    AddBranch(&jetPrunedM_, "jetPrunedMass");
    AddBranch(&jetPrunedEn_, "jetPrunedEn");
    AddBranch(&jetPrunedCorrUncUp_, "jetPrunedCorrUncUp");
    AddBranch(&jetPrunedCorrUncDown_, "jetPrunedCorrUncDown");
    AddBranch(&jetPrunedCharge_, "jetPrunedCharge");
    AddBranch(&jetPrunedPartonFlavor_, "jetPrunedPartonFlavor");


    AddBranch(&jetPrunedSSV_, "jetPrunedSSV");
    AddBranch(&jetPrunedCSV_, "jetPrunedCSV");        
    AddBranch(&jetPrunedTCHP_, "jetPrunedTCHP");
    AddBranch(&jetPrunedTCHE_, "jetPrunedTCHE");
    AddBranch(&jetPrunedJP_, "jetPrunedJP");
    AddBranch(&jetPrunedJBP_, "jetPrunedJBP");


    AddBranch(&jetSDmass_, "jetSDmass");
    AddBranch(&jetTRmass_, "jetTRmass");
    AddBranch(&jetPRmass_, "jetPRmass");
    AddBranch(&jetFimass_, "jetFimass");

    AddBranch(&jet_DoubleSV_,"jet_DoubleSV");



  AddBranch(&nSubSDJet_,"nSubSDJet");
  AddBranch(&subjetSDPt_, "subjetSDPt");
  AddBranch(&subjetSDEta_, "subjetSDEta");
  AddBranch(&subjetSDPhi_, "subjetSDPhi");
  AddBranch(&subjetSDM_, "subjetSDMass");
//  AddBranch(&subjetSDEn_, "subjetSDEn");
//  AddBranch(&subjetSDCharge_, "subjetSDCharge");
//  AddBranch(&subjetSDPartonFlavor_, "subjetSDPartonFlavor");
  AddBranch(&subjetSDCSV_, "subjetSDCSV");     




/*	    
  AddBranch(&nSubPrunedJet_,"nSubPrunedJet");
  
  AddBranch(&subjetPrunedPt_, "subjetPrunedPt");
  AddBranch(&subjetPrunedEta_, "subjetPrunedEta");
  AddBranch(&subjetPrunedPhi_, "subjetPrunedPhi");
  AddBranch(&subjetPrunedM_, "subjetPrunedMass");
  AddBranch(&subjetPrunedEn_, "subjetPrunedEn");
  AddBranch(&subjetPrunedCharge_, "subjetPrunedCharge");
  AddBranch(&subjetPrunedPartonFlavor_, "subjetPrunedPartonFlavor");
*/

//   AddBranch(&subjetPrunedSSV_, "subjetPrunedSSV");
//  AddBranch(&subjetPrunedCSV_, "subjetPrunedCSV");        
//   AddBranch(&subjetPrunedTCHP_, "subjetPrunedTCHP");
//   AddBranch(&subjetPrunedTCHE_, "subjetPrunedTCHE");
//   AddBranch(&subjetPrunedJP_, "subjetPrunedJP");
//   AddBranch(&subjetPrunedJBP_, "subjetPrunedJBP");
  
  }

}


void
jetTree::Clear(){
  nJet_ = 0;
  jetPt_.clear();
  jetEta_.clear();
  jetPhi_.clear();
  jetM_.clear();
  jetEn_.clear();
  jetCorrUncUp_.clear();
  jetCorrUncDown_.clear();
  jetCharge_.clear();
  jetPartonFlavor_.clear();
  jetPassID_.clear();


  genjetPx_.clear();
  genjetPy_.clear();
  genjetPz_.clear();
  genjetEn_.clear();
  
  genjetEM_.clear();
  genjetHAD_.clear();
  genjetINV_.clear();
  genjetAUX_.clear();
  matchedDR_.clear();



  jet_nSV_.clear();
  jet_SVMass_.clear();  
  jet_SVEnergyRatio_.clear();
  









  jetSSV_.clear();
  jetSSVHE_.clear();
  jetCSV_.clear();        
  jetCISVV2_.clear();        
  jetTCHP_.clear();
  jetTCHE_.clear();
  jetJP_.clear();
  jetJBP_.clear();

  jetTau1_.clear();
  jetTau2_.clear();
  jetTau3_.clear();
  jetTau4_.clear();

  jetMuEF_.clear();
  jetPhoEF_.clear();
  jetCEmEF_.clear();
  jetCHadEF_.clear();
  jetNEmEF_.clear();
  jetNHadEF_.clear();
  jetCMulti_.clear();

  jetPrunedPt_.clear();
  jetPrunedEta_.clear();
  jetPrunedPhi_.clear();
  jetPrunedM_.clear();
  jetPrunedEn_.clear();
  jetPrunedCorrUncUp_.clear();
  jetPrunedCorrUncDown_.clear();
  jetPrunedCharge_.clear();
  jetPrunedPartonFlavor_.clear();


  jetPrunedSSV_.clear();
  jetPrunedCSV_.clear();        
  jetPrunedTCHP_.clear();
  jetPrunedTCHE_.clear();
  jetPrunedJP_.clear();
  jetPrunedJBP_.clear();

  jetSDmass_.clear();
  jetTRmass_.clear();
  jetPRmass_.clear();
  jetFimass_.clear();  


  jet_DoubleSV_.clear();









  nSubSDJet_.clear();  
//  subjetMotherIndex_.clear();
  subjetSDPt_.clear();
  subjetSDEta_.clear();
  subjetSDPhi_.clear();
  subjetSDM_.clear();
  subjetSDEn_.clear();
  subjetSDCharge_.clear();
  subjetSDPartonFlavor_.clear();
  subjetSDCSV_.clear();        
















 /* 
  subjetPrunedPt_.clear();
  subjetPrunedEta_.clear();
  subjetPrunedPhi_.clear();
  subjetPrunedM_.clear();
  subjetPrunedEn_.clear();
  subjetPrunedCharge_.clear();
  subjetPrunedPartonFlavor_.clear();


   subjetPrunedSSV_.clear();
  subjetPrunedCSV_.clear();        
   subjetPrunedTCHP_.clear();
   subjetPrunedTCHE_.clear();
   subjetPrunedJP_.clear();
   subjetPrunedJBP_.clear();
  
*/

}
