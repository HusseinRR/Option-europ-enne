#include "iostream"
#include "affiche.h"
#include "vecteur.hpp"


int main(){
    afficher("HI");
    return 0;
    vector V(1,0);
    vector W(0,1);
    cout<<V;
    cout<<W;
    cout<<V+W;
    cout<<V-W;
    cout<<V|W;//pdt scalaire

}