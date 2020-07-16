#include<iostream>
#include"TCanvas.h"
#include"TF1.h"
#include"TH1F.h"
#include"TH2F.h"
#include"TFile.h"
#include"TTree.h"
#include "TLegend.h"
#include "TStyle.h"
#include <TROOT.h>
#include <TChain.h>

int main() {
  TFile* file_optics1 = new TFile(
      "root_PPSS_2020/1pythia8_13TeV_protons_100k_transported_205m_beta40cm_6500GeV_y-185murad.root");
  TFile* file_optics2 = new TFile(
      "root_PPSS_2020/1pythia8_13TeV_protons_100k_transported_205m_beta40cm_6500GeV_y-140murad.root");

  TTree* tree_optics1 = (TTree*)file_optics1->Get("ntuple");
  TTree* tree_optics2 = (TTree*)file_optics2->Get("ntuple");

  tree_optics1->SetMakeClass(1);
  tree_optics2->SetMakeClass(1);

  Float_t x1, x2, y1, y2, sx1, sx2, sy1, sy2, px, py, pz;

  tree_optics1->SetBranchAddress("x", &x1);
  tree_optics2->SetBranchAddress("x", &x2);

  tree_optics1->SetBranchAddress("y", &y1);
  tree_optics2->SetBranchAddress("y", &y2);

  tree_optics1->SetBranchAddress("sx", &sx1);
  tree_optics2->SetBranchAddress("sx", &sx2);

  tree_optics1->SetBranchAddress("sy", &sy1);
  tree_optics2->SetBranchAddress("sy", &sy2);

  tree_optics1->SetBranchAddress("px", &px);
  tree_optics1->SetBranchAddress("py", &py);
  tree_optics1->SetBranchAddress("pz", &pz);

  std::cout << "x1_max = " << tree_optics1->GetMaximum("x") << "\tx1_min = " << tree_optics1->GetMinimum("x") << std::endl;
  std::cout << "x2_max = " << tree_optics2->GetMaximum("x") << "\tx2_min = " << tree_optics2->GetMinimum("x") << std::endl;

  std::cout << "y1_max = " << tree_optics1->GetMaximum("y") << "\ty1_min = " << tree_optics1->GetMinimum("y") << std::endl;
  std::cout << "y2_max = " << tree_optics2->GetMaximum("y") << "\ty2_min = " << tree_optics2->GetMinimum("y") << std::endl;

  std::cout << "sx1_max = " << tree_optics1->GetMaximum("sx") << "\tsx1_min = " << tree_optics1->GetMinimum("sx") << std::endl;
  std::cout << "sx2_max = " << tree_optics2->GetMaximum("sx") << "\tsx2_min = " << tree_optics2->GetMinimum("sx") << std::endl;

  std::cout << "sy1_max = " << tree_optics1->GetMaximum("sy") << "\tsy1_min = " << tree_optics1->GetMinimum("sy") << std::endl;
  std::cout << "sy2_max = " << tree_optics2->GetMaximum("sy") << "\tsy2_min = " << tree_optics2->GetMinimum("sy") << std::endl;

  std::cout << "px1_max = " << tree_optics1->GetMaximum("px") << "\tpx1_min = " << tree_optics1->GetMinimum("px") << std::endl;
  std::cout << "py1_max = " << tree_optics1->GetMaximum("py") << "\tpy1_min = " << tree_optics1->GetMinimum("py") << std::endl;
  std::cout << "pz1_max = " << tree_optics1->GetMaximum("pz") << "\tpz1_min = " << tree_optics1->GetMinimum("pz") << std::endl;

  std::cout << "px2_max = " << tree_optics2->GetMaximum("px") << "\tpx2_min = " << tree_optics2->GetMinimum("px") << std::endl;
  std::cout << "py2_max = " << tree_optics2->GetMaximum("py") << "\tpy2_min = " << tree_optics2->GetMinimum("py") << std::endl;
  std::cout << "pz2_max = " << tree_optics2->GetMaximum("pz") << "\tpz2_min = " << tree_optics2->GetMinimum("pz") << std::endl;

  //px1_max = 2.6869        px1_min = -3.08671
  //py1_max = 2.96795       py1_min = -3.79311
  //pz1_max = 6470          pz1_min = 5000.05

  Long64_t nentries = tree_optics1->GetEntriesFast();
  std::cout << "Number of entries: " << nentries << std::endl;

  // 1D histos for the x, y, sx and sy difference between optics1(-185murad) and optics2(-140murad)
  TH1F* x_diff = new TH1F("x_diff", "x difference between optics1(-185murad) and optics2(-140murad);x_diff;nOfEvents", 100, -0.001, 0.001);
  TH1F* y_diff = new TH1F("y_diff", "y difference between optics1(-185murad) and optics2(-140murad);y_diff;nOfEvents", 100, 0.0004, 0.0008);
  TH1F* sx_diff = new TH1F("sx_diff", "sx difference between optics1(-185murad) and optics2(-140murad);sx_diff;nOfEvents", 100, -0.001, 0.001);
  TH1F* sy_diff = new TH1F("sy_diff", "sy difference between optics1(-185murad) and optics2(-140murad)sy_diff;nOfEvents", 100, -0.00001, 0);

  // 2D histos for the x, y, sx and sy vs px/py/pz difference between optics1(-185murad) and optics2(-140murad)
  TH2F* x_diff_vs_px = new TH2F("x_diff_vs_px", "x difference between optics1(-185murad) and optics2(-140murad) vs px;x_diff;px;nOfEvents", 100, -0.001, 0.001, 100, -3.1, 2.7);
  TH2F* y_diff_vs_px = new TH2F("y_diff_vs_px", "y difference between optics1(-185murad) and optics2(-140murad) vs px;y_diff;px;nOfEvents", 100, 0.0004, 0.0008, 100, -3.1, 2.7);
  TH2F* sx_diff_vs_px = new TH2F("sx_diff_vs_px", "sx difference between optics1(-185murad) and optics2(-140murad) vs px;sx_diff;px;nOfEvents", 100, -0.001, 0.001, 100, -3.1, 2.7);
  TH2F* sy_diff_vs_px = new TH2F("sy_diff_vs_px", "sy difference between optics1(-185murad) and optics2(-140murad) vs px;sy_diff;px;nOfEvents", 100, -0.00001, 0, 100, -3.1, 2.7);

  TH2F* x_diff_vs_py = new TH2F("x_diff_vs_py", "x difference between optics1(-185murad) and optics2(-140murad) vs py;x_diff;py;nOfEvents", 100, -0.001, 0.001, 100, -3.8, 3);
  TH2F* y_diff_vs_py = new TH2F("y_diff_vs_py", "y difference between optics1(-185murad) and optics2(-140murad) vs py;y_diff;py;nOfEvents", 100, 0.0004, 0.0008, 100, -3.8, 3);
  TH2F* sx_diff_vs_py = new TH2F("sx_diff_vs_py", "sx difference between optics1(-185murad) and optics2(-140murad) vs py;sx_diff;py;nOfEvents", 100, -0.001, 0.001, 100, -3.8, 3);
  TH2F* sy_diff_vs_py = new TH2F("sy_diff_vs_py", "sy difference between optics1(-185murad) and optics2(-140murad) vs py;sy_diff;py;nOfEvents", 100, -0.00001, 0, 100, -3.8, 3);

  TH2F* x_diff_vs_pz = new TH2F("x_diff_vs_pz", "x difference between optics1(-185murad) and optics2(-140murad) vs pz;x_diff;pz;nOfEvents", 100, -0.001, 0.001, 100, 5000, 6470.2);
  TH2F* y_diff_vs_pz = new TH2F("y_diff_vs_pz", "y difference between optics1(-185murad) and optics2(-140murad) vs pz;y_diff;pz;nOfEvents", 100, 0.0004, 0.0008, 100, 5000, 6470.2);
  TH2F* sx_diff_vs_pz = new TH2F("sx_diff_vs_pz", "sx difference between optics1(-185murad) and optics2(-140murad) vs pz;sx_diff;pz;nOfEvents", 100, -0.001, 0.001, 100, 5000, 6470.2);
  TH2F* sy_diff_vs_pz = new TH2F("sy_diff_vs_pz", "sy difference between optics1(-185murad) and optics2(-140murad) vs pz;sy_diff;pz;nOfEvents", 100, -0.00001, 0, 100, 5000, 6470.2);

  for (Int_t i = 0; i < nentries; i++) {
    tree_optics1->GetEntry(i);
    tree_optics2->GetEntry(i);

    x_diff->Fill(x1-x2);
    y_diff->Fill(y1-y2);
    sx_diff->Fill(sx1-sx2);
    sy_diff->Fill(sy1-sy2);

    x_diff_vs_px->Fill(x1-x2, px);
    x_diff_vs_py->Fill(x1-x2, py);
    x_diff_vs_pz->Fill(x1-x2, pz);

    y_diff_vs_px->Fill(y1-y2, px);
    y_diff_vs_py->Fill(y1-y2, py);
    y_diff_vs_pz->Fill(y1-y2, pz);

    sx_diff_vs_px->Fill(sx1-sx2, px);
    sx_diff_vs_py->Fill(sx1-sx2, py);
    sx_diff_vs_pz->Fill(sx1-sx2, pz);

    sy_diff_vs_px->Fill(sy1-sy2, px);
    sy_diff_vs_py->Fill(sy1-sy2, py);
    sy_diff_vs_pz->Fill(sy1-sy2, pz);
  }
  delete tree_optics1, tree_optics2, file_optics1, file_optics2;

  TCanvas* canvas_diffs_1d = new TCanvas("canvas_diffs_1d", "canvas", 1280, 720);
  canvas_diffs_1d->SaveAs("optics1(-185murad)-optics2(-140murad).pdf[");

  x_diff->Draw("hist");
  canvas_diffs_1d->SaveAs("optics1(-185murad)-optics2(-140murad).pdf");
  canvas_diffs_1d->Clear();

  y_diff->Draw("hist");
  canvas_diffs_1d->SaveAs("optics1(-185murad)-optics2(-140murad).pdf");
  canvas_diffs_1d->Clear();

  sx_diff->Draw("hist");
  canvas_diffs_1d->SaveAs("optics1(-185murad)-optics2(-140murad).pdf");
  canvas_diffs_1d->Clear();

  sy_diff->Draw("hist");
  canvas_diffs_1d->SaveAs("optics1(-185murad)-optics2(-140murad).pdf");
  canvas_diffs_1d->Clear();

  canvas_diffs_1d->SaveAs("optics1(-185murad)-optics2(-140murad).pdf]");

  delete canvas_diffs_1d, x_diff, y_diff, sx_diff, sy_diff;

  TCanvas* canvas_diffs_2d = new TCanvas("canvas_diffs_2d", "canvas", 1280, 720);
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf[");

  x_diff_vs_px->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  x_diff_vs_py->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  x_diff_vs_pz->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  y_diff_vs_px->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  y_diff_vs_py->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  y_diff_vs_pz->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  sx_diff_vs_px->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  sx_diff_vs_py->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  sx_diff_vs_pz->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  sy_diff_vs_px->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  sy_diff_vs_py->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  sy_diff_vs_pz->Draw("colz");
  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf");
  canvas_diffs_2d->Clear();

  canvas_diffs_2d->SaveAs("optics1(-185murad)-optics2(-140murad)_vs_p.pdf]");

  delete canvas_diffs_2d, x_diff_vs_px, x_diff_vs_py, x_diff_vs_pz, 
         y_diff_vs_px, y_diff_vs_py, y_diff_vs_pz, 
         sx_diff_vs_px, sx_diff_vs_py, sx_diff_vs_pz, 
         sy_diff_vs_px, sy_diff_vs_py, sy_diff_vs_pz;

  return 0;
}
