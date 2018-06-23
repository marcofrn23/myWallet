#include <iostream>
#include <fstream>
#include <cstring>
#include <itpp/itbase.h>
#include "Market.h"

using namespace std;
using namespace itpp;

market::market(string M, string a, string m) { //Se l'utente non dà questi input significa che li vuole dare dopo
    NUM_ASSET = 0;
    if (!M.empty()) {cout << M <<endl; name = M;}
    if (!a.empty() && !m.empty()) { // controlla se ha fornito i nomi dei file di input, in questo caso prova a reperire i dati
      try {
        if (!get(a, m)) {
          throw false; // se non trova alcun file adeguato lancia l'eccezione
        }
      }
      catch(bool open) {
        cerr << "ERROR: No default input file available!" <<endl;
      }
    }
    else get("assets.txt", "covariance.txt");
}
void market::update(string asset_path, string mat_path) {
    try {
      if (!get(asset_path, mat_path)) throw false; //I nomi forniti devono corrispondere a file adeguati e disponibili
    }
    catch(bool open) {
      cerr << "ERROR: file is not available!" <<endl;
    }
}

string market::get_file_format(string filename) {

    string f ="";
    const unsigned long t = std::string::npos;

    if (filename.find(".txt") != t) {
        f = "TXT";
    }
    else if (filename.find(".csv") != t) {
        f = "CSV";
    }
    else if (filename.find(".xml") != t) {
        f = "XML";
    }
    else {f = "Missing"; cout <<"ERROR: Please type input file name correctly"<<endl;}
    return f;
}

bool market::get(string asset_path, string mat_path) {

    fstream input, input2;
    bool file = false;
    if (asset_path.find("assets") && mat_path.find("covariance") == std::string::npos) return file;
    file = true;
    ASSET_PATH = asset_path;
    MAT_PATH = mat_path;
    string f1 = get_file_format(ASSET_PATH);
    string f2 = get_file_format(MAT_PATH);
    if (f1 != f2 && f1!="Missing" && f2!="Missing") format = "MIXED";  //Nel caso capitasse che si ha un file in un formato e l'altro in un altro formato
    else format = f1;

    unsigned i = 0;
    unsigned j = 0;
    if (format == "TXT" || format == "MIXED") {
        if (f1 == "TXT") {
          input.open(ASSET_PATH, ios::in);
          if (!input.is_open()) {cerr << "ERROR: Could not open file/file non existing!\n"; file = false; return file;}
          while (input.good()) {
              //getline(input, titles[i]);
              input >> asset[i];
              i++;
          }
          input.close();
          NUM_ASSET = i-1;
          i = 0;
        }
        if (f2 == "TXT") {
          input2.open(MAT_PATH, ios::in);
          while (input2.good() && j< NUM_ASSET) {
              while (i < NUM_ASSET) {
                  input2 >> covariance[j][i];
                  i++;
              }
              j++;
              i=0;
          }
          input2.close();
        }
    }
    if (format == "CSV" || format == "MIXED") {
        i = 0;
        j = 0;
        char c;
        if (f1 == "CSV") {
          input.open(ASSET_PATH, ios::in);
          if (!input.is_open()) {cerr << "ERROR: Could not open file/file non existing!\n"; file = false; return file;}
          while (input.good()) {
              input >> asset[i];
              input.get(c);
              i++;
          }
          input.close();
          NUM_ASSET = i;
          i = 0;
          c = ' ';
        }
        if (f2 == "CSV") {
          input2.open(MAT_PATH, ios::in);
          while (input2.good()) {
              if (i == NUM_ASSET) {i=0; j++;}
              input2 >> covariance[j][i];
              i++;
          }
          input2.close();
        }
    }
    if (format == "XML" || format == "MIXED") {
        char TAG[30];
        i = 0;
        j = 0;
        char c;
        if (f1 == "XML") {
          input.open(ASSET_PATH, ios::in);
          if (!input.is_open()) {cerr << "ERROR: Could not open file/file non existing!\n"; file = false; return file;}
          while(input.good()) {
              input.get(c);
              if (!isblank(c) && c != '\n') {
                  if (c == '<') {
                      input.getline(TAG, 100, '>');
                  }
                  else {input >> asset[i]; i++;}
              }
          }
          input.close();
          NUM_ASSET = i;
          i = 0;
          c = ' ';
        }
        if (f2 == "XML") {
          input2.open(MAT_PATH, ios::in);
          while (input2.good()) {
              if (i == NUM_ASSET) {i=0; j++;}
              input2.get(c);
              if (!isblank(c) && c != '\n') {
                  if (c == '<') {
                      input2.getline(TAG, 100, '>');
                  }
                  else {
                      input2 >> covariance[j][i];
                      i++;
                  }
              }
          }
          input2.close();
        }
    }
    return file;
}


void market::out() {
    update(ASSET_PATH, MAT_PATH);  //Mantiene i dati aggionati se ci sono modifiche
    cout << "Situazione di mercato: " <<endl;
    cout <<"--------- E[r] Var"<<endl;
    for (unsigned i = 0; i<NUM_ASSET; i++) {
        cout <<"Asset n." << i+1 <<" "<<" "<<asset[i] <<" "<<covariance[i][i]<<endl<<endl;
    }
}

void market::out(string filename) {
    ofstream out;
    out.open(filename, ios::out);
    if (!out.is_open()) {cerr << "ERROR: Could not open/create file\n"; return;}
    update(ASSET_PATH, MAT_PATH);  //Mantiene i dati aggionati se ci sono modifiche
    out << "Situazione di mercato: " <<endl;
    out <<"--------- E[r] Var"<<endl;
    for (unsigned i = 0; i<NUM_ASSET; i++) {
        out <<"Asset n." << i+1 <<" "<<" "<<asset[i] <<" "<<covariance[i][i]<<endl<<endl;
    }
    out.close();
}

void market::display_info() {
    update(ASSET_PATH, MAT_PATH);
    cout << "Formato dei file di input: " <<format<<endl;
    cout << "Numero di assets: " << NUM_ASSET <<endl;
}


vec market::get_assets() {
   update(ASSET_PATH, MAT_PATH);
   vec assets(NUM_ASSET);
   for(unsigned i = 0; i<NUM_ASSET; i++) {
       assets[i] = asset[i];
   }
   return assets;
}
mat market::get_covariance() {
   update(ASSET_PATH, MAT_PATH);
   mat covar(NUM_ASSET, NUM_ASSET);
   for(unsigned i = 0; i<NUM_ASSET; i++) {
       for(unsigned j = 0; j<NUM_ASSET; j++) {
           covar(i,j) = covariance[i][j];
       }
   }
   return covar;
}

unsigned market::get_length() {
  return NUM_ASSET;
}
