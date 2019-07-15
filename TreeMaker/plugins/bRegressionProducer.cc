#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
//#include "DelPanj/TreeMaker/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
//#include "DelPanj/TreeMaker/interface/DiPhotonCandidate_dublicate.h"
#include "RecoJets/JetProducers/interface/PileupJetIdAlgo.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
//#include "DelPanj/TreeMaker/interface/VertexCandidateMap.h"
//#include "flashgg/DataFormats/interface/Jet_extra.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "DelPanj/TreeMaker/interface/jetTree.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/Math/interface/deltaR.h"


#include <iostream>
#include <string>
#include <vector>

#include "DNN/Tensorflow/interface/Graph.h"
#include "DNN/Tensorflow/interface/Tensor.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"

#define debug 0

using namespace std;
using namespace edm;

//namespace flashgg {

  class bRegressionProducer : public edm::EDProducer
  {

  public:
      bRegressionProducer( const ParameterSet & );
      ~bRegressionProducer(){};
      void InitJet();
      void SetNNVectorVar();
      std::vector<float> EvaluateNN();
  private:
      void produce( Event &, const EventSetup & ) override;
      //        std::vector<edm::InputTag> inputTagJets_;
      edm::InputTag inputTagJets_;
      EDGetTokenT<View<pat::Jet> > jetToken_;
      //EDGetTokenT<pat::JetCollection> jetToken_;
      edm::EDGetTokenT<double> rhoToken_;
      //string bRegressionWeightfileName_;
      double y_mean_,y_std_;

      dnn::tf::Graph NNgraph_;
      std::vector<float> NNvectorVar_;
      //add vector of mva for eache jet

      //mva variables
      float Jet_pt ;
      float Jet_eta ;
      float rho ;
      float Jet_mt ;
      float Jet_leadTrackPt ;
      float Jet_leptonPtRel ;
      float Jet_leptonDeltaR ;
      float Jet_neHEF ;
      float Jet_neEmEF ;
      float Jet_vtxPt ;
      float Jet_vtxMass ;
      float Jet_vtx3dL ;
      float Jet_vtxNtrk ;
      float Jet_vtx3deL ;
      float Jet_numDaughters_pt03 ;
      float Jet_energyRing_dR0_em_Jet_e ;
      float Jet_energyRing_dR1_em_Jet_e ;
      float Jet_energyRing_dR2_em_Jet_e ;
      float Jet_energyRing_dR3_em_Jet_e ;
      float Jet_energyRing_dR4_em_Jet_e ;
      float Jet_energyRing_dR0_neut_Jet_e ;
      float Jet_energyRing_dR1_neut_Jet_e ;
      float Jet_energyRing_dR2_neut_Jet_e ;
      float Jet_energyRing_dR3_neut_Jet_e ;
      float Jet_energyRing_dR4_neut_Jet_e ;
      float Jet_energyRing_dR0_ch_Jet_e ;
      float Jet_energyRing_dR1_ch_Jet_e ;
      float Jet_energyRing_dR2_ch_Jet_e ;
      float Jet_energyRing_dR3_ch_Jet_e ;
      float Jet_energyRing_dR4_ch_Jet_e ;
      float Jet_energyRing_dR0_mu_Jet_e ;
      float Jet_energyRing_dR1_mu_Jet_e ;
      float Jet_energyRing_dR2_mu_Jet_e ;
      float Jet_energyRing_dR3_mu_Jet_e ;
      float Jet_energyRing_dR4_mu_Jet_e ;
      float Jet_chHEF;//implement from here
      float Jet_chEmEF;
      float Jet_leptonPtRelInv;
      int isEle;
      int isMu;
      int isOther;
      float Jet_mass;
      float Jet_withPtd;


  };


  bRegressionProducer::bRegressionProducer( const ParameterSet &iConfig ) :
      //     inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "JetTag" )) {
      inputTagJets_( iConfig.getParameter<edm::InputTag>( "JetTag" )) ,
      rhoToken_( consumes<double>(iConfig.getParameter<edm::InputTag>( "rhoFixedGridCollection" ) ) ),
     // bRegressionWeightfileName_( iConfig.getUntrackedParameter<std::string>("bRegressionWeightfile")),
      y_mean_(iConfig.getUntrackedParameter<double>("y_mean")),
      y_std_(iConfig.getUntrackedParameter<double>("y_std"))
  {
      jetToken_= consumes<View<pat::Jet> >(inputTagJets_);
      //jetToken_= consumes<pat::JetCollection>(inputTagJets_);

      std::string cmssw_base = getenv("CMSSW_BASE");
      std::string bRegressionWeightfileName_ = cmssw_base+"/src/DelPanj/TreeMaker/data/model-18";

      NNgraph_ = *(new dnn::tf::Graph(bRegressionWeightfileName_.c_str())); //FIXME make this configurable, for variables for breg check this PR https://github.com/cms-analysis/flashgg/pull/968 REMEMBER TO ADD THE LAST CONE!
      Jet_pt = 0.;
      Jet_eta = 0.;
      rho = 0.;
      Jet_mt = 0.;
      Jet_leadTrackPt = 0.;
      Jet_leptonPtRel = 0.;
      Jet_leptonDeltaR = 0.;
      Jet_neHEF = 0.;
      Jet_neEmEF = 0.;
      Jet_vtxPt = 0.;
      Jet_vtxMass = 0.;
      Jet_vtx3dL = 0.;
      Jet_vtxNtrk = 0.;
      Jet_vtx3deL = 0.;
      Jet_numDaughters_pt03 = 0;
      Jet_energyRing_dR0_em_Jet_e = 0.;
      Jet_energyRing_dR1_em_Jet_e = 0.;
      Jet_energyRing_dR2_em_Jet_e = 0.;
      Jet_energyRing_dR3_em_Jet_e = 0.;
      Jet_energyRing_dR4_em_Jet_e = 0.;
      Jet_energyRing_dR0_neut_Jet_e = 0.;
      Jet_energyRing_dR1_neut_Jet_e = 0.;
      Jet_energyRing_dR2_neut_Jet_e = 0.;
      Jet_energyRing_dR3_neut_Jet_e = 0.;
      Jet_energyRing_dR4_neut_Jet_e = 0.;
      Jet_energyRing_dR0_ch_Jet_e = 0.;
      Jet_energyRing_dR1_ch_Jet_e = 0.;
      Jet_energyRing_dR2_ch_Jet_e = 0.;
      Jet_energyRing_dR3_ch_Jet_e = 0.;
      Jet_energyRing_dR4_ch_Jet_e = 0.;
      Jet_energyRing_dR0_mu_Jet_e = 0.;
      Jet_energyRing_dR1_mu_Jet_e = 0.;
      Jet_energyRing_dR2_mu_Jet_e = 0.;
      Jet_energyRing_dR3_mu_Jet_e = 0.;
      Jet_energyRing_dR4_mu_Jet_e = 0.;
      Jet_chHEF = 0.;//implement from here
      Jet_chEmEF = 0.;
      Jet_leptonPtRelInv = 0.;
      isEle = 0.;
      isMu = 0.;
      isOther = 0.;
      Jet_mass = 0.;
      Jet_withPtd = 0.;

      produces<vector<pat::Jet> > ();
  }



  void bRegressionProducer::produce( Event &evt, const EventSetup & )
  {
      // input jets
      Handle<View<pat::Jet> > jets;
      //Handle<pat::JetCollection> jets;
      evt.getByToken( jetToken_, jets );//just to try get the first one
      unique_ptr<vector<pat::Jet> > jetColl( new vector<pat::Jet> );
      //unique_ptr<vector<pat::JetCollection> > jetColl( new vector<pat::JetCollection> );
      for( unsigned int i = 0 ; i < jets->size() ; i++ ) {

          InitJet();

          Ptr<pat::Jet> pjet = jets->ptrAt( i );
          //Ptr<pat::JetCollection> pjet = jets->ptrAt( i );
          pat::Jet fjet = pat::Jet( *pjet );

          //            if (fjet.pt()<15. || fabs(fjet.eta())>2.5) continue;


          //variables needed for regression
          //you need to take uncorrected jet for variables
          Jet_pt = fjet.correctedJet("Uncorrected").pt() ;
          Jet_eta = fjet.eta();
          //Jet_leadTrackPt = leadTrackPt;//fjet.userFloat("leadTrackPt");
          edm::Handle<double> rhoHandle;
          evt.getByToken( rhoToken_, rhoHandle );
          const double rhoFixedGrd = *( rhoHandle.product() );
          rho = rhoFixedGrd;
          Jet_mt = sqrt(fjet.correctedJet("Uncorrected").energy()*fjet.correctedJet("Uncorrected").energy()-fjet.correctedJet("Uncorrected").pz()*fjet.correctedJet("Uncorrected").pz());

         // this part we are taking from jetProducer file

          float ptD=0.;
          //int nSecVertices = 0;
          float sumWeight=0;
          float sumPt=0;
          for(const auto & d : pjet->daughterPtrVector()){
              sumWeight+=(d->pt())*(d->pt());
              sumPt+=d->pt();
          }
          ptD = (sumWeight > 0 ? sqrt(sumWeight)/sumPt : 0);

         float leadTrackPt_ = 0, softLepPt = 0, softLepRatio = 0, softLepDr = 0;
         float sumPtDrSq = 0.;
         float sumPtSq = 0.;
         float softLepPtRel = 0.;
         float softLepPtRelInv=0.;


         float cone_boundaries[] = { 0.05, 0.1, 0.2, 0.3, 0.4 }; // hardcoded boundaries: should be made configurable
         size_t ncone_boundaries = sizeof(cone_boundaries)/sizeof(float);
         std::vector<float> chEnergies(ncone_boundaries+1,0.);
         std::vector<float> emEnergies(ncone_boundaries+1,0.);
         std::vector<float> neEnergies(ncone_boundaries+1,0.);
         std::vector<float> muEnergies(ncone_boundaries+1,0.);
         int numDaug03 = 0;

         int softLepPdgId=0;


         for ( unsigned k = 0; k < fjet.numberOfSourceCandidatePtrs(); ++k ) {
             reco::CandidatePtr pfJetConstituent = fjet.sourceCandidatePtr(k);

             const reco::Candidate* kcand = pfJetConstituent.get();
             const pat::PackedCandidate* lPack = dynamic_cast<const pat::PackedCandidate *>( kcand );
             if ( !lPack ) throw cms::Exception( "NoPackedConstituent" ) << " For jet " << i << " failed to get constituent " << k << std::endl;
             float candPt = kcand->pt();
             float candDr   = reco::deltaR(*kcand,fjet);
             sumPtDrSq += candPt*candPt*candDr*candDr;
             sumPtSq += candPt*candPt;

             if( candPt > 0.3 ) { ++numDaug03; }
             if(lPack->charge() != 0 && candPt > leadTrackPt_) leadTrackPt_ = candPt;

             if(abs(lPack->pdgId()) == 11 || abs(lPack->pdgId()) == 13) {
                 if(candPt > softLepPt){
                     softLepPt = candPt;
                     softLepRatio = candPt/pjet->pt();
                     if (0){std::cout << "softLepRatio"  << softLepRatio << std::endl;}
                     softLepDr = candDr;
                     softLepPtRel = ( pjet->px()*lPack->px() + pjet->py()*lPack->py() + pjet->pz()*lPack->pz() ) / pjet->p();
                     softLepPtRel = sqrt( lPack->p()*lPack->p() - softLepPtRel*softLepPtRel );

                     softLepPtRelInv = ( pjet->px()*lPack->px() + pjet->py()*lPack->py() + pjet->pz()*lPack->pz() ) / lPack->p();
                     softLepPtRelInv = sqrt( pjet->p()*pjet->p() - softLepPtRelInv*softLepPtRelInv );

                     softLepPdgId = lPack->pdgId();
                 }
             }

             int pdgid = abs(lPack->pdgId());
             size_t icone = std::lower_bound(&cone_boundaries[0],&cone_boundaries[ncone_boundaries],candDr) - &cone_boundaries[0];
             float candEnergy = kcand->energy();
             // std::cout << "pdgId " << pdgid << " candDr " << candDr << " icone " << icone << " " << candEnergy << std::endl;
             if( pdgid == 22 || pdgid == 11 ) {
                 // std::cout << " fill EM" << std::endl;
                 emEnergies[icone] += candEnergy;
             } else if ( pdgid == 13 ) {
                 // std::cout << " fill mu" << std::endl;
                 muEnergies[icone] += candEnergy;
             } else if ( lPack-> charge() != 0 ) {
                 // std::cout << " fill ch" << std::endl;
                 chEnergies[icone] += candEnergy;
             } else {
                 // std::cout << " fill ne" << std::endl;
                 neEnergies[icone] += candEnergy;
             }
         }



          //this max probably not needed, it's just heppy
          Jet_leadTrackPt = leadTrackPt_;
          Jet_leptonPtRel = softLepPtRel;  //std::max(float(0.),fjet.userFloat("softLepPtRel"));
          Jet_leptonDeltaR = softLepDr;  //std::max(float(0.),fjet.userFloat("softLepDr"));
          Jet_neHEF = fjet.neutralHadronEnergyFraction();
          Jet_neEmEF = fjet.neutralEmEnergyFraction();
          Jet_chHEF = fjet.chargedHadronEnergyFraction();
          Jet_chEmEF = fjet.chargedEmEnergyFraction();
          Jet_leptonPtRelInv = softLepPtRelInv*Jet_pt/fjet.pt();  //fjet.userFloat("softLepPtRelInv")*Jet_pt/fjet.pt();

          int lepPdgID = softLepPdgId; //fjet.userInt("softLepPdgId");
          if (abs(lepPdgID)==13){
              isMu=1;
          }else if (abs(lepPdgID)==11){
              isEle=1;
          }else{
              isOther=1;
          }
          Jet_mass=fjet.correctedJet("Uncorrected").mass();
          Jet_withPtd=ptD;//fjet.userFloat("ptD");

          //if(fjet.userFloat("nSecVertices")>0){
//                float vertexX=fjet.userFloat("vtxPosX")-fjet.userFloat("vtxPx");//check if it's correct
//                float vertexY=fjet.userFloat("vtxPosY")-fjet.userFloat("vtxPy");
//                Jet_vtxPt = sqrt(vertexX*vertexX+vertexY*vertexY);
          Jet_vtxPt=sqrt(fjet.userFloat("vtxPx")*fjet.userFloat("vtxPx")+fjet.userFloat("vtxPy")*fjet.userFloat("vtxPy"));
          Jet_vtxMass = std::max(float(0.),fjet.userFloat("vtxMass"));
          Jet_vtx3dL = std::max(float(0.),fjet.userFloat("vtx3DVal"));
          Jet_vtxNtrk = std::max(float(0.),fjet.userFloat("vtxNtracks"));
          Jet_vtx3deL = std::max(float(0.),fjet.userFloat("vtx3DSig"));
          //}
          //if (fjet.emEnergies().size()>0){//since in order to save space we save this info only if the candidate has a minimum pt or eta
          Jet_energyRing_dR0_em_Jet_e = emEnergies[0]/fjet.correctedJet("Uncorrected").energy();//remember to divide by jet energy
          Jet_energyRing_dR1_em_Jet_e = emEnergies[1]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR2_em_Jet_e = emEnergies[2]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR3_em_Jet_e = emEnergies[3]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR4_em_Jet_e = emEnergies[4]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR0_neut_Jet_e = neEnergies[0]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR1_neut_Jet_e = neEnergies[1]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR2_neut_Jet_e = neEnergies[2]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR3_neut_Jet_e = neEnergies[3]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR4_neut_Jet_e = neEnergies[4]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR0_ch_Jet_e = chEnergies[0]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR1_ch_Jet_e = chEnergies[1]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR2_ch_Jet_e = chEnergies[2]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR3_ch_Jet_e = chEnergies[3]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR4_ch_Jet_e = chEnergies[4]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR0_mu_Jet_e = muEnergies[0]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR1_mu_Jet_e = muEnergies[1]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR2_mu_Jet_e = muEnergies[2]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR3_mu_Jet_e = muEnergies[3]/fjet.correctedJet("Uncorrected").energy();
          Jet_energyRing_dR4_mu_Jet_e = muEnergies[4]/fjet.correctedJet("Uncorrected").energy();
          //}
          Jet_numDaughters_pt03 = numDaug03;//fjet.userInt("numDaug03");

          std::vector<float> bRegNN(3,-999);


          if(debug){
              cout<<"Jet_pt :"<<Jet_pt <<endl;
              cout<<"Jet_eta :"<<Jet_eta <<endl;
              cout<<"rho :"<<rho <<endl;
              cout<<"Jet_mt :"<<Jet_mt <<endl;
              cout<<"Jet_leadTrackPt :"<<Jet_leadTrackPt <<endl;
              cout<<"Jet_leptonPtRel :"<<Jet_leptonPtRel <<endl;
              cout<<"Jet_leptonDeltaR :"<<Jet_leptonDeltaR <<endl;
              cout<<"Jet_neHEF :"<<Jet_neHEF <<endl;
              cout<<"Jet_neEmEF :"<<Jet_neEmEF <<endl;
              cout<<"Jet_vtxPt :"<<Jet_vtxPt <<endl;
              cout<<"Jet_vtxMass :"<<Jet_vtxMass <<endl;
              cout<<"Jet_vtx3dL :"<<Jet_vtx3dL <<endl;
              cout<<"Jet_vtxNtrk :"<<Jet_vtxNtrk <<endl;
              cout<<"Jet_vtx3deL :"<<Jet_vtx3deL <<endl;
              cout<<"Jet_energyRing_dR0_em_Jet_e :"<<Jet_energyRing_dR0_em_Jet_e <<endl;
              cout<<"Jet_energyRing_dR1_em_Jet_e :"<<Jet_energyRing_dR1_em_Jet_e <<endl;
              cout<<"Jet_energyRing_dR2_em_Jet_e :"<<Jet_energyRing_dR2_em_Jet_e <<endl;
              cout<<"Jet_energyRing_dR3_em_Jet_e :"<<Jet_energyRing_dR3_em_Jet_e <<endl;
              cout<<"Jet_energyRing_dR4_em_Jet_e :"<<Jet_energyRing_dR4_em_Jet_e <<endl;
              cout<<"Jet_energyRing_dR0_neut_Jet_e :"<<Jet_energyRing_dR0_neut_Jet_e <<endl;
              cout<<"Jet_energyRing_dR1_neut_Jet_e :"<<Jet_energyRing_dR1_neut_Jet_e <<endl;
              cout<<"Jet_energyRing_dR2_neut_Jet_e :"<<Jet_energyRing_dR2_neut_Jet_e <<endl;
              cout<<"Jet_energyRing_dR3_neut_Jet_e :"<<Jet_energyRing_dR3_neut_Jet_e <<endl;
              cout<<"Jet_energyRing_dR4_neut_Jet_e :"<<Jet_energyRing_dR4_neut_Jet_e <<endl;
              cout<<"Jet_energyRing_dR0_ch_Jet_e :"<<Jet_energyRing_dR0_ch_Jet_e <<endl;
              cout<<"Jet_energyRing_dR1_ch_Jet_e :"<<Jet_energyRing_dR1_ch_Jet_e <<endl;
              cout<<"Jet_energyRing_dR2_ch_Jet_e :"<<Jet_energyRing_dR2_ch_Jet_e <<endl;
              cout<<"Jet_energyRing_dR3_ch_Jet_e :"<<Jet_energyRing_dR3_ch_Jet_e <<endl;
              cout<<"Jet_energyRing_dR4_ch_Jet_e :"<<Jet_energyRing_dR4_ch_Jet_e <<endl;
              cout<<"Jet_energyRing_dR0_mu_Jet_e :"<<Jet_energyRing_dR0_mu_Jet_e <<endl;
              cout<<"Jet_energyRing_dR1_mu_Jet_e :"<<Jet_energyRing_dR1_mu_Jet_e <<endl;
              cout<<"Jet_energyRing_dR2_mu_Jet_e :"<<Jet_energyRing_dR2_mu_Jet_e <<endl;
              cout<<"Jet_energyRing_dR3_mu_Jet_e :"<<Jet_energyRing_dR3_mu_Jet_e <<endl;
              cout<<"Jet_energyRing_dR4_mu_Jet_e :"<<Jet_energyRing_dR4_mu_Jet_e <<endl;
              cout<<"Jet_numDaughters_pt03 :"<<Jet_numDaughters_pt03 <<endl;
              cout<<"Jet_chHEF:"<<Jet_chHEF<<endl;
              cout<<"Jet_chEmEF:"<<Jet_chEmEF<<endl;
              cout<<"Jet_leptonPtRelInv:"<<Jet_leptonPtRelInv<<endl;
              cout<<"isEle:"<<isEle<<endl;
              cout<<"isMu:"<<isMu<<endl;
              cout<<"isOther:"<<isOther<<endl;
              cout<<"Jet_mass:"<<Jet_mass<<endl;
              cout<<"Jet_withPtd:"<<Jet_withPtd<<endl;
          }

          SetNNVectorVar();
          bRegNN = EvaluateNN();
          NNvectorVar_.clear();

          fjet.addUserFloat("bRegNNCorr", bRegNN[0]*y_std_+y_mean_);
          fjet.addUserFloat("bRegNNResolution",0.5*(bRegNN[2]-bRegNN[1])*y_std_);

          //TLorentzVector jetCorrected;
          //jetCorrected.SetPtEtaPhiE(fjet.pt()*fjet.userFloat("bRegNNCorr"),fjet.eta(),fjet.phi(),fjet.p4().e()*fjet.userFloat("bRegNNCorr"));

          //math::XYZTLorentzVector jetCorr;
          //jetCorr.SetPxPyPzE(jetCorrected.Px(),jetCorrected.Py(),jetCorrected.Pz(),jetCorrected.E());

          //fjet.setP4(jetCorr);//set the jet  with the regressed pt

          //std::cout << "checking again regression float values in same events"  << fjet.userFloat("bRegNNCorr") << std::endl;

          if (0){
              cout<<"bRegNNCorr:"<<bRegNN[0]*y_std_+y_mean_<<endl;
              cout<<"bRegNNResolution:"<<0.5*(bRegNN[2]-bRegNN[1])*y_std_<<endl;
              std::cout<<"--------------------------------------------------------------"<<std::endl;
              std::cout<<"--------------------------------------------------------------"<<std::endl;
          }


          jetColl->push_back( fjet );



      }
      evt.put( std::move( jetColl ) );
  }

  void bRegressionProducer::InitJet(){
      Jet_pt = 0.;
      Jet_eta = 0.;
      rho = 0.;
      Jet_mt = 0.;
      Jet_leadTrackPt = 0.;
      Jet_leptonPtRel = 0.;
      Jet_leptonDeltaR = 0.;
      Jet_neHEF = 0.;
      Jet_neEmEF = 0.;
      Jet_vtxPt = 0.;
      Jet_vtxMass = 0.;
      Jet_vtx3dL = 0.;
      Jet_vtxNtrk = 0.;
      Jet_vtx3deL = 0.;
      Jet_numDaughters_pt03 = 0;
      Jet_energyRing_dR0_em_Jet_e = 0.;
      Jet_energyRing_dR1_em_Jet_e = 0.;
      Jet_energyRing_dR2_em_Jet_e = 0.;
      Jet_energyRing_dR3_em_Jet_e = 0.;
      Jet_energyRing_dR4_em_Jet_e = 0.;
      Jet_energyRing_dR0_neut_Jet_e = 0.;
      Jet_energyRing_dR1_neut_Jet_e = 0.;
      Jet_energyRing_dR2_neut_Jet_e = 0.;
      Jet_energyRing_dR3_neut_Jet_e = 0.;
      Jet_energyRing_dR4_neut_Jet_e = 0.;
      Jet_energyRing_dR0_ch_Jet_e = 0.;
      Jet_energyRing_dR1_ch_Jet_e = 0.;
      Jet_energyRing_dR2_ch_Jet_e = 0.;
      Jet_energyRing_dR3_ch_Jet_e = 0.;
      Jet_energyRing_dR4_ch_Jet_e = 0.;
      Jet_energyRing_dR0_mu_Jet_e = 0.;
      Jet_energyRing_dR1_mu_Jet_e = 0.;
      Jet_energyRing_dR2_mu_Jet_e = 0.;
      Jet_energyRing_dR3_mu_Jet_e = 0.;
      Jet_energyRing_dR4_mu_Jet_e = 0.;
      Jet_chHEF = 0.;//implement from here
      Jet_chEmEF = 0.;
      Jet_leptonPtRelInv = 0.;
      isEle = 0;
      isMu = 0;
      isOther = 0;
      Jet_mass = 0.;
      Jet_withPtd = 0.;


  }//end InitJet

  void bRegressionProducer::SetNNVectorVar(){

      NNvectorVar_.push_back(Jet_pt) ;//0
      NNvectorVar_.push_back(Jet_eta) ;
      NNvectorVar_.push_back(rho) ;
      NNvectorVar_.push_back(Jet_mt) ;
      NNvectorVar_.push_back(Jet_leadTrackPt) ;
      NNvectorVar_.push_back(Jet_leptonPtRel) ;//5
      NNvectorVar_.push_back(Jet_leptonDeltaR) ;
      NNvectorVar_.push_back(Jet_neHEF) ;
      NNvectorVar_.push_back(Jet_neEmEF) ;
      NNvectorVar_.push_back(Jet_vtxPt) ;
      NNvectorVar_.push_back(Jet_vtxMass) ;//10
      NNvectorVar_.push_back(Jet_vtx3dL) ;
      NNvectorVar_.push_back(Jet_vtxNtrk) ;
      NNvectorVar_.push_back(Jet_vtx3deL) ;
      NNvectorVar_.push_back(Jet_numDaughters_pt03) ;//this variable has changed order, in bdt it was last, check why
      NNvectorVar_.push_back(Jet_energyRing_dR0_em_Jet_e) ;//15
      NNvectorVar_.push_back(Jet_energyRing_dR1_em_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR2_em_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR3_em_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR4_em_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR0_neut_Jet_e) ;//20
      NNvectorVar_.push_back(Jet_energyRing_dR1_neut_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR2_neut_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR3_neut_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR4_neut_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR0_ch_Jet_e) ;//25
      NNvectorVar_.push_back(Jet_energyRing_dR1_ch_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR2_ch_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR3_ch_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR4_ch_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR0_mu_Jet_e) ;//30
      NNvectorVar_.push_back(Jet_energyRing_dR1_mu_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR2_mu_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR3_mu_Jet_e) ;
      NNvectorVar_.push_back(Jet_energyRing_dR4_mu_Jet_e) ;
      NNvectorVar_.push_back(Jet_chHEF);//35
      NNvectorVar_.push_back(Jet_chEmEF);
      NNvectorVar_.push_back(Jet_leptonPtRelInv);
      NNvectorVar_.push_back(isEle);
      NNvectorVar_.push_back(isMu);
      NNvectorVar_.push_back(isOther);//40
      NNvectorVar_.push_back(Jet_mass);
      NNvectorVar_.push_back(Jet_withPtd);


  }

  std::vector<float> bRegressionProducer::EvaluateNN(){
      unsigned int shape=NNvectorVar_.size();
      dnn::tf::Shape xShape[] = { 1, shape };
      dnn::tf::Tensor* x = NNgraph_.defineInput(new dnn::tf::Tensor("ffwd_inp:0", 2, xShape));
      dnn::tf::Tensor* y = NNgraph_.defineOutput(new dnn::tf::Tensor("ffwd_out/BiasAdd:0"));
      for (int i = 0; i < x->getShape(1); i++){
          //            std::cout<<"i:"<<i<<" x:"<<NNvectorVar_[i]<<std::endl;
          x->setValue<float>(0, i, NNvectorVar_[i]);
      }
      NNgraph_.eval();
      std::vector<float> correction(3);//3 outputs, first value is mean and then other 2 quantiles
      correction[0] = y->getValue<float>(0, 0);
      correction[1] = y->getValue<float>(0, 1);
      correction[2] = y->getValue<float>(0, 2);
      //std::cout << "checking correction value"  << correction[0] << std::endl;
      return correction;
  }//end EvaluateNN

//}



//typedef flashgg::bRegressionProducer flashggbRegressionProducer;
DEFINE_FWK_MODULE( bRegressionProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
