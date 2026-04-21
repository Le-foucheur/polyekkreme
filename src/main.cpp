#include <stdio.h>
#include <cmath>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>

#include "pendule.h"
#include "maestro.h"
#include "tui.h"

void my_handler(int s)
{
    printf("\x1b[?1049l\x1b[?25h\x1b[?47l");
    exit(s);
}

int main() {

    /*
        Initialisation Générales
    */
    
    // les differente config
    bool resolution = true;
    double tmax = -1;
    double pas = 0.01;
    double grav = 9.81;
    int nb_pendule_max = 0;
    bool pendule_info = true;
    std::string id_print = "none";
    bool custom_pendulum = false;
    double theta0 = 0.;

    std::ifstream is_file("./inputs/config.cfg");
    if (!is_file.good()){
        fprintf(stderr, "impossible d’ouvrir le fichier ./inputs/config.cfg\n");
        exit(1);
    }

    std::string line;
    while (!is_file.eof()){
        std::getline(is_file >> std::ws, line);

        //on regarde pas si c'est un commentaire
        if (line.at(0) != '#'){
            std::istringstream is_line(line);

            std::string key;
            std::string value;

            if (std::getline(is_line >> std::ws, key, '=')) {
                if (std::getline(is_line >> std::ws, value, '#')){

                    try {
                        if (key == "tmax " || key == "tmax") {

                            tmax = std::stod(value);

                        } else if (key == "dt " || key == "dt") {

                            pas = std::stod(value);

                        } else if (key == "gravity " || key == "gravity") {

                            grav = std::stod(value);

                        } else if (key == "nb_pendulums " || key == "nb_pendulums") {
                            
                            nb_pendule_max = std::stoi(value);

                        } else if (key == "pendulums_info " || key == "pendulums_info") {

                            if (value == "false" || value == "false "){
                                pendule_info = false;
                            }

                        } else if (key == "id_print " || key == "id_print") {

                            id_print = value;

                        } else if (key == "custom_pendulum " || key == "custom_pendulum") {

                            if (value == "true" || value == "true "){
                                custom_pendulum = true;
                            }

                        } else if (key == "resolution " || key == "resolution") {

                            if (value == "false" || value == "false "){
                                resolution = false;
                            }

                        } else if (key == "theta0 " || key == "theta0") {

                            theta0 = std::stod(value);

                        }
                    } catch (...) {
                        fprintf(stderr, "syntaxe error on %s in config.cfg\n", key.c_str());
                        exit(1);
                    }
                }
            }

        }
    }

    is_file.close();

    /*
        Initialisation du TUI
    */
    signal(SIGINT, my_handler);
    
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    TUI tuigauche = TUI((w.ws_col - 1) / 3, w.ws_row, true);
    TUI_PENDULE tuidroit = TUI_PENDULE( 2 * (w.ws_col - 1) / 3, w.ws_row, tuigauche.w() + 1, resolution);
    /*
        Fin initialisation du TUI
    */


    // init maestro
    Maestro maitre = Maestro(nb_pendule_max, pas, tmax, grav);


    /*
        Initialisation des Pendules
    */

    // Cas ou l’utilisateur ne genere pas de pendule custom
    if (!custom_pendulum){
        Pendule* ancre = new Pendule(0, 1, 1, theta0, 0);
        
        maitre.add_pendule(ancre);
        
        
        for(int i = 1; i < nb_pendule_max; i++){
            Pendule* parent = maitre.get_pendule()[maitre.nb_p() - 1];
            
            Pendule* nouveau = new Pendule(i, 1, 1, theta0, 0, parent);
            
            maitre.add_pendule(nouveau);
            
        }

    // Cas de pendule custom
    } else {
        std::ifstream pendulefile("./inputs/pendule.csv");
        if (!pendulefile.good()){
            fprintf(stderr, "impossible d’ouvrir le fichier ./inputs/pendule.csv\n");
            exit(1);
        }

        std::string poubelle;
        std::getline(pendulefile, poubelle);

        while(!pendulefile.eof()){
            std::getline(pendulefile, line);

            if (line != ""){
                std::istringstream is_line(line);

                int id;
                double masse;
                double longueur;
                double theta;
                double omega;

                std::string tmpinfo;
                try {

                    std::getline(is_line, tmpinfo, ';');
                    id = std::stoi(tmpinfo);

                    std::getline(is_line, tmpinfo, ';');
                    masse = std::stod(tmpinfo);

                    std::getline(is_line, tmpinfo, ';');
                    longueur = std::stod(tmpinfo);

                    std::getline(is_line, tmpinfo, ';');
                    theta = std::stod(tmpinfo);

                    std::getline(is_line, tmpinfo, ';');
                    omega = std::stod(tmpinfo);

                } catch (...) {
                    fprintf(stderr, "syntaxe error in pendule.csv\n");
                    exit(1);
                }

                if (maitre.nb_p() == 0){

                    Pendule* ancre = new Pendule(id, masse, longueur, theta, omega);

                    maitre.add_pendule(ancre);

                } else {

                    Pendule* parent = maitre.get_pendule()[maitre.nb_p() - 1];

                    Pendule* nouveau = new Pendule(id, masse, longueur, theta, omega, parent);

                    maitre.add_pendule(nouveau);
                }
            }
        }

        pendulefile.close();
    }

    // gestion de id_print pour savoir quel sont les pendules à afficher dans les infos à gauche

    std::vector<int> id_show;

    if ((id_print != "none" || id_print != "none ") && id_print.find("-") != std::string::npos) {
        std::string id1;
        std::string id2;
        std::istringstream is_id_print(id_print);

        std::getline(is_id_print, id1, ':');
        std::getline(is_id_print, id2);

        try {
            
            for (int i = std::stoi(id1); i <= std::stoi(id2); i++){
                id_show.push_back(i);
            }

        } catch (...) {
            fprintf(stderr, "syntaxe error on id_print in config.cfg\n");
            exit(1);
        }
    } else if (id_print == "none" || id_print == "none"){
        for (int i = 0; i < maitre.nb_p(); i++){
            id_show.push_back(i);
        }
    } else {

        std::string id;
        std::istringstream is_id_print(id_print);

        try {

            while(getline(is_id_print, id, ',')){
                id_show.push_back(std::stoi(id));
            }

        } catch (...) {
            fprintf(stderr, "syntaxe error on id_print in config.cfg\n");
            exit(1);
        }
    }

    // initialisation des fichiers de sauvegarde des données
    maitre.init_save();

    /*
        Début de l’affichage
    */
    float dodo = 1.f;
    
    const double FPS = 300.0;
    const long micro_per_frame = (long)(1000000.0 / FPS); 

    int steps_per_frame = (int)((1.0 / FPS) / maitre.dt());
    if (steps_per_frame < 1) {
        steps_per_frame = 1;
    }

    printf("\x1b[?47h\x1b[?1049h\x1b[?25l");
    while (maitre.t() < maitre.tmax() || maitre.tmax() < 0) {
        clock_t t = clock();
        

        // print les info à gauche
        tuigauche.add_info(pendule_info, maitre, &id_show);
        tuigauche.print_screen();
        
        //print les deux pendules
        tuidroit.draw_pendule(maitre);
        tuidroit.transfere_sub_to_screen();
        tuidroit.print_screen();

        maitre.save();

        for (int s = 0; s < steps_per_frame; s++){
            maitre.calcule_temp_plus_1();
        }
        

        //clean les pendules pour retiré les artefacte
        tuidroit.sub_screen_clean();
        tuidroit.screen_clean();
        tuigauche.screen_clean();
        tuidroit.transfere_sub_to_screen();


        //on retire le temps consomé par le procésus pour rendre le tout fluide
        t = clock() - t;
        dodo = (float)t / CLOCKS_PER_SEC ; 
        if (dodo <= micro_per_frame)
        {
            usleep((useconds_t)(micro_per_frame - dodo));
        }
    }
    printf("\x1b[?1049l\x1b[?25h\x1b[?47l");

    return 0;
}

