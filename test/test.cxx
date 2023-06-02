// EDM4hepSource
#include "EDM4hepSource/EDM4hepSource.hxx"
// ROOT
#include <ROOT/RDataFrame.hxx>
#include <TCanvas.h>
// EDM4hep
#include <edm4hep/MCParticleCollection.h>
#include <edm4hep/SimCalorimeterHitCollection.h>


float firstParticleMomentaX(edm4hep::MCParticleCollection& inParticles) {
  return inParticles[0].getMomentum().x;
}

float nCaloHits(edm4hep::SimCalorimeterHitCollection& hits) {
  return hits.size();
}


int main(int argc, char *argv[]) {
  // auto fileName = "/home/jsmiesko/Work/FCC/e4hsource/input/edm4hep_events.root";
  // auto fileName = "/home/jsmiesko/Work/FCC/e4hsource/input/testSiD_edm4hep.root";
  auto fileName = "/tmp/e4hsource/testSiD_edm4hep.root";

  ROOT::EnableImplicitMT(8);

  ROOT::RDataFrame rdf(std::make_unique<e4hsource::EDM4hepSource>(fileName));

  rdf.Describe().Print();
  std::cout << std::endl;

  std::cout << "Into: Num. of slots: " <<  rdf.GetNSlots() << std::endl;

  auto rdf2 = rdf.Define("partMomentumX", firstParticleMomentaX, {"MCParticles"});
  auto rdf3 = rdf2.Define("nCaloHits", nCaloHits, {"EcalBarrelHits"});
  auto h_partMomentumX = rdf3.Histo1D("partMomentumX");
  auto h_nCaloHits = rdf3.Histo1D("nCaloHits");

  h_partMomentumX->Print();
  h_nCaloHits->Print();

  auto canvas = std::make_unique<TCanvas>("canvas", "Canvas", 450, 450);
  h_partMomentumX->Draw();
  canvas->Print("partMomentumX.pdf");
  h_nCaloHits->Draw();
  canvas->Print("nCaloHits.pdf");

  return EXIT_SUCCESS;
}
