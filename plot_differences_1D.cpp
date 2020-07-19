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
#include"TH1F.h"
#include <string>
#include "TSystem.h"
#include "TSystemDirectory.h"
#include "TGraph.h"
#include "THStack.h"

using namespace std;

int main() {

  TFile* file_optics1 = new TFile(
      "root_PPSS_2020/default_p8_13TeV_p_100k_t_205m_beta40cm_6500GeV_y-185murad.root");
  char dirname[]="root_PPSS_2020/shift_x/";  //Directory where result files of transport code are stored !! files need to begin with "shifted"
 



  gROOT->ProcessLine( "gErrorIgnoreLevel = 1001;");
  int it_per_axis=0;

  TCanvas* canvas_diffs_1d_1 = new TCanvas("canvas_diffs_1d_1", "canvas", 1280, 720);
  TCanvas* canvas_diffs_1d_2 = new TCanvas("canvas_diffs_1d_2", "canvas", 1280, 720);
  TCanvas* canvas_diffs_1d_3 = new TCanvas("canvas_diffs_1d_3", "canvas", 1280, 720);
  TCanvas* canvas_diffs_1d_4 = new TCanvas("canvas_diffs_1d_4", "canvas", 1280, 720);
  
  auto legend = new TLegend(0.12,0.6,0.25,0.9);
  THStack *hs_x = new THStack("hs_x","x difference between default and shifted optics;x_diff;nOfEvents");
  THStack *hs_y = new THStack("hs_y","y difference between default and shifted optics;y_diff;nOfEvents");
  THStack *hs_sx = new THStack("hs_sx","sx difference between default and shifted optics;sx_diff;nOfEvents");
  THStack *hs_sy = new THStack("hs_sy","sy difference between default and shifted optics;sy_diff;nOfEvents");

  TSystemDirectory dir(dirname, dirname); 

  TList *files = dir.GetListOfFiles(); 
  if (files) { TSystemFile *file; TString fname; 
    TIter next(files); 
    //std::cout<<"Files in directory: "<<dirname<<'\n';
    while ((file=(TSystemFile*)next())) { 
      fname = file->GetName(); 
      if (!file->IsDirectory() && fname.EndsWith(".root") && fname.BeginsWith("shifted") ) 


  { 
  
  cout << endl<<"Processing: "<< fname.Data()<< endl; 


  std::string path_dir=dir.GetName();
  std::string path_file=fname.Data();
  std::string optics2_path=path_dir+=path_file;

  char optics2_path_arr[optics2_path.length()+1];
  strcpy(optics2_path_arr, optics2_path.c_str()); 

  TFile* file_optics2 = new TFile(optics2_path_arr);



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


  Long64_t nentries = tree_optics1->GetEntriesFast();

  // 1D histos for the x, y, sx and sy difference between optics1_default and optics1_shifted

  TH1F* x_diff_binset = new TH1F("x_diff_binset", "", 100, 1.1*(tree_optics1->GetMinimum("x")-tree_optics2->GetMaximum("x")), 1.1*(tree_optics1->GetMaximum("x")-tree_optics2->GetMinimum("x")) );
  TH1F* y_diff_binset = new TH1F("y_diff_binset", "", 100, 1.1*(tree_optics1->GetMinimum("y")-tree_optics2->GetMaximum("y")), 1.1*(tree_optics1->GetMaximum("y")-tree_optics2->GetMinimum("y")) );
  TH1F* sx_diff_binset = new TH1F("sx_diff_binset", "", 100, 1.1*(tree_optics1->GetMinimum("sx")-tree_optics2->GetMaximum("sx")), 1.1*(tree_optics1->GetMaximum("sx")-tree_optics2->GetMinimum("sx")) );
  TH1F* sy_diff_binset = new TH1F("sy_diff_binset", "", 100, 1.1*(tree_optics1->GetMinimum("sy")-tree_optics2->GetMaximum("sy")), 1.1*(tree_optics1->GetMaximum("sy")-tree_optics2->GetMinimum("sy")) );

  for (Int_t i = 0; i < nentries; i++) {
    tree_optics1->GetEntry(i);
    tree_optics2->GetEntry(i);

    x_diff_binset->Fill(x1-x2);
    y_diff_binset->Fill(y1-y2);
    sx_diff_binset->Fill(sx1-sx2);
    sy_diff_binset->Fill(sy1-sy2);

  }

  double num_of_Stdev=3.5;


  TH1F* x_diff = new TH1F("x_diff", "x difference between default and shifted optics;x_diff;nOfEvents", 
    100,-0.005,0.005);
  TH1F* y_diff = new TH1F("y_diff", "y difference between default and shifted optics;y_diff;nOfEvents", 
    100, -0.005,0.005);
  TH1F* sx_diff = new TH1F("sx_diff", "sx difference between default and shifted optics;sx_diff;nOfEvents", 
    100, -0.00001,0.00001);
  TH1F* sy_diff = new TH1F("sy_diff", "sy difference between default and shifted optics;sy_diff;nOfEvents", 
    100, -0.00003,0.00003);



  for (Int_t i = 0; i < nentries; i++) {
    tree_optics1->GetEntry(i);
    tree_optics2->GetEntry(i);

    x_diff->Fill(x1-x2);
    y_diff->Fill(y1-y2);
    sx_diff->Fill(sx1-sx2);
    sy_diff->Fill(sy1-sy2);

  }


  delete tree_optics1, tree_optics2, file_optics1, file_optics2;


  x_diff->SetLineColor(it_per_axis+1);
  hs_x->Add(x_diff);

  
  //WIP as file-naming is still undetermined
  string str_leg = fname.Data();
  unsigned first=str_leg.find("murad_");
  unsigned last=str_leg.find(".root");
  string substr=str_leg.substr(first+6,last-first-6);


  legend->AddEntry(x_diff,substr.c_str() );

  y_diff->SetLineColor(it_per_axis+1);
  hs_y->Add(y_diff);

  sx_diff->SetLineColor(it_per_axis+1);
  hs_sx->Add(sx_diff);

  sy_diff->SetLineColor(it_per_axis+1);
  hs_sy->Add(sy_diff);

 

  //delete x_diff, y_diff, sx_diff, sy_diff;
 
  it_per_axis+=1;


    } 
  } 
} 



  std::string filename_1d = "plots_PPSS_2020/plottest.pdf";
 
  canvas_diffs_1d_1->SaveAs((filename_1d + "[").c_str());

  canvas_diffs_1d_1->cd();
  hs_x->Draw("nostack");
  legend->Draw();
  canvas_diffs_1d_1->SaveAs(filename_1d.c_str());

  canvas_diffs_1d_2->cd();
  hs_y->Draw("nostack");
  legend->Draw();
  canvas_diffs_1d_2->SaveAs(filename_1d.c_str());

  canvas_diffs_1d_3->cd();
  hs_sx->Draw("nostack");
  legend->Draw();
  canvas_diffs_1d_3->SaveAs(filename_1d.c_str());

  canvas_diffs_1d_4->cd();
  hs_sy->Draw("nostack");
  legend->Draw();
  canvas_diffs_1d_4->SaveAs(filename_1d.c_str());

  canvas_diffs_1d_1->SaveAs((filename_1d + "]").c_str());


  delete canvas_diffs_1d_1, canvas_diffs_1d_2, canvas_diffs_1d_3, canvas_diffs_1d_4;
  delete hs_x,hs_y,hs_sx,hs_sy;

  return 0;
}
