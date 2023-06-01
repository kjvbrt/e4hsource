#include "EDM4hepSource/EDM4hepSource.hxx"

#include <ROOT/RDataFrame.hxx>
#include <TCanvas.h>

#include <memory>


float firstParticleMomentaX(edm4hep::MCParticleCollection& inParticles) {
  return inParticles[0].getMomentum().x;
}


int main(int argc, char *argv[]) {
  // auto fileName = "/home/jsmiesko/Work/FCC/e4hsource/input/edm4hep_events.root";
  auto fileName = "/home/jsmiesko/Work/FCC/e4hsource/input/testSiD_edm4hep.root";

  ROOT::EnableImplicitMT(1);

  ROOT::RDataFrame rdf(std::make_unique<e4hsource::EDM4hepSource>(fileName));

  rdf.Describe().Print();
  std::cout << std::endl;

  std::cout << "Into: Num. of slots: " <<  rdf.GetNSlots() << std::endl;

  auto rdf2 = rdf.Define("mX", firstParticleMomentaX, {"MCParticles"});
  auto hist = rdf2.Histo1D("mX");

  hist->Print();

  auto canvas = std::make_unique<TCanvas>("canvas", "Canvas", 450, 450);
  hist->Draw();
  canvas->Print("test.pdf");

  return EXIT_SUCCESS;
}
