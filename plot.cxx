#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "iostream"
#include "filesystem"
#include "string"

int main() {
  char pathToOpticFiles[] = 
    "root_PPSS_2020";

  Int_t i = 0;
  for (const auto& entry : std::filesystem::directory_iterator(pathToOpticFiles)) {
    std::cout << entry.path().string() << std::endl;
    std::string fileName = entry.path().string();
    TFile* f = TFile::Open(fileName.c_str());
    f->ls();

    TTree* t = (TTree*)f->Get("ntuple");

    Long64_t nentries = t->GetEntriesFast();
    std::cout << "Number of entries: " << nentries << std::endl;

    TCanvas* canv = new TCanvas("canv", "canvas", 1280, 720);

    i++;
    std::string plotName = "plots" + std::to_string(i) + ".pdf";
    std::string openPlotName = "plots" + std::to_string(i) + ".pdf[";
    std::string closePlotName = "plots" + std::to_string(i) + ".pdf]";
    // Open plots.pdf
    canv->SaveAs(openPlotName.c_str()); 

    // 1D histos
    t->Draw("process_code");
    canv->SaveAs(plotName.c_str()); 
    canv->Clear();

    t->Draw("x");
    canv->SaveAs(plotName.c_str()); 
    canv->Clear();

    t->Draw("y");
    canv->SaveAs(plotName.c_str()); 
    canv->Clear();

    t->Draw("sx");
    canv->SaveAs(plotName.c_str()); 
    canv->Clear();

    t->Draw("sy");
    canv->SaveAs(plotName.c_str()); 
    canv->Clear();

    // 2D histos
    t->Draw("x:y", "", "colz");
    canv->SaveAs(plotName.c_str()); 
    canv->Clear();

    t->Draw("x:sx", "", "colz");
    canv->SaveAs(plotName.c_str()); 
    canv->Clear();

    t->Draw("y:sy", "", "colz");
    canv->SaveAs(plotName.c_str()); 
    canv->Clear();

    canv->SaveAs(closePlotName.c_str());

    delete canv, t, f;
  }


  return 0;
}
