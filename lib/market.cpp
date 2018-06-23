#include <iostream>
#include <fstream>
#include <cstring>
#include <itpp/itbase.h>
#include "Market.h"

using namespace std;
using namespace itpp;

market::market(string M, string a, string m) { //Se l'utente non dà questi input significa che li vuole dare dopo
    if (!M.empty()) {cout << M <<endl; name = M;}
    if (!a.empty() && !m.empty()) { // controlla se ha fornito i nomi dei file di input, in questo caso prova a reperire i dati
      try {
        if (!get(a, m)) {
          throw false; // se non trova alcun file adeguato lancia l'eccezione
        }
      }
      catch(bool open) {
        cout << "ERROR: No default input file available!" <<endl;
      }
    }
}
void market::update(string asset_path, string mat_path) {
    try {
      if (!get(asset_path, mat_path)) throw false; //I nomi forniti devono corrispondere a file adeguati e disponibili
    }
    catch(bool open) {
      cout << "ERROR: file is not available!" <<endl;
    }
}

void market::get_file_format(string filename) {

    const unsigned long t = std::string::npos;

    if (filename.find(".txt") != t) {
        format = "TXT";
    }
    else if (filename.find(".csv") != t) {
        format = "CSV";
    }
    else if (filename.find(".xml") != t) {
        format = "XML";
    }
    else format = "Missing";
}

bool market::get(string asset_path, string mat_path) {

    fstream input, input2;
    bool file = false;
    if (asset_path.find("assets") && mat_path.find("covariance") == std::string::npos) return file;
    ASSET_PATH = asset_path;
    MAT_PATH = mat_path;
    get_file_format(ASSET_PATH);
    get_file_format(MAT_PATH);
    file = true;

    unsigned i = 0;
    unsigned j = 0;
    if (format == "TXT") {
        input.open(ASSET_PATH, ios::in);
        if (!input.is_open()) {cerr << "ERROR: Could not open file!\n"; file = false; return file;}
        while (input.good()) {
            //getline(input, titles[i]);
            input >> asset[i];
            i++;
        }
        input.close();
        NUM_ASSET = i-1;
        i = 0;
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
    if (format == "CSV") {
        input.open(ASSET_PATH, ios::in);
        if (!input.is_open()) {cerr << "ERROR: Could not open file!\n"; file = false; return file;}
        i = 0;
        j = 0;
        char c;
        while (input.good()) {
            //getline(input, titles[i], ',');  //CSV è identico a txt ma ci sono le virgole in mezzo
            input >> asset[i];
            input.get(c);
            i++;
        }
        input.close();
        NUM_ASSET = i;
        i = 0;
        c = ' ';
        input2.open(MAT_PATH, ios::in);
        while (input2.good()) {
            if (i == NUM_ASSET) {i=0; j++;}
            input2 >> covariance[j][i];
            i++;
        }
        input2.close();
    }
    if (format == "XML") {
        char TAG[30];
        input.open(ASSET_PATH, ios::in);
        if (!input.is_open()) {cerr << "ERROR: Could not open file!\n"; file = false; return file;}
        i = 0;
        j = 0;
        char c;
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
    return file;
}


void market::display() {
    update(ASSET_PATH, MAT_PATH);  //Mantiene i dati aggionati se ci sono modifiche
    cout << "Situazione di mercato: " <<endl;
    cout <<"--------- E[r] Var"<<endl;
    for (unsigned i = 0; i<NUM_ASSET; i++) {
        cout <<"Asset n." << i+1 <<" "<<" "<<asset[i] <<" "<<covariance[i][i]<<endl<<endl;
    }
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
