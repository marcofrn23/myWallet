#ifndef PORTFOLIO_H
#define PORTFOLIO_H

class Portfolio {

public: 
	Portfolio (); 
	Portfolio (unsigned num_asset);
	Portfolio (vec &w); 
	
	int set_weights (vec &w);   
	vec get_weights();
  
	double get_return(Market &m); 
	double get_sigma(Market &m);
  
	void Frontier(Market &m, double  pRet); 
	bool isEfficient(Market &m);

	void Mvp(Market &m);
 	Portfolio Mvp_new(Market &m); 
	Portfolio ZeroBetaPortfolio(Market &m);   
  
	void set_name(string n); 
	string get_name(); 
  
	void out(Market &m); 
  void out(Market &m, string filename) 

private: 
	string name ="Portfolio 1"; 
	vec weights; 
};

#endif
