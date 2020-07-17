/**
 \author Maciej Trzebinski
 \version 1.0
 \date 21/03/2020
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <unistd.h>

#include <TFile.h>
#include <TH1F.h>
#include <TTree.h>
#include <TROOT.h>
#include <algorithm>
#include <map>
using std::cout;
using std::endl;
using std::vector;
using std::sin;
using std::cos;
using std::sinh;
using std::cosh;
using std::sqrt;
using std::string;
using std::ifstream;
using std::istringstream;

struct Shift {
  double x_shift = 0; 
  double y_shift = 0; 
  double z_shift = 0;
};

struct Magnet {
  string magnet_type = "";
  int magnet_number = 0;
};

class ProtonTransport {
  public:
    ProtonTransport(); //!< constructor 
    ~ProtonTransport(); //!< destructor 
    void PrepareBeamline(string, bool); 
    void simple_tracking(double);
    void simple_pythia_tracking(double);
    void SetBeamEnergy(double);
    double GetBeamEnergy();
    void SetBeampipeSeparation(double);
    double GetBeampipeSeparation();
    void set_shift(int, int, int, double);
    void do_shift(Magnet, string);
  private:
    double IP1Pos;
    double x, y, z, px, py, pz, sx, sy;
    std::map<Magnet, Shift> magnet_to_shift;
    int numb_of_obj_uses[5]; //1 quadrupole 2 dipole 3 h kicker 4 v kicker
    //obj type -diplole quadrupole etc
    //shift obj number 1st 2dn dipole etc ; shift value - self explanatory ;shift axis_axis- x y z strength - condition applied in magnet methods
    // values or vectors- depending if we'll shift 2 things at once - if not values will work
    // numb_of_obj_uses - how many times an magnet/dipole etc has been used for specifiv proton - needs to be cleared NEEDS TO BE A VECTOR WITH ENTRY CORESPONDING OBJ TYPE 
    double beam_energy;
    bool BeampipesAreSeparated;
    double BeampipeSeparation;
    void Marker(bool);
    void simple_drift(double, bool);
    void simple_rectangular_dipole(double, double);
    void simple_horizontal_kicker(double, double);
    void simple_vertical_kicker(double, double);
    void simple_quadrupole(double, double, bool);
    vector <vector <string> > element;
    bool DoApertureCut;
};

/** \class ProtonTransport
\brief All tools needed for proton transport through LHC structures.

Class constructor sets the following default values: \n 
beam_energy of 6500 \n 
BeampipesAreSeparated = false \n 
BeampipeSeparation of 97.e-3 \n 
DoApertureCut = true
*/
ProtonTransport::ProtonTransport() :
  beam_energy(6500.),
  BeampipesAreSeparated(false),
  BeampipeSeparation(97.e-3),
  DoApertureCut(true)
{
}


ProtonTransport::~ProtonTransport()
{
}


/**
\brief Set beam energy in GeV.

Must be accordingly to settings used in optics file.
\param[in] E value of beam energy in GeV.
*/
void ProtonTransport::SetBeamEnergy(double E){
  beam_energy = E;
}

/**
\brief Return beam energy in GeV.

\return beam_energy
*/
double ProtonTransport::GetBeamEnergy(){
  return beam_energy;
}

/**
\brief Set beam separation in m.

In vicinity of collision point protons are in one, common beampipe.
Splitting into two beampipes at TAN element, just before second dipole.
The nominal separation is 97.e-3 m i.e. 97 mm.

\param[in] d vaule of beam separation in m
*/
void ProtonTransport::SetBeampipeSeparation(double d){
  BeampipeSeparation = d;
}

/**
\brief Get beam separation in m.

\return BeampipeSeparation
*/
double ProtonTransport::GetBeampipeSeparation(){
  return BeampipeSeparation;
}

/**
\brief Beam element - marker.

Marker is a "dummy" beam element used to mark a certain place between elements (in drift).
*/
void ProtonTransport::Marker(bool verbose=false){
  if (!verbose) return;
  cout << "MARKER\t";
  cout << "z [m]: " << z;
  cout << "\tx [mm]: " << x*1.e3; 
  cout << "\ty [mm]: " << y*1.e3;
  cout << "\tpx [GeV]: " << px;
  cout << "\tpy [GeV]: " << py;
  cout << "\tpz [GeV]: " << pz;
  cout << "\tsx: " << sx;
  cout << "\tsy: " << sy << endl;
	//cout << z << "\t" << x << "\t" << y << "\t" << sx*pz << "\t" << sy*pz << endl;
}  

void ProtonTransport::simple_drift(double L, bool verbose=false){
  x+=L*sx;
  y+=L*sy;
  z+=L;
  if (!verbose) return;
  cout << "DRIFT\t";
  cout << "z [m]: " << z;
  cout << "\tx [mm]: " << x*1.e3; 
  cout << "\ty [mm]: " << y*1.e3;
  cout << "\tpx [GeV]: " << px;
  cout << "\tpy [GeV]: " << py;
  cout << "\tpz [GeV]: " << pz;
  cout << "\tsx: " << sx;
  cout << "\tsy: " << sy << endl;
}

void ProtonTransport::simple_rectangular_dipole(double L, double K0L){
  numb_of_obj_uses[2]+=1;
  do_shift(Magnet{"dipole", numb_of_obj_uses[2]}, "subtract");

  if (fabs(K0L) < 1.e-15)
  {
    simple_drift(L);
    return;
  }
  z += L;
  x += L*sx + L*0.5*K0L*beam_energy/pz; // length * initial slope + length * half of angle (from geometry) * correction due to energy loss
  y += L*sy;
  sx += K0L*beam_energy/pz;
  //sy does not change

  do_shift(Magnet{"dipole", numb_of_obj_uses[2]}, "addict");
}

void ProtonTransport::simple_horizontal_kicker(double L, double HKICK){
  numb_of_obj_uses[3]+=1;
  do_shift(Magnet{"horizontal_kicker", numb_of_obj_uses[3]}, "subtract");


  if (fabs(HKICK) < 1.e-15)
  {
    simple_drift(L);
    return;
  }
  z += L;
  x += L*sx + L*0.5*HKICK*beam_energy/pz; // length * initial slope + length * half of angle (from geometry) * correction due to energy loss
  y += L*sy;
  sx += HKICK*beam_energy/pz;

  do_shift(Magnet{"horizontal_kicker", numb_of_obj_uses[3]}, "addict");

}

void ProtonTransport::simple_vertical_kicker(double L, double VKICK){
  numb_of_obj_uses[4]+=1;
  do_shift(Magnet{"vertical_kicker", numb_of_obj_uses[3]}, "subtract");

  if (fabs(VKICK) < 1.e-15)
  {
    simple_drift(L);
    return;
  }
  z += L;
  x += L*sx;
  y += L*sy + L*0.5*VKICK*beam_energy/pz; // length * initial slope + length * half of angle (from geometry) * correction due to energy loss
  sy += VKICK*beam_energy/pz;

  do_shift(Magnet{"vertical_kicker", numb_of_obj_uses[3]}, "addict"); 
}


void ProtonTransport::set_shift(Magnet m, Shift sh){ //1 quadrupole; number;axis 1x 2y 3z; value
  magnet_to_shift[m] = sh;
}

void ProtonTransport::do_shift(Magnet m, string command) {
  if (command == "addict") {
    x = magnet_to_shift[m].x_shift + x; 
    y = magnet_to_shift[m].x_shift + y; 
    z = magnet_to_shift[m].x_shift + z; 
  } else if (command = "subtract") {
    x = magnet_to_shift[m].x_shift - x; 
    y = magnet_to_shift[m].x_shift - y; 
    z = magnet_to_shift[m].x_shift - z; 
  } else {
    std::cout << "No such command" << std::endl;
  }
}

void ProtonTransport::simple_quadrupole(double L, double K1L, bool verbose=false){
  numb_of_obj_uses[1]+=1;

  if (fabs(K1L) < 1.e-15)
  {
    simple_drift(L);
    return;
  }

  do_shift(Magnet{"quadrupole", numb_of_obj_uses[1]}, "subtract"); 



  z += L;
  double qk  = sqrt((fabs(K1L) * beam_energy)/(pz * L));
  double qkl = qk * L;
  double x_tmp = x;
  double y_tmp = y;

  if (K1L >= 0.) //horizontal focussing
  {
    fabs(x)  > 1.e-15 ? x =  cos(qkl) * x       : x = 0.;
    fabs(sx) > 1.e-15 ? x += sin(qkl) * sx / qk : x += 0.;

    fabs(y)  > 1.e-15 ? y = cosh(qkl) * y        : y = 0.;
    fabs(sy) > 1.e-15 ? y += sinh(qkl) * sy / qk : y += 0.;

    fabs(sx) > 1.e-15 ? sx = cos(qkl) * sx           : sx = 0.;
    fabs(x_tmp)  > 1.e-15 ? sx += -qk * sin(qkl) * x_tmp : sx += 0.;

    fabs(sy) > 1.e-15 ? sy = cosh(qkl) * sy          : sy = 0.;
    fabs(y_tmp)  > 1.e-15 ? sy += qk * sinh(qkl) * y_tmp : sy += 0.;
  }
  else //vertical focussing
  {
    fabs(y)  > 1.e-15 ? y =  cos(qkl) * y       : y = 0.;
    fabs(sy) > 1.e-15 ? y += sin(qkl) * sy / qk : y += 0.;

    fabs(x)  > 1.e-15 ? x = cosh(qkl) * x        : x = 0.;
    fabs(sx) > 1.e-15 ? x += sinh(qkl) * sx / qk : x += 0.;

    fabs(sy) > 1.e-15 ? sy = cos(qkl) * sy           : sy = 0.;
    fabs(y_tmp)  > 1.e-15 ? sy += -qk * sin(qkl) * y_tmp : sy += 0.;

    fabs(sx) > 1.e-15 ? sx = cosh(qkl) * sx          : sx = 0.;
    fabs(x_tmp)  > 1.e-15 ? sx += qk * sinh(qkl) * x_tmp : sx += 0.;
  }
  

  do_shift(Magnet{"quadrupole", numb_of_obj_uses[1]}, "addict"); 


// std::cout<<numb_of_obj_uses<<'\n';
  
  if (!verbose) return;
  cout << "QUADRUPOLE\t";
  cout << "z [m]: " << z;
  cout << "\tx [mm]: " << x*1.e3; 
  cout << "\ty [mm]: " << y*1.e3;
  cout << "\tpx [GeV]: " << px;
  cout << "\tpy [GeV]: " << py;
  cout << "\tpz [GeV]: " << pz;
  cout << "\tsx: " << sx;
  cout << "\tsy: " << sy << endl;
}

/**
\brief Extract beam elements from twiss file.

Twiss files may provide 
*/
void ProtonTransport::PrepareBeamline(string filename, bool verbose=false){

  vector <string> sorted_param; //type, S, L, HKICK, VKICK, K0L, K1L, K2L, K3L, APERTYPE, APER_1, APER_2, APER_3, APER_4, X, Y, PX, PY
  vector <string> unsorted_name;
  vector <string> unsorted_param;

  int n_elements = 18;
  int sorting_order[n_elements];
  for (int a=0; a<n_elements; a++) sorting_order[a] = 0;
  string sorting_order_names[n_elements] = {"KEYWORD", "S", "L", "HKICK", "VKICK", "K0L", "K1L", "K2L", "K3L", "APERTYPE", "APER_1", "APER_2", "APER_3", "APER_4", "X", "Y", "PX", "PY"};

  bool IsIP1 = false;

  ifstream in;
  in.open(filename.c_str());
  if (access(filename.c_str(), F_OK)) {cout << "ERROR! No file named: " << filename << endl; return;}
  
  while (!in.eof())
  {
    string line;
    if (in.peek() == 64) //64 is '@' symbol
    {
      //these lines are comments, not used for now in the code so can be skipped
      in.ignore(5000, '\n');
      continue;
    }
    if (in.peek() == 42) //64 is '*' symbol
    {
      //this line contains names of variables; this can differ between twiss files as order depends on parameters given at MAD-X generation step
      getline(in, line);
      istringstream ss(line);
      for (string keyword; ss >> keyword; ) unsorted_name.push_back(keyword);
    
      for (unsigned int a = 0; a<unsorted_name.size(); a++) 
        for (int b=0; b<n_elements; b++)
          if ((unsorted_name.at(a)).compare(sorting_order_names[b]) == 0) sorting_order[b] = a-1; // -1 because description starts with '*' symbol whereas values does not have it

      for (int a=0; a<n_elements; a++)
      {
        if ((a < 9) && sorting_order[a] == 0) {cout << "ERROR! Key element: " << sorting_order_names[a] << " is missing in Twiss file!" << endl; return;}
        if ((a >= 9) && (a < 14) && sorting_order[a] == 0) {cout << "WARNING! Information about aperture is missing. Will not be considered." << endl;}
        if ((a >= 14) && (a < 18) && sorting_order[a] == 0) {cout << "WARNING! Information about " << sorting_order_names[a] << " is missing. Will not be considered." << endl;}
      }
      
      continue;
    }
    if (sorting_order[0] == 0) {cout << "ERROR! In Twiss file there is no line starting with '*' which defines element type..." << endl; return;}
    
    if (in.peek() == 36) //64 is '@' symbol
    {
      //these lines MAD-X parameter types, can be skipped
      in.ignore(5000, '\n');
      continue;
    }
    
    if (in.peek() == EOF) break;
    
    if (verbose) for (int b=0; b<n_elements; b++) cout << sorting_order[b] << endl;
    
    getline(in, line);
    istringstream ss(line);
    unsorted_param.clear();
    for (string value; ss >> value; )
    {
      if (value.compare("IP1") != 0) IsIP1 = true;
      unsorted_param.push_back(value);
    }
    if (!IsIP1) continue; //some twiss files start before the IP... also position should be counted from IP1
    
    sorted_param.clear();
    for (int a=0; a<n_elements; a++) sorted_param.push_back(unsorted_param.at(sorting_order[a]));
    element.push_back(sorted_param);
    
  }
}

void ProtonTransport::simple_tracking(double obs_point){

  int m_process_code;
  vector<float> *m_px;
  vector<float> *m_py;
  vector<float> *m_pz;
  vector<float> *m_e;
  
  TBranch        *b_process_code;   //!
  TBranch        *b_px;   //!
  TBranch        *b_py;   //!
  TBranch        *b_pz;   //!
  TBranch        *b_e;   //!

  m_px = 0;
  m_py = 0;
  m_pz = 0;
  m_e = 0;


  TFile *f = new TFile("pythia8_13TeV_protons_100k.root", "READ");
  TTree * ntuple;
  f->GetObject("ntuple",ntuple);

  TH1F * h_sigma;
  f->GetObject("sigma",h_sigma);
  TH1F * h_eff;
  f->GetObject("efficiency",h_eff);

  gROOT->ProcessLine("#include <vector>");

  ntuple->SetMakeClass(1);

  ntuple->SetBranchAddress("process_code", &m_process_code, &b_process_code);
  ntuple->SetBranchAddress("px", &m_px, &b_px);
  ntuple->SetBranchAddress("py", &m_py, &b_py);
  ntuple->SetBranchAddress("pz", &m_pz, &b_pz);
  ntuple->SetBranchAddress("e", &m_e, &b_e);

  int nevents = ntuple->GetEntries();
  
  int n_process_code;
  float n_px, n_py, n_pz, n_e;
  float n_x, n_y, n_sx, n_sy;
  TFile * p = new TFile("pythia8_13TeV_protons_100k_transported_shifted_205m_beta40cm_6500GeV_y-185murad.root","recreate");
  TH1F * cs = new TH1F("sigma", "cross-section [mb]", 1, 0., 1.);
  TH1F * efficiency = new TH1F("efficiency", "efficiency", 1, 0., 1.);
  TTree * tree = new TTree("ntuple", "ntuple");
  tree->Branch("process_code", &n_process_code);
//  tree->Branch("n_particles", &n_particles);
  tree->Branch("px", &n_px);
  tree->Branch("py", &n_py);
  tree->Branch("pz", &n_pz);
  tree->Branch("e", &n_e);
  tree->Branch("x", &n_x);
  tree->Branch("y", &n_y);
  tree->Branch("sx", &n_sx);
  tree->Branch("sy", &n_sy);
  
  for (int evt=0; evt<nevents; evt++)
  {
    ntuple->GetEntry(evt);

//  for (double E=6500.; E<=1.00001*7000.; E += 100.)
  {
  //  for (double x0 = 2.e-5; x0 >= -1.000001*2.e-5; x0 -= 1.e-5)
    {
    // for (double y0 = -2.e-5; y0 <= 1.000001*2.e-5; y0 += 1.e-5)
      {
      //  for (double px0 = 0.4e0; px0 >=-1.000001*0.4e0; px0 -= 0.2e0)
        {
        //  for (double py0 = -0.4e0; py0 <=1.000001*0.4e0; py0 += 0.2e0)
          {
//            double z0 = 0.;
//double E=7000., x0=0., y0=0., px0=0., py0=0.;            
 //           cout << E << "\t" << x0 << "\t" << y0 << "\t" << z0 << "\t" << px0 << "\t" << py0 << "\t";

  x = 0.;
  y = 0.;
  z = 0.;
  px = m_px->at(0);
  py = m_py->at(0) + 140.e-6*6500.;
  pz = m_pz->at(0);
  sx = px/pz;
  sy = py/pz;

  BeampipesAreSeparated = false;

  bool Observe = false, Observed = false;
  for (unsigned int a=0; a<element.size(); a++)
  {  
    if (fabs(z+stod(element[a].at(2)) - obs_point) < 1.e-3 && !Observe && !Observed) {Observed = true; Observe = true;}
    else Observe = false;
    if ((element[a].at(0)).compare("\"MARKER\"") == 0) ProtonTransport::Marker(Observe); //Marker(true) will return proton x, y, z, px, py, pz at position of marker
    else if ((element[a].at(0)).compare("\"DRIFT\"") == 0) ProtonTransport::simple_drift(stod(element[a].at(2)), Observe); // L
    else if ((element[a].at(0)).compare("\"RBEND\"") == 0) ProtonTransport::simple_rectangular_dipole(stod(element[a].at(2)), stod(element[a].at(5))); //L, K0L
    else if ((element[a].at(0)).compare("\"HKICKER\"") == 0) ProtonTransport::simple_horizontal_kicker(stod(element[a].at(2)), stod(element[a].at(3))); //L, HKICK
    else if ((element[a].at(0)).compare("\"VKICKER\"") == 0) ProtonTransport::simple_vertical_kicker(stod(element[a].at(2)), stod(element[a].at(4))); //L, VKICK
    else if ((element[a].at(0)).compare("\"QUADRUPOLE\"") == 0) ProtonTransport::simple_quadrupole(stod(element[a].at(2)), stod(element[a].at(6)), Observe); //L, K1L 
    else if ((element[a].at(0)).compare("\"MULTIPOLE\"") == 0)
    {
      if ( fabs(stod(element[a].at(5))) > 1.e-10 )
      {
        cout << "Warning! MULTIPOLE taken as rectangular dipole! Check in twiss files if this is correct! Position: " << element[a].at(1) << endl;
        ProtonTransport::simple_rectangular_dipole(stod(element[a].at(2)), stod(element[a].at(5))); //L, K0L
      }
      else if ( fabs(stod(element[a].at(3))) > 1.e-10 )
      {
        cout << "Warning! MULTIPOLE taken as horizontal kicker! Check in twiss files if this is correct! Position: " << element[a].at(1) << endl;
        ProtonTransport::simple_horizontal_kicker(stod(element[a].at(2)), stod(element[a].at(3))); //L, HKICK
      }
      else if ( fabs(stod(element[a].at(4))) > 1.e-10 )
      {
        cout << "Warning! MULTIPOLE taken as vertical kicker! Check in twiss files if this is correct! Position: " << element[a].at(1) << endl;
        ProtonTransport::simple_vertical_kicker(stod(element[a].at(2)), stod(element[a].at(4))); //L, VKICK
      }
      else if ( fabs(stod(element[a].at(6))) > 1.e-10 )
      {
        cout << "Warning! MULTIPOLE taken as quadrupole! Check in twiss files if this is correct! Position: " << element[a].at(1) << endl;
        ProtonTransport::simple_quadrupole(stod(element[a].at(2)), stod(element[a].at(6))); //L, K1L
      }
      else
      {
//TODO        cout << "Warning! MULTIPOLE taken as drift! Check in twiss files if this is correct! Position: " << element[a].at(1) << endl;
        ProtonTransport::simple_drift(stod(element[a].at(2)), Observe); // L
      }
    }
/*
   The following elements are taken as a drift (if L!=0) or monitor (if L=0):
      * SOLENOID (TODO)
      * RCOLLIMATOR (TODO)
      * MONITOR (L=0 anyway)
      * PLACEHOLDER
      * INSTRUMENT
*/
    else
    {
      if (stod(element[a].at(2)) > 1.e-10) ProtonTransport::simple_drift(stod(element[a].at(2)));
      else ProtonTransport::Marker(Observe);
    }
    
    if (z > 130. && !BeampipesAreSeparated)
    {
      BeampipesAreSeparated = true;
      x += BeampipeSeparation;
    }
    if (z > obs_point)
    {
      //cout << element[a].at(0) << "\t" << z << "\t" << x  << "\t" << sx << "\t" << sx*(z - obs_point) << endl;
      n_process_code = m_process_code;
      n_px = m_px->at(0);
      n_py = m_py->at(0);
      n_pz = m_pz->at(0);
      n_e = m_e->at(0);
      n_x = x - sx*(z - obs_point);
      n_y = y - sy*(z - obs_point);
      n_sx = sx;
      n_sy = sy;
      tree->Fill();
      break;
    }

  
}


    		//std::fill(numb_of_obj_uses.begin(), numb_of_obj_uses.end(),0); //placed here it should work 	NEEDS TO BE A VECTOR WITH ENTRY CORESPONDING OBJ TYPE
			for (int i = 0; i < 5; ++i)
			{
				numb_of_obj_uses[i]=0;
			}


}

}

}

}

}

}

  cs->Fill(0.5, h_sigma->GetBinContent(1));
  cs->Write();
  efficiency->Fill(0.5, h_eff->GetBinContent(1));
  efficiency->Write();
  tree->Write();
//  p->Write();
  p->Close();

}

// 6 quadrupoles, 2 dipoles, 5 horizotal kickers and 5 vertical kickers
int main() {
  ProtonTransport * p;
  //p.prepare_beamline("alfaTwiss1.txt_beta30cm_6500GeV_y140murad");
  p = new ProtonTransport;
  //p->PrepareBeamline("optics_PPSS_2020/alfaTwiss1.txt_beta30cm_6500GeV_y140murad", false);
  p->PrepareBeamline("optics_PPSS_2020/alfaTwiss1.txt_beta40cm_6500GeV_y-185murad", false);
  //p->set_shift(1,3,2,0.0004); //1 quadrupole 2 dipole 3hkicker 4vkicker; number 1,2... ;axis 1x 2y 3z 4strength; value //- only those are currently working
  p->set_shift(Magnet{"dipole", 2}, Shift{}); //kickers and dipole not working for some reason, or they don't matter at all?
  Shift s;
  s.x_shift(0.1);
  p->set_shift(Magnet{"dipole", 1}, s); //kickers and dipole not working for some reason, or they don't matter at all?
  

  p->simple_tracking(205.);
  //p.simple_tracking(58.3145);
  //p.simple_tracking(44.742);
  //p.simple_tracking(31.529);
  //p.simple_tracking(29.335);
  //p.simple_tracking(21.564);


  delete p;
  return 0;
}
