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

	TH1F x_dispo_difference("x_dispo_difference", "x difference opt1-opt2", 100, -0.01, 0.01);
	TH1F y_dispo_difference("y_dispo_difference", "y difference opt1-opt2", 100, -0.01, 0.01);
	TH1F sx_dispo_difference("sx_dispo_difference", "sx difference opt1-opt2", 100, -0.0001, 0.0001);
	TH1F sy_dispo_difference("sy_dispo_difference", "sy difference opt1-opt2", 100, -0.0001, 0.0001);

	TH2F xdiff_pxdiff("xdiff_pxdiff", "xdiff_pxdiff;x;px", 100, -0.005, 0.005, 100, -5, 5);
	TH2F xdiff_pydiff("xdiff_pydiff", "xdiff_pydiff;x;py", 100, -0.005, 0.005, 100, -5, 5);
	TH2F xdiff_pzdiff("xdiff_pzdiff", "xdiff_pzdiff;x;pz", 100, -0.01, 0.01, 100, 4500, 7000);

	TH2F ydiff_pxdiff("ydiff_pxdiff", "ydiff_pxdiff;y;px", 100, -1, 1, 100, -5, 5);
	TH2F ydiff_pydiff("ydiff_pydiff", "ydiff_pydiff;y;py", 100, -1, 1, 100, -5, 5);
	TH2F ydiff_pzdiff("ydiff_pzdiff", "ydiff_pzdiff;y;pz", 100, -0.01, 0.01, 100, 4500, 7000);

	TH2F sxdiff_pxdiff("sxdiff_pxdiff", "sxdiff_pxdiff;sx;px", 100,  -0.0005, 0.0005, 100, -5, 5);
	TH2F sxdiff_pydiff("sxdiff_pydiff", "sxdiff_pydiff;sx;py", 100,  -0.0005, 0.0005, 100, -5, 5);
	TH2F sxdiff_pzdiff("sxdiff_pzdiff", "sxdiff_pzdiff;sx;pz", 100, -0.0001, 0.0001, 100, 4500, 7000);

	TH2F sydiff_pxdiff("sydiff_pxdiff", "sydiff_pxdiff;sy;px", 100, -1, 1, 100, -5, 5);
	TH2F sydiff_pydiff("sydiff_pydiff", "sydiff_pydiff;sy;py", 100, -1, 1, 100, -5, 5);
	TH2F sydiff_pzdiff("sydiff_pzdiff", "sydiff_pzdiff;sy;pz", 100, -0.0001, 0.0001, 100, 4500, 7000);


	//Loading files

	TFile f1("pythia8_13TeV_protons_100k_transported_205m_beta40cm_6500GeV_y-185murad.root");
	TFile f2("pythia8_13TeV_protons_100k_transported_shifted_205m_beta40cm_6500GeV_y-185murad.root");
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
    
    cout<<"\n\ndiffx GetRMS: "<<x_dispo_difference.GetRMS()<<'\n';
    cout<<"diffx GetMean: "<<x_dispo_difference.GetMean()<<"\n\n";

	cout<<"diffy GetRMS: "<<y_dispo_difference.GetRMS()<<'\n';
	cout<<"diffy GetMean: "<<y_dispo_difference.GetMean()<<"\n\n";
	
	cout<<"diffsx GetRMS: "<<sx_dispo_difference.GetRMS()<<'\n';
	cout<<"diffsx GetMean: "<<sx_dispo_difference.GetMean()<<"\n\n";

	cout<<"diffsy GetRMS: "<<sy_dispo_difference.GetRMS()<<'\n';
	cout<<"diffsy GetMean: "<<sy_dispo_difference.GetMean()<<"\n\n";

	  return 0;
}
