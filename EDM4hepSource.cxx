#include "EDM4hepSource.hxx"

#include <ROOT/RDataFrame.hxx>
#include "TCanvas.h"

#include <memory>


int main(int argc, char *argv[]) {
  auto fileName = "/home/jsmiesko/FCC/e4hsource/edm4hep_events.root";

  auto nPart = [](edm4hep::MCParticleCollection& inParts) { std::cout << inParts.size() << "\n"; return inParts.size(); };

  ROOT::EnableImplicitMT(2);

  ROOT::RDataFrame rdf(std::make_unique<EDM4hepSource>(fileName));

  std::cout << "Info: Num. of slots: " <<  rdf.GetNSlots() << std::endl;

  auto rdf2 = rdf.Define("nPart", nPart, {"MCParticles"});
  auto hist = rdf2.Histo1D("nPart");

  hist->Print();

  auto canvas = std::make_unique<TCanvas>("canvas", "Canvas", 450, 450);
  hist->Draw();
  canvas->Print("test.pdf");

  return EXIT_SUCCESS;
}
