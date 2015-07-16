 
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
  isSpring15_(iConfig.getParameter<bool>("isSpring15")),
  jet2012ID_()
{
  
  std::size_t found = desc.find("FAT");
  if (found!=std::string::npos)
    isFATJet_=true;
  else
    isFATJet_=false;
  if(strcmp(desc.data(),"ADD")==0) isADDJet_=true;
  else isADDJet_=false; 
  runAddJet_ = iConfig.getParameter<bool>("runAddjetPY");

  jetP4_       = new TClonesArray("TLorentzVector");
  jetPrunedP4_ = new TClonesArray("TLorentzVector");
  genjetP4_    = new TClonesArray("TLorentzVector");
  SetBranches();



}


jetTree::~jetTree(){

  delete jetP4_;
  delete jetPrunedP4_;
  delete genjetP4_;

}


void
jetTree::Fill(const edm::Event& iEvent, edm::EventSetup const& iSetup){
  Clear();


  // cout<<isFATJet_<<"----------------- jet Event loop start------------------"<<endl;
  // cout<<"mini AOD Lable:"<<JetLabel_<<"............................................"<<endl; 
  // cout<<"add jet  Lable:"<<AddjetlabelC_<<"............................................"<<endl;  
 


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
  
  
     

  pat::JetCollection jets(*(JetHandle.product()));
  std::sort(jets.begin(),jets.end(),PtGreater());

  std::vector<pat::Jet>::const_iterator jet =jets.begin();   
  // cout<<"start Fatjet loop"<<endl; 

  for(;jet!=jets.end();jet++){
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
      genjetEM_.push_back(-999.0);
      genjetHAD_.push_back(-999.0);
      genjetINV_.push_back(-999.0);
      genjetAUX_.push_back(-999.0);
      matchedDR_.push_back(-999.0);
      new( (*genjetP4_)[nJet_-1]) TLorentzVector(0,0,0,0);
    }

    jetRawFactor_.push_back(jet->jecFactor("Uncorrected"));
    new( (*jetP4_)[nJet_-1]) TLorentzVector(jet->p4().px(),
					    jet->p4().py(),
					    jet->p4().pz(),
					    jet->p4().energy());

    jetCharge_.push_back(jet->charge());
    jetPartonFlavor_.push_back(jet->partonFlavour());



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
    
    bool passPU = false;
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
    jetCSV_.push_back(jet->bDiscriminator("combinedSecondaryVertexBJetTags"));      jetSSVHE_.push_back(jet->bDiscriminator("pfSimpleSecondaryVertexHighEffBJetTags"));      
    jetCISVV2_.push_back(jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
    jetTCHP_.push_back(jet->bDiscriminator("pfTrackCountingHighPurBJetTags"));
    jetTCHE_.push_back(jet->bDiscriminator("pfTrackCountingHighEffBJetTags"));
    jetJP_.push_back(jet->bDiscriminator("pfJetProbabilityBJetTags"));
    jetJBP_.push_back(jet->bDiscriminator("pfJetBProbabilityBJetTags"));

    jetTau1_.push_back(jet->userFloat("NjettinessAK8:tau1"));
    jetTau2_.push_back(jet->userFloat("NjettinessAK8:tau2"));
    jetTau3_.push_back(jet->userFloat("NjettinessAK8:tau3"));
    jetTau4_.push_back(jet->userFloat("NjettinessAK8:tau2")/jet->userFloat("NjettinessAK8:tau1"));
    
    


    if(isFATJet_ || (runAddJet_&&isADDJet_)) // only run with FATjet
      {

	if(isFATJet_){
	  jetSDmass_.push_back(jet->userFloat("ak8PFJetsCHSSoftDropMass"));
	  jetTRmass_.push_back(jet->userFloat("ak8PFJetsCHSTrimmedMass")); 
	  jetPRmass_.push_back(jet->userFloat("ak8PFJetsCHSPrunedMass"));
	  jetFimass_.push_back(jet->userFloat("ak8PFJetsCHSFilteredMass"));
	}

	
	if(runAddJet_ && isADDJet_){
          //HBB tagger
	  jet_DoubleSV_.push_back(jet->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags"));
           


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
         
	      //           cout<<"numbers of 2nd vtx "<<candSVTagInfo->nVertices()<<endl;
           
	      jet_nSV_.push_back(candSVTagInfo->nVertices());  
                       

	      std::vector<float> jet_SVMass_float;
	      std::vector<float> jet_SVEnergyRatio_float;

	      jet_SVMass_float.clear();
	      jet_SVEnergyRatio_float.clear();
            

	      for(unsigned int n_2ndvtx=0;n_2ndvtx<candSVTagInfo->nVertices();n_2ndvtx++)
		{

		  jet_SVMass_float.push_back(candSVTagInfo->secondaryVertex(n_2ndvtx).p4().mass());
		  //            cout<<"2nd vtx mass :"<<" "<<candSVTagInfo->secondaryVertex(n_2ndvtx).p4().mass()<<endl;  

		}
	      jet_SVMass_.push_back(jet_SVMass_float);
 
	    }
	} /// if its ADDJET
 
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
	
	if(isSpring15_) { // this is commented because subjet doesn't work with PHYS14 samples.
	  auto wSubjets = jetptr->subjets("SoftDrop");
	  //	std::cout<<" working after SoftDrop "<<std::endl;
	  int nSubSoftDropjets=0;	
	
	  std::vector<float> subjetSDPx; 
	  std::vector<float> subjetSDPy; 
	  std::vector<float> subjetSDPz; 
	  std::vector<float> subjetSDE; 	
	  std::vector<int>   subjetSDCharge;
	  // std::vector<int>   subjetSDPartonFlavor;
	  std::vector<float> subjetSDCSV; 

	  subjetSDPx.clear();
	  subjetSDPy.clear();
	  subjetSDPz.clear();
	  subjetSDE.clear();
	  subjetSDCharge.clear();
	  // subjetSDPartonFlavor.clear();
	  subjetSDCSV.clear(); 


	  for ( auto const & iw : wSubjets ) 
	    {

	      nSubSoftDropjets++;
	      
	      subjetSDPx.push_back(iw->px());
	      subjetSDPy.push_back(iw->py());
	      subjetSDPz.push_back(iw->pz());
	      subjetSDE.push_back(iw->energy());	
	      subjetSDCharge.push_back(iw->charge());
	      subjetSDCSV.push_back(iw->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));   

	    }//subjet loop
	  nSubSDJet_.push_back(nSubSoftDropjets); 
	  subjetSDPx_.push_back(subjetSDPx);
	  subjetSDPy_.push_back(subjetSDPy);
	  subjetSDPz_.push_back(subjetSDPz);
	  subjetSDE_.push_back(subjetSDE);
	  subjetSDCharge_.push_back(subjetSDCharge);
	  subjetSDCSV_.push_back(subjetSDCSV); 
       
	  
	} //ifSPRIN15

      }//if is Fat jet or add jets


  }//jet loop

  



}



void
jetTree::SetBranches(){
  
  AddBranch(&nJet_, "nJet");

  AddBranch(&genjetP4_,"genjetP4");
  AddBranch(&genjetEM_ ,"genjetEM");
  AddBranch(&genjetHAD_ ,"genjetHAD");
  AddBranch(&genjetINV_ ,"genjetINV");
  AddBranch(&genjetAUX_ ,"genjetAUX");
  AddBranch(&matchedDR_ ,"matchedDR");

  AddBranch(&jetRawFactor_,"jetRawFactor");

  AddBranch(&jetP4_, "jetP4");

  AddBranch(&jetCharge_, "jetCharge");
  AddBranch(&jetPartonFlavor_, "jetPartonFlavor");
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

  AddBranch(&jet_nSV_, "jet_nSV");
  AddBranch(&jet_SVMass_, "jet_SVMass");
  AddBranch(&jet_SVEnergyRatio_, "jet_SVEnergyRatio");


  AddBranch(&jetTau1_, "jetTau1");
  AddBranch(&jetTau2_, "jetTau2");
  AddBranch(&jetTau3_, "jetTau3");
  AddBranch(&jetTau4_, "jetTau4");



  
  if(isFATJet_){
    AddBranch(&jetSDmass_, "jetSDmass");
    AddBranch(&jetTRmass_, "jetTRmass");
    AddBranch(&jetPRmass_, "jetPRmass");
    AddBranch(&jetFimass_, "jetFimass");
  }


  if(runAddJet_&&isADDJet_)
    AddBranch(&jet_DoubleSV_,"jet_DoubleSV");

  if(isFATJet_ || (runAddJet_&&isADDJet_))
    {
      AddBranch(&nSubSDJet_,"nSubSDJet");
      AddBranch(&subjetSDPx_, "subjetSDPx");     
      AddBranch(&subjetSDPy_, "subjetSDPy");     
      AddBranch(&subjetSDPz_, "subjetSDPz");     
      AddBranch(&subjetSDE_, "subjetSDCE");     
      AddBranch(&subjetSDCSV_, "subjetSDCSV");     

    }
  
  

}


void
jetTree::Clear(){


  nJet_ =0;

  genjetP4_->Clear();
  genjetEM_.clear();
  genjetHAD_.clear();
  genjetINV_.clear();
  genjetAUX_.clear();
  matchedDR_.clear();

  jetRawFactor_.clear();

  jetP4_->Clear();

  jetCorrUncUp_.clear();
  jetCorrUncDown_.clear();
  jetCharge_.clear();
  jetPartonFlavor_.clear();
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

  //jet secondary vtx

  jet_nSV_.clear();
  jet_SVMass_.clear();
  jet_SVEnergyRatio_.clear();



  jetTau1_.clear();
  jetTau2_.clear();
  jetTau3_.clear();
  jetTau4_.clear();




  // pruned information

  jetPrunedP4_->Clear();
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

  //ak8jet mass
 
  jetSDmass_.clear(); 
  jetTRmass_.clear();
  jetPRmass_.clear();
  jetFimass_.clear();
  

  //jet  Hbb tagger for fat and add jet

  jet_DoubleSV_.clear();


  // subjet of jets

  nSubSDJet_.clear();
  subjetSDPx_.clear();
  subjetSDPy_.clear();
  subjetSDPz_.clear();
  subjetSDE_.clear();
  subjetSDCharge_.clear();
  subjetSDPartonFlavor_.clear();
  subjetSDCSV_.clear();        



}
