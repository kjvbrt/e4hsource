// EDM4hepSource
#include "EDM4hepSource/EDM4hepSource.hxx"
// ROOT
#include <ROOT/RVec.hxx>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RLogger.hxx>
#include <TCanvas.h>
// EDM4hep
#include <edm4hep/MCParticle.h>
#include <edm4hep/MCParticleCollection.h>
#include <edm4hep/SimCalorimeterHitCollection.h>

edm4hep::MCParticleCollection selElectrons(edm4hep::MCParticleCollection& inParticles) {
  edm4hep::MCParticleCollection electrons;
  electrons.setSubsetCollection();
  for (auto particle: inParticles) {
    if (particle.getPDG() == 11) {
      auto electron = edm4hep::MCParticle(particle);
      electrons.push_back(electron);
    }
  }

  return electrons;
}

struct selPDG {
  selPDG(int pdg = 11, bool chargeConjugateAllowed = true);
  const int m_pdg;
  const bool m_chargeConjugateAllowed;
  edm4hep::MCParticleCollection operator() (edm4hep::MCParticleCollection& inParticles);
};

selPDG::selPDG(int pdg,
               bool chargeConjugateAllowed) : m_pdg(pdg),
                                              m_chargeConjugateAllowed(chargeConjugateAllowed) {};

edm4hep::MCParticleCollection selPDG::operator() (edm4hep::MCParticleCollection& inParticles) {
  edm4hep::MCParticleCollection result;
  result.setSubsetCollection();
  for (auto particle: inParticles) {
    if (m_chargeConjugateAllowed) {
      if (std::abs(particle.getPDG() ) == std::abs(m_pdg)) {
        result.push_back(particle);
      }
    }
    else {
      if(particle.getPDG() == m_pdg) {
        result.push_back(particle);
      }
    }
  }

  return result;
}

ROOT::VecOps::RVec<float> getPx(edm4hep::MCParticleCollection& inParticles) {
  ROOT::VecOps::RVec<float> result;
  for (auto particle: inParticles) {
    result.push_back(particle.getMomentum().x);
  }

  return result;
}


int main(int argc, char *argv[]) {
  // auto verbosity = ROOT::Experimental::RLogScopedVerbosity(ROOT::Detail::RDF::RDFLogChannel(), ROOT::Experimental::ELogLevel::kInfo);

  int nCPU = 8;
  if (argc > 1) {
    nCPU = atoi(argv[1]);
  }

  auto fileName = "/tmp/e4hsource/testSiD_edm4hep.root";
  // auto fileName = "/tmp/e4hsource/testSiD_1_edm4hep.root";
  // auto fileName = "/tmp/e4hsource/testSiD_10000_edm4hep.root";

  ROOT::EnableImplicitMT(nCPU);

  ROOT::RDataFrame rdf(std::make_unique<e4hsource::EDM4hepSource>(fileName));

  // rdf.Describe().Print();
  std::cout << std::endl;

  std::cout << "Info: Num. of slots: " <<  rdf.GetNSlots() << std::endl;

  auto rdf2 = rdf.Define("particles_px", getPx, {"MCParticles"});
  // auto rdf3 = rdf2.Define("electrons", selElectrons, {"MCParticles"});
  auto rdf3 = rdf2.Define("electrons", selPDG(11, false), {"MCParticles"});
  auto rdf4 = rdf3.Define("electrons_px", getPx, {"electrons"});
  auto h_particles_px = rdf4.Histo1D("particles_px");
  auto h_electrons_px = rdf4.Histo1D("electrons_px");

  h_particles_px->Print();
  h_electrons_px->Print();

  auto canvas = std::make_unique<TCanvas>("canvas", "Canvas", 450, 450);
  h_particles_px->Draw();
  canvas->Print("source_particles_px.pdf");
  h_electrons_px->Draw();
  canvas->Print("source_electrons_px.pdf");

  return EXIT_SUCCESS;
}
