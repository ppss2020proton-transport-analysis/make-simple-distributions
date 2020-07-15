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

using namespace std;



int main(){

	TH1F x_dispo_1("x_dispo_1", "x opt1", 100, -0.1, 0.1);
	TH1F x_dispo_2("x_dispo_2", "x opt2", 100, -1, 1);
	TH1F x_dispo_difference("x_dispo_difference", "x difference opt1-opt2", 100, -0.001, 0.001);
	TH1F y_dispo_difference("y_dispo_difference", "y difference opt1-opt2", 100, 0.0004, 0.0008);
	TH1F sx_dispo_difference("sx_dispo_difference", "sx difference opt1-opt2", 100, -0.001, 0.001);
	TH1F sy_dispo_difference("sy_dispo_difference", "sy difference opt1-opt2", 100, -0.00001, 0.00001);

	TH2F xdiff_pxdiff("xdiff_pxdiff", "xdiff_pxdiff;x;px", 100, -1, 1, 100, -5, 5);
	TH2F xdiff_pydiff("xdiff_pydiff", "xdiff_pydiff;x;py", 100, -1, 1, 100, -5, 5);
	TH2F xdiff_pzdiff("xdiff_pzdiff", "xdiff_pzdiff;x;pz", 100, -10000, 10000, 100, -10000, 10000);

	TH2F ydiff_pxdiff("ydiff_pxdiff", "ydiff_pxdiff;y;px", 100, 0.00058, 0.00065, 100, -5, 5);
	TH2F ydiff_pydiff("ydiff_pydiff", "ydiff_pydiff;y;py", 100, 0.00058, 0.00065, 100, -5, 5);
	TH2F ydiff_pzdiff("ydiff_pzdiff", "ydiff_pzdiff;y;pz", 100, 0.0005, 0.0007, 100, 4500, 7000);

	TH2F sxdiff_pxdiff("sxdiff_pxdiff", "sxdiff_pxdiff;sx;px", 100, -0.01, 0.01, 100, -4, 4);
	TH2F sxdiff_pydiff("sxdiff_pydiff", "sxdiff_pydiff;sx;py", 100, -0.01, 0.01, 100, -4, 4);
	TH2F sxdiff_pzdiff("sxdiff_pzdiff", "sxdiff_pzdiff;sx;pz", 100, -10000, 10000, 100, -10000, 10000);

	TH2F sydiff_pxdiff("sydiff_pxdiff", "sydiff_pxdiff;sy;px", 100, -0.000006, -0.000001, 100, -4, 4);
	TH2F sydiff_pydiff("sydiff_pydiff", "sydiff_pydiff;sy;py", 100, -0.000006, -0.000001, 100, -4, 4);
	TH2F sydiff_pzdiff("sydiff_pzdiff", "sydiff_pzdiff;sy;pz", 100, -10000, 10000, 100, -10000, 10000);

	// TH1F x_dispo_difference("x_dispo_difference", "x difference opt1-opt2", 100, -0.5, 0.5);
	// TH1F y_dispo_difference("y_dispo_difference", "y difference opt1-opt2", 100, 0.004, 0.004);
	// TH1F sx_dispo_difference("sx_dispo_difference", "sx difference opt1-opt2", 100, -0.003, 0.001);
	// TH1F sy_dispo_difference("sy_dispo_difference", "sy difference opt1-opt2", 100, -0.00002, 0.00001);

	// TH2F xdiff_pxdiff("xdiff_pxdiff", "xdiff_pxdiff", 100, -0.4,-0.2, 100, -1, 1);
	// TH2F xdiff_pydiff("xdiff_pydiff", "xdiff_pydiff", 100,  -0.4,-0.2, 100, -1, 1);
	// TH2F xdiff_pzdiff("xdiff_pzdiff", "xdiff_pzdiff", 100,  -0.4,-0.2, 100, -1, 1);

	// TH2F ydiff_pxdiff("ydiff_pxdiff", "ydiff_pxdiff", 100, -0.0058, 0.0066, 10, -0.0001, 0.0001);
	// TH2F ydiff_pydiff("ydiff_pydiff", "ydiff_pydiff", 100, -0.0058, 0.0066, 10, -0.0001, 0.0001);
	// TH2F ydiff_pzdiff("ydiff_pzdiff", "ydiff_pzdiff", 100, -0.0058, 0.0066, 10, -0.0001, 0.0001);

	// TH2F sxdiff_pxdiff("sxdiff_pxdiff", "sxdiff_pxdiff", 100, -0.0001, 0.0005, 10, -1, 1);
	// TH2F sxdiff_pydiff("sxdiff_pydiff", "sxdiff_pydiff", 100, -0.0001, 0.0005, 10, -1, 1);
	// TH2F sxdiff_pzdiff("sxdiff_pzdiff", "sxdiff_pzdiff", 100, -0.0001, 0.0005, 10, -1, 1);

	// TH2F sydiff_pxdiff("sydiff_pxdiff", "sydiff_pxdiff", 100, -0.0001, 0.00005, 10, -0.0001, 0.0001);
	// TH2F sydiff_pydiff("sydiff_pydiff", "sydiff_pydiff",  100, -0.0001, 0.00005, 10, -0.0001, 0.0001);
	// TH2F sydiff_pzdiff("sydiff_pzdiff", "sydiff_pzdiff",  100, -0.0001, 0.00005, 10, -0.0001, 0.0001);



	//Loading files

	TFile f1("pythia8_13TeV_protons_100k_transported_205m_beta40cm_6500GeV_y-185murad.root");
	TFile f2("pythia8_13TeV_protons_100k_transported_205m_beta40cm_6500GeV_y-140murad.root");
	//TFile f2("pythia8_13TeV_protons_100k_transported_2_205m_beta40cm_6500GeV_y-185murad.root");

	TTree* tree1=(TTree*)f1.Get("ntuple");
	TTree* tree2=(TTree*)f2.Get("ntuple");
	//t->Show(10);

	tree1->SetMakeClass(1);
	tree2->SetMakeClass(1);

	float x1,x2,y1,y2,sx1,sx2,sy1,sy2,px1,px2,py1,py2,pz1,pz2;

	//fChain->Branch("x", &n_x);


  	tree1->SetBranchAddress("x", &x1);
  	tree2->SetBranchAddress("x", &x2);

	tree1->SetBranchAddress("y", &y1);
  	tree2->SetBranchAddress("y", &y2);

	tree1->SetBranchAddress("sx", &sx1);
  	tree2->SetBranchAddress("sx", &sx2);

	tree1->SetBranchAddress("sy", &sy1);
  	tree2->SetBranchAddress("sy", &sy2);

	tree1->SetBranchAddress("px", &px1);
  	tree2->SetBranchAddress("px", &px2);

	tree1->SetBranchAddress("py", &py1);
  	tree2->SetBranchAddress("py", &py2);

	tree1->SetBranchAddress("pz", &pz1);
  	tree2->SetBranchAddress("pz", &pz2);


	Long64_t nentries = tree1->GetEntriesFast();
	cout<<nentries<<'\n';


	for (int i = 0; i < nentries; ++i)

	{
		tree1->GetEntry(i);
		tree2->GetEntry(i);

		x_dispo_difference.Fill(x1-x2);
		y_dispo_difference.Fill(y1-y2);
		sx_dispo_difference.Fill(sx1-sx2);
		sy_dispo_difference.Fill(sy1-sy2);

		xdiff_pxdiff.Fill(x1-x2,px1);
		xdiff_pydiff.Fill(x1-x2,py1);
		xdiff_pzdiff.Fill(x1-x2,pz1);

		ydiff_pxdiff.Fill(y1-y2,px1);
		ydiff_pydiff.Fill(y1-y2,py1);
		ydiff_pzdiff.Fill(y1-y2,pz1);

		sxdiff_pxdiff.Fill(sx1-sx2,px1);
		sxdiff_pydiff.Fill(sx1-sx2,py1);
		sxdiff_pzdiff.Fill(sx1-sx2,pz1);

		sydiff_pxdiff.Fill(sy1-sy2,px1);
		sydiff_pydiff.Fill(sy1-sy2,py1);
		sydiff_pzdiff.Fill(sy1-sy2,pz1);


	}	


	TCanvas c("can1","no title",800,600);

	c.SaveAs("opt_1-opt2.pdf[");


	// 1D plots
    c.Clear();
    x_dispo_difference.Draw("hist");
	c.SaveAs("opt_1-opt2.pdf");

    c.Clear();
    y_dispo_difference.Draw("hist");
	c.SaveAs("opt_1-opt2.pdf");

    c.Clear();
    sx_dispo_difference.Draw("hist");
	c.SaveAs("opt_1-opt2.pdf");

	c.Clear();
    sy_dispo_difference.Draw("hist");
	c.SaveAs("opt_1-opt2.pdf");

	// 2D Plots x - difference
	c.Clear();
    xdiff_pxdiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");

	c.Clear();
    xdiff_pydiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");

	c.Clear();
    xdiff_pzdiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");

	//y - difference

	c.Clear();
    ydiff_pxdiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");

	c.Clear();
    ydiff_pydiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");

	c.Clear();
    ydiff_pzdiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");


	//sx - difference
	c.Clear();
    sxdiff_pxdiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");

	c.Clear();
    sxdiff_pydiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");

	c.Clear();
    sxdiff_pzdiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");

	//sy - difference
	c.Clear();
    sydiff_pxdiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");

	c.Clear();
    sydiff_pydiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");

	c.Clear();
    sydiff_pzdiff.Draw("colz");
	c.SaveAs("opt_1-opt2.pdf");


	c.SaveAs("opt_1-opt2.pdf]");




	  return 0;
}
