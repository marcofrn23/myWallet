#ifndef MARKET_H
#define MARKET_H

class market {

public:
    market(string MARKET_NAME = "", string asset_path = "", string mat_path = ""); //l'utente può anche non dare alcun argomento

    void update(string asset_path ="", string mat_path = "");
    // usabile sia per rilevare modifiche/spostamenti nei file aperti sia per caricare i dati

    void set_assets(double newval, int pos) {asset[pos] = newval;};
    void set_covariance(double newval, int pos1, int pos2) {covariance[pos1][pos2] = newval;};
    void display();
    void display_info();
    string name;

    // METODI DI INTERFACCIA
    vec get_assets();
    mat get_covariance();

private:
    void get_file_format(string);
    bool get(string asset_path ="", string mat_path =""); // prova a reperire i dati, se non ce la fa restituisce false
    string ASSET_PATH;
    string MAT_PATH;
    string format;        //Formato dei file, deve essere lo stesso per mat.cov. e asset.
    int NUM_ASSET;
    double asset[50];
    double covariance[50][50];
};

#endif
