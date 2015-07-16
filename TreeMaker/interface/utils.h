#ifndef __UTILS_H_
#define __UTILS_H_
#include "TLorentzVector.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include <vector>
#include <string>
#include <map>
class PtGreater {
  public:
  template <typename T> bool operator () (const T& i, const T& j) {
    return (i.pt() > j.pt());
  }
};


TLorentzVector Part2LorVec(reco::Candidate& cand);
bool PassAll(std::map<std::string, bool> cutrecd);
bool PassAllBut(std::string, std::map<std::string, bool>);

double getPFIsolation(edm::Handle<pat::PackedCandidateCollection>,
		      const reco::Candidate*, double, double, double, bool);

#endif

