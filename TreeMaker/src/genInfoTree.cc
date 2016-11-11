#include "DelPanj/TreeMaker/interface/genInfoTree.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"

#include <bitset>
#include "DataFormats/HepMCCandidate/interface/GenStatusFlags.h"
#include "TLorentzVector.h"

genInfoTree::genInfoTree(std::string name, TTree* tree, const edm::ParameterSet& iConfig):
  baseTree(name,tree),
  genPartLabel_ (iConfig.getParameter<edm::InputTag>("genPartLabel")),
  MAXNGENPAR_(iConfig.getParameter<unsigned int>("maxNumGenPar")),
  applyStatusSelection_(iConfig.getParameter<bool>("applyStatusSelection")),
  applyPromptSelection_(iConfig.getParameter<bool>("applyPromptSelection")),
  saveLHEWeights_(iConfig.getParameter<bool>("saveLHEWeights"))
{
  genParP4_ =   new TClonesArray("TLorentzVector");
  SetBranches();
}


genInfoTree::~genInfoTree()
{
  delete genParP4_;
}


//
// member functions
//

void
genInfoTree::GetRunInfo(const edm::Run& iRun)
{
  edm::Handle<LHERunInfoProduct> run;
  typedef std::vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;
  if(iRun.getByLabel( "externalLHEProducer", run )){
    LHERunInfoProduct myLHERunInfoProduct = *(run.product());
    for (headers_const_iterator iter=myLHERunInfoProduct.headers_begin(); iter!=myLHERunInfoProduct.headers_end(); iter++){
      // std::cout << iter->tag() << std::endl;
      std::vector<std::string> lines = iter->lines();
      for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
	std::string thisline = lines.at(iLine);
	// only print out lines with weight information, otherwise the log file will be too big
	if(thisline.find("weight id")!=std::string::npos ||
	   thisline.find("weightgroup")!=std::string::npos)
	  std::cout << thisline;
      }
    }
  }
}



// ------------ method called to for each event  ------------
void
genInfoTree::Fill(const edm::Event& iEvent)
{
  Clear();
  if(iEvent.isRealData())return;

  using namespace edm;
  edm::Handle<reco::GenParticleCollection> genParticleHandle;
  if(not iEvent.getByLabel(genPartLabel_, genParticleHandle))
    {
      std::cout<<
	"GenAnalyzer: Generator Level Information not found\n"
	       <<std::endl;
    }

  edm::Handle<GenEventInfoProduct>    genEventInfoHandle;

  if (iEvent.getByLabel("generator", genEventInfoHandle)) {
    if (genEventInfoHandle->hasBinningValues())
      ptHat_ = genEventInfoHandle->binningValues()[0];
      
    mcWeight_ = genEventInfoHandle->weight();

    if (genEventInfoHandle->pdf()) {
      pdf_.push_back(genEventInfoHandle->pdf()->id.first);    // PDG ID of incoming parton #1
      pdf_.push_back(genEventInfoHandle->pdf()->id.second);   // PDG ID of incoming parton #2
      pdf_.push_back(genEventInfoHandle->pdf()->x.first);     // x value of parton #1
      pdf_.push_back(genEventInfoHandle->pdf()->x.second);    // x value of parton #2
      pdf_.push_back(genEventInfoHandle->pdf()->xPDF.first);  // PDF weight for parton #1
      pdf_.push_back(genEventInfoHandle->pdf()->xPDF.second); // PDF weight for parton #2
      pdf_.push_back(genEventInfoHandle->pdf()->scalePDF);    // scale of the hard interaction
    }
  }

  // add HT information
  edm::Handle<LHEEventProduct> evt;
  if(iEvent.getByLabel( "externalLHEProducer", evt )){

    originalLHEweight_ = evt->originalXWGTUP();
    // get PDF and scale weights
    if(saveLHEWeights_){
      for (unsigned int i=0; i< evt->weights().size(); i++) {
	float tempMCWeight = genEventInfoHandle->weight() > 0? 1: -1;
	float sysLHEweight = tempMCWeight* evt->weights()[i].wgt/evt->originalXWGTUP();
	pdfscaleSysWeights_.push_back( sysLHEweight );
      }
    }


    HT_=0;
    const lhef::HEPEUP hepeup_ = evt->hepeup();

    const int nup_ = hepeup_.NUP; 
    const std::vector<int> idup_ = hepeup_.IDUP;
    const std::vector<lhef::HEPEUP::FiveVector> pup_ = hepeup_.PUP;
    const std::vector<int> istup_ = hepeup_.ISTUP;

    for ( unsigned int icount = 0 ; icount < (unsigned int)nup_; icount++ ) {

      int PID    = idup_[icount];
      int status = istup_[icount];
      double px = (pup_[icount])[0];
      double py = (pup_[icount])[1];
            
      if(status!=1)continue;
      // if it's not a gluon or quark
      if(!(abs(PID)==21 || (abs(PID)<6 && abs(PID)>0)))continue;
      HT_ += sqrt(px*px+py*py);
    
    } // end of loop over particles
  } // if LHEEventInfo is found


  // first save the vector of candidates
  unsigned int nParticles_=0;
  bool hasStatusFlag=false; // status flags are not always in every sample, need to check first
  std::vector<const reco::Candidate*> cands;
  std::vector<std::vector<reco::GenParticle>::const_iterator> myParticles;
  for( std::vector<reco::GenParticle>::const_iterator 
	 it_gen = genParticleHandle->begin(); 
       it_gen != genParticleHandle->end(); it_gen++ ) 
    {
      reco::GenParticle gen = *it_gen;
      nParticles_++;
       if(nParticles_==1){
       	const reco::GenStatusFlags& cmsswStatus = it_gen->statusFlags();
       	int status=0;
       	for(unsigned int ist=0; ist<cmsswStatus.flags_.size(); ist++)
       	  {
       	    if(cmsswStatus.flags_[ist])
       	      status |= (0x1 << (ist+1)); 
       	  }
       	if(status>0)hasStatusFlag=true;
       	else hasStatusFlag=false;
       }
       // particle needs to have status < 30 or isPrompt if statusFlag information is available
       if( ( applyStatusSelection_ && !applyPromptSelection_ && gen.status()>=30 )
	   || 
	   ( applyPromptSelection_ &&
	     ( 
	      ( hasStatusFlag && !gen.statusFlags().isPrompt() && gen.status()>=30) || 
	       (!hasStatusFlag && gen.status()>=30)
	       )
	     )
	 )continue; // only save beam particle, hard scattering and stable particles
      cands.push_back(&*it_gen);
      myParticles.push_back(it_gen);
    }

  // now loop
  std::vector<const reco::Candidate*>::const_iterator found = cands.begin();
  for(unsigned int genIndex=0; genIndex < MAXNGENPAR_ && genIndex < myParticles.size(); genIndex++){
    
    std::vector<reco::GenParticle>::const_iterator geni = myParticles[genIndex];
    nGenPar_++;

   TLorentzVector p4(geni->px(),geni->py(),geni->pz(),geni->energy());
    new( (*genParP4_)[nGenPar_-1]) TLorentzVector(p4);

    genParQ_.push_back(geni->charge());
    genParId_.push_back(geni->pdgId());
    genParSt_.push_back(geni->status());

    int mompid = -9999;
    int gmompid = -9999;
    if( geni->numberOfMothers() >0 )
      {
        mompid = geni->mother(0)->pdgId();
        if(geni->mother(0)->numberOfMothers() >0)
          gmompid = geni->mother(0)->mother(0)->pdgId();
        else
          gmompid = -9999;
      }
    else
      {
        mompid = -9999;
        gmompid = -9999;
      }

	
    genMomParId_.push_back(mompid);
    genGMomParId_.push_back(gmompid);


    genParIndex_.push_back(genIndex);

    int iMo1 = -1;
    int iMo2 = -1;
    int iDa1 = -1;
    int iDa2 = -1;
    int NMo = geni->numberOfMothers();
    int NDa = geni->numberOfDaughters();

    found = find(cands.begin(), cands.end(), geni->mother(0));
    if(found != cands.end()) iMo1 = found - cands.begin() ;

    found = find(cands.begin(), cands.end(), geni->mother(1));
    if(found != cands.end()) iMo2 = found - cands.begin() ;

    found = find(cands.begin(), cands.end(), geni->daughter(0));
    if(found != cands.end()) iDa1 = found - cands.begin() ;

    found = find(cands.begin(), cands.end(), geni->daughter(1));
    if(found != cands.end()) iDa2 = found - cands.begin() ;

    genNMo_.push_back(NMo);
    genNDa_.push_back(NDa);
    genMo1_.push_back(iMo1);
    genMo2_.push_back(iMo2);
    genDa1_.push_back(iDa1);
    genDa2_.push_back(iDa2);
    const reco::GenStatusFlags& cmsswStatus = geni->statusFlags();
    int status=0;
    for(unsigned int ist=0; ist<cmsswStatus.flags_.size(); ist++)
      {
	if(cmsswStatus.flags_[ist])
	  status |= (0x1 << (ist+1)); 
      }
    genStFlag_.push_back(status);
      
  } // end of loop over particles

   
}



void  
genInfoTree::SetBranches(){

  AddBranch(&ptHat_, "ptHat");
  AddBranch(&mcWeight_, "mcWeight");

  AddBranch(&HT_, "HT");
  AddBranch(&pdf_, "pdf");
  AddBranch(&originalLHEweight_, "originalLHEweight");
  AddBranch(&pdfscaleSysWeights_, "pdfscaleSysWeights");
  

  AddBranch(&nGenPar_, "nGenPar");
  AddBranch(&genParP4_, "genParP4");
  AddBranch(&genParQ_,"genParQ");
  AddBranch(&genParId_,"genParId");
  AddBranch(&genParSt_,"genParSt");
  AddBranch(&genMomParId_,"genMomParId");
  AddBranch(&genGMomParId_,"genGMomParId");
  AddBranch(&genParIndex_,"genParIndex");

  AddBranch(&genNMo_,"genNMo");
  AddBranch(&genNDa_,"genNDa");
  AddBranch(&genMo1_,"genMo1");
  AddBranch(&genMo2_,"genMo2");
  AddBranch(&genDa1_,"genDa1");
  AddBranch(&genDa2_,"genDa2");
  AddBranch(&genStFlag_,"genStFlag");
  
  
}


void  
genInfoTree::Clear(){

  ptHat_ = -9999.0;
  mcWeight_ = -9999.0; 

  HT_    = -9999.0;
  pdf_.clear();
  originalLHEweight_ = 1;
  pdfscaleSysWeights_.clear();
  nGenPar_ =0;
  genParP4_->Clear();

  genParQ_.clear();
  genParId_.clear();
  genParSt_.clear();
  genMomParId_.clear();
  genGMomParId_.clear();
  genParIndex_.clear();
  genNMo_.clear();
  genNDa_.clear();
  genMo1_.clear();
  genMo2_.clear();
  genDa1_.clear();
  genDa2_.clear();
  genStFlag_.clear();

  
}



