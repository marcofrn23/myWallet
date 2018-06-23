#include <iostream>
#include "market.h"
#include "portfolio.h"

int main (int argc const char argv[]) {     //Ho abbozzato un main in base a come ho inteso il tutto

  market mkt("MERCATO EUROPEO", "assets.xml", "covariance.xml"); //se i file non si chiamano assets.* e covariance.* si sputtana
  mkt.out(); //stampa nella console
  mkt.display_info(); //stampa altri dati
  a = mkt.get_length();
  Portfolio pf1(a); //portafoglio vuoto della lung. giusta 
  vec myportf(a);
  cout <<"Inserisci i "<< a <<" valori wi del portafoglio"<<endl;  //nel caso in cui vuole fare il portafoglio come gli pare (?)
  for (unsigned i =0; i<a; i++) {
    cin << myportf(i);
  } 
  Portfolio pf2(myportf);
  pf2.set_name("Pippo");        
  double expr = pf2.get_return(mkt);
  double sigma = pf2.get_sigma(mkt);
  bool efficient = pf2.isEfficient(mkt);
  Portfolio pf3 = pf2.ZeroBetaPortfolio(mkt);

  pf1.Mvp(mkt);
  expr = 0.3;
  pf1.Frontier(mkt, expr)
  pf1.out(mkt);  //display
  pf1.out(mkt, "nuovofilecreatoora.txt"); //crea un file e ci scrive i dati di mkt e pf1 in ordine
  
}
