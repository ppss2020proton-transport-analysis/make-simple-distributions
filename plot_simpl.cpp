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


	TCanvas c("can1","no title",800,600);

	TFile f("pythia8_13TeV_protons_100k_transported_2_205m_beta40cm_6500GeV_y-185murad.root");
	f.ls();	
	
	TTree*fChain=(TTree*)f.Get("ntuple");
	//t->Show(10);

	Long64_t nentries = fChain->GetEntriesFast();
	cout<<nentries<<'\n';

	c.SaveAs("output_plot.pdf[");


	fChain->Draw("x");
	c.SaveAs("output_plot.pdf");

  	c.Clear();
	fChain->Draw("y");
	c.SaveAs("output_plot.pdf");

	c.Clear();
	fChain->Draw("sx");
	c.SaveAs("output_plot.pdf");

	c.Clear();
	fChain->Draw("sy");
	c.SaveAs("output_plot.pdf");

	//2D plots

	c.Clear();
	fChain->Draw("y:x","","colz");
	c.SaveAs("output_plot.pdf");

	c.Clear();
	fChain->Draw("sx:x","","colz");
	c.SaveAs("output_plot.pdf");

	c.Clear();
	fChain->Draw("sy:y","","colz");
	c.SaveAs("output_plot.pdf");

	c.Clear();
	fChain->Draw("y:pz","","colz");
	c.SaveAs("output_plot.pdf");




	c.SaveAs("output_plot.pdf]");



	  return 0;
}
