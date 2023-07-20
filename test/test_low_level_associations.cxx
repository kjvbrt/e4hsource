// ROOT
#include "ROOT/RVec.hxx"
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RLogger.hxx"
#include "TCanvas.h"
// EDM4hep
#include "edm4hep/MCParticleData.h"
#include "edm4hep/SimCalorimeterHitData.h"
// FCCAnalyses
#include "FCCAnalyses/SmearObjects.h"
#include "FCCAnalyses/ReconstructedParticle2MC.h"


int main(int argc, char *argv[]) {
  // auto verbosity = ROOT::Experimental::RLogScopedVerbosity(ROOT::Detail::RDF::RDFLogChannel(), ROOT::Experimental::ELogLevel::kInfo);

  int nCPU = 8;
  if (argc > 1) {
    nCPU = atoi(argv[1]);
  }

  // auto fileName = "/tmp/e4hsource/testSiD_edm4hep.root";
  // auto fileName = "/tmp/e4hsource/testSiD_1_edm4hep.root";
  // auto fileName = "/tmp/e4hsource/testSiD_10000_edm4hep.root";
  // auto fileName = "/home/jsmiesko/Work/FCC/e4hsource/input/p8_ee_ZH_ecm240_edm4hep.root";
  auto fileName = "/tmp/e4hsource/p8_ee_ZH_ecm240_edm4hep.root";

  ROOT::EnableImplicitMT(nCPU);

  ROOT::RDataFrame rdf("events", fileName);

  // rdf.Describe().Print();
  std::cout << std::endl;

  std::cout << "Info: Num. of slots: " <<  rdf.GetNSlots() << std::endl;

  auto rdf2 = rdf.Alias("MCRecoAssociations0", "_MCRecoAssociations_rec.index");
  auto rdf3 = rdf2.Alias("MCRecoAssociations1", "_MCRecoAssociations_sim.index");
  auto rdf4 = rdf3.Define(
      "RP_MC_index",
      FCCAnalyses::ReconstructedParticle2MC::getRP2MC_index,
      {"MCRecoAssociations0", "MCRecoAssociations1", "ReconstructedParticles"}
  );
  auto rdf5 = rdf4.Define(
      "SmearedTracks",
      FCCAnalyses::SmearObjects::SmearedTracks(2.0, 2.0, 2.0, 2.0, 2.0, true),
      {"ReconstructedParticles", "_EFlowTrack_trackStates", "RP_MC_index", "Particle"}
  );
  auto rdf6 = rdf5.Define("smearTrack_omega", "return SmearedTracks[0].omega;");
  auto h_smeared_tracks_omega = rdf6.Histo1D("smearTrack_omega");


  h_smeared_tracks_omega->Print();

  auto canvas = std::make_unique<TCanvas>("canvas", "Canvas", 450, 450);
  h_smeared_tracks_omega->Draw();
  canvas->Print("low_level_smeared_tracks_omega.pdf");

  return EXIT_SUCCESS;
}
