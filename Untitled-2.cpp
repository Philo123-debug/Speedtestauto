// _popen() permet de récupéré la sortie en plus de l'éxécuter
// char est le tableau
// V1.4
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <regex>
#include <time.h>
#include <chrono>
#include <thread>
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
using namespace std;


void loadbar(int temps, string raison){
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int load = 100;
    temps /= 100;
    for (int i = 0; i < load; i++)
    {
        cout << i << "%" << "[";
        int j = 0;
        SetConsoleTextAttribute(h, 12);
        while (j < i)
        {
            cout << "=";
            j++;
        }
        cout << endl << raison << endl;
        this_thread::sleep_for(chrono::seconds(temps));
        SetConsoleTextAttribute(h, 7);
        system("cls");
    }
}
void verif(double valeurdo, double valeurup, double docible1, double upcible2){
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << "=================================speedtest=======================================" << endl;
    // test dowload
    if(valeurdo <= docible1){
        SetConsoleTextAttribute(h, 12);
        cout << "DOWNLOAD : " << valeurdo << endl;
        SetConsoleTextAttribute(h, 7);
    }else{
        SetConsoleTextAttribute(h, 2);
        cout << "DOWNLOAD : " << valeurdo << endl;
        SetConsoleTextAttribute(h, 7);
    }
    // test upload
    if(valeurup <= upcible2){
        SetConsoleTextAttribute(h, 12);
        cout << "UPLOAD : " << valeurup << endl;
        SetConsoleTextAttribute(h, 7);
    }else{
        SetConsoleTextAttribute(h, 2);
        cout << "UPLOAD : " << valeurup << endl;
        SetConsoleTextAttribute(h, 7);
    }
    cout << "=================================speedtest=======================================" << endl;
}

void algo(){
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 7);
    FILE* pipe;
    char buffer[256];
    int time = 0;
    double docible1 = 0;
    double upcible2 = 0;
    string sortiedow;
    string sortieup;
    string nb_essaie;
    ifstream fichierEntrer("data.txt");
    getline(fichierEntrer, sortiedow);
    getline(fichierEntrer, sortieup);
    getline(fichierEntrer, nb_essaie);
    cout << "===================================== INFOS ===========================================" << endl;
    cout << "Dernière entrée : " << endl <<
     "Download - " << sortiedow << " Mbps" << endl <<
     "Upload - " << sortieup << " Mbps" << endl;
    SetConsoleTextAttribute(h, 12);
    cout << "/!\\ INFOS IMPORTANTE : le .exe doit bien être lancer en Administrateur" << endl << "v2.1.1" << endl;
    SetConsoleTextAttribute(h, 7);
    cout << "=================================speedtest cible=======================================" << endl;
    cout << "Download cible = ";
    cin >> docible1;
    cout << "Upload cible = ";
    cin >> upcible2;
    cout << "=================================speedtest cible=======================================" << endl;
    system("cls");
    ofstream fichierSortie("data.txt");
    fichierSortie << docible1 << endl;
    fichierSortie << upcible2 << endl;
    fichierSortie.close();
    double download_value = 0 ;
    double upload_value = 0;
    string boucle = "true";
    do
    {
    do
    {
    // récupération upload
    pipe = _popen("powershell -Command \"Get-Content (Get-ChildItem 'C:\\ProgramData\\Salad\\logs\\ndm' | Sort-Object LastWriteTime | Select-Object -Last 1).FullName | Select-String 'rate:|Ping:' | findstr \"Upload\"\"", "r");
    fgets(buffer, 256, pipe);
    string Upload(buffer);
    _pclose(pipe);
    
    // récupération download
    pipe = _popen("powershell -Command \"Get-Content (Get-ChildItem 'C:\\ProgramData\\Salad\\logs\\ndm' | Sort-Object LastWriteTime | Select-Object -Last 1).FullName | Select-String 'rate:|Ping:' | findstr \"Download\"\"", "r");
    fgets(buffer, 256, pipe);
    string download(buffer);
    cout << "=================================speedtest=======================================" << endl;
    cout << Upload;
    cout << download;
    cout << "=================================speedtest=======================================" << endl;
    // extraction
    regex rgx("rate:\\s*([0-9]+\\.[0-9]+)");
    smatch match;
    regex_search(download, match, rgx);
    //    cout << match[1] << endl;
    smatch match2;
    regex_search(Upload, match2, rgx);
    //    cout << match2[1] << endl;
    // convertion string --> double
    string string_download = match[1];
    string string_upload = match2[1];
    download_value = stod(string_download) ;
    upload_value = stod(string_upload);
        if(download_value < docible1 | upload_value < upcible2){
            system("cls");
            verif(download_value, upload_value, docible1, upcible2 );
            system("taskkill /f /im Salad.Bowl.Service.exe");
            this_thread::sleep_for(chrono::seconds(5));
            system("taskkill /f /im Salad.Bowl.Service.exe");
            loadbar(60, "kill en cours");
            time = +1;
        }
    } while (download_value < docible1 || upload_value < upcible2);
    cout << "\nProgramme terminer | il y a eu " << time << " tentatives" <<  endl;
    cout << "cycle de 24h pour les vérif" << endl;
    this_thread::sleep_for(chrono::seconds(84400));
    } while (boucle == "true");
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    // (1, "chargement des données");
    algo();
    return 0;
}