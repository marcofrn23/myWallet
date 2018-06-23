#include <iostream>
#include <fstream>
#include <cstring>
#include <itpp/itbase.h>

using namespace std;
using namespace itpp;
#define EPSILON 0.001

Portfolio::Portfolio () {
	weights=zeros(1); 
}

Portfolio::Portfolio (unsigned num_asset) {
	weights=zeros(num_asset);
}

Portfolio::Portfolio (vec &w) {

  try {
		if(!set_weights(w)) throw false;
	}
	int setOutput=set_weights(w); 
  catch(bool set) {
		weights=zeros(w.size()); 
		cerr << "ERROR: an error occured in setting weights: portfolio created, weights set to 0"<<endl;
	}
}

//setWeights function 

bool Portfolio::set_weights (vec &w) {
	
	double sum=0.; 
	for (int i=0; i<w.size(); i++)
		sum+=w(i); 

	if (abs(sum-1) <= EPSILON) {
			weights=w; 
			// cout<<"setweights: portfolio weights successfully set"<<endl; 
			return true; 
		}
	else {
		cerr << "SET_WEIGHTS: unable to set weights \n Weights must sum to 1!"<<endl; 
		return false;
	}
}

vec Portfolio::get_weights() {
	return weights; 
}

double Portfolio::get_return (Market &m) {
	return ((get_weights().transpose()) * m.get_assets())(0); 
}

double Portfolio::get_sigma(Market &m) {
	
	vec w=get_weights(); 
	mat Cov=m.get_covariance(); 
  return (sqrt(w.transpose()*Cov*w)(0));
}

//Efficient Function 

void Portfolio::Frontier (Market &m, double pRet) {
	
	  vec e=m.get_assets(); 
    int no_assets=e.size();
    vec one = ones(no_assets);
    mat CovInv = inv(m.get_covariance());               // inverse of covariance
    mat A = one.transpose()*CovInv*e;
    double a = A(0,0);
    mat B = e.transpose()*CovInv*e;      
    double b = B(0,0);
    mat C = one.transpose()*CovInv*one;  
    double c = C(0,0);
    double d = b*c-a*a;
    mat CovInv_1=CovInv*one; 
    mat CovInv_e=CovInv*e;
    mat g = (CovInv_1 * b - CovInv_e * a) * (1.0/d);
    mat h = (CovInv_e * c - CovInv_1 * a) * (1.0/d);
    mat w = g + h*pRet;

    // cout << "w:" << w <<endl;
    vec pv =w.get_col(0); 
    //cout << "pv: \t "<< pv <<endl; 
	  set_weights(pv);     
}

/*
void Portfolio :: Mvp_not_working (Market &m)
{

	vec e = m.getAssets(); 

	mat Cov = m.getCovariance();

	mat CovInv=Cov.transpose(); 

	int no_assets=e.size(); 
	vec one = ones(no_assets);


	mat C = one.transpose()*CovInv*one;  
	double c=C(0,0);
	mat w= one.transpose()*CovInv; 

	w/=c; 

	vec pv =w.get_row(0);

		//cout << "mvp: \t "<< pv <<endl; 

    setWeights(pv); 

}
*/

void Portfolio::Mvp (Market &m) {  //rende il portfolio in questione l'mvp rispetto al mercato m 

	  vec e=m.get_assets(); 

    int no_assets=e.size();

    vec one = ones(no_assets);

    mat CovInv = inv(m.getCovariance());   

    mat A = one.transpose()*CovInv*e;
    double a = A(0,0);
  
    mat C = one.transpose()*CovInv*one;  
    double c = C(0,0);
    Frontier (m, a/c); 
   
}

Portfolio Portfolio::Mvp_new(Market &m) {

		vec e=m.get_assets(); 				
    int no_assets=e.size();

    vec one = ones(no_assets);

    mat CovInv = inv(m.get_covariance());   

    mat A = one.transpose()*CovInv*e;
    double a = A(0,0);
  
    mat C = one.transpose()*CovInv*one;  
    double c = C(0,0);

   	Portfolio Mvp (no_assets); 

	  Mvp.set_name("Mvp " + m.marketName); 

	  Mvp.Frontier(m, a/c); 

	  return Mvp; 
}


bool Portfolio::isEfficient(Market &m) {

	  int no_assets = (m.get_assets().size());
	  Portfolio portf(no_assets);
	  portf.Mvp(m); 

	  if (portf.get_return(m)>=get_return(m))
		  return false; 
	  else 
		  return true; 
}



Portfolio Portfolio :: ZeroBetaPortfolio (Market &m)   //crea un nuovo portfolio, che è lo zero beta di quello in questione.
{
	  vec e = m.get_assets(); 

	  int no_assets=e.size(); 
	  vec one = ones(no_assets);

	  mat Cov=m.get_covariance(); 
	  mat CovInv=inv(Cov); 
	
	  mat A = one.transpose()*CovInv*e;
	  mat B = e.transpose()*CovInv*e; 
	  mat C = one.transpose()*CovInv*one;  

	  mat D = B*C - A*A; 

	  double erP = get_return(m); 

	  mat erPm (1,1); 

	  erPm(0,0) = erP; 

	  double erZeroBetaP = ((A*inv(C)) - ((D*inv(C*C)) * inv (erPm - (A*inv(C))) )) (0,0); 

	  Portfolio ZeroBetaP (no_assets); 

	  ZeroBetaP.set_name("Zero Beta " + name ); 

	  ZeroBetaP.Frontier(m, erZeroBetaP);

	  return ZeroBetaP; 
}

string Portfolio::get_name () {
	  return name; 
}

void Portfolio::set_name (string n) {
	  name = n; 
}

void Portfolio::out(Market &m) {

	  cout << "\t " << name << endl; 
	  cout << "\n"; 
	  cout << "Weights "<<"\t " << get_weights() << endl; 
	  cout << "Expected return: "<<"\t " << get_return(m) << endl; 
	  cout << "Variance(Risk): "<< "\t\t " << get_sigma(m)<< endl; 
	  cout << "\n"; 
}

void Portfolio::out(Market &m, string filename) {

    m.out(filename);
    ofsteam outp;
    outp.open(filename, ios::app);
    if (!outp.is_open()) {cerr << "ERROR: Could not open/create file\n"; return;}
	  outp << "\t " << name << endl; 
	  outp << "\n"; 
	  outp << "Weights "<<"\t " << get_weights() << endl; 
	  outp << "Expected return: "<<"\t " << get_return(m) << endl; 
	  outp << "Variance(Risk): "<< "\t\t " << get_sigma(m)<< endl; 
	  outp << "\n"; 
    //.....
    outp.close();
}
