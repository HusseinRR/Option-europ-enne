//g++ main_gabriel.cpp vecteur.hpp Matrice.hpp -o main_gabriel.exe -Wall ; .\main_gabriel.exe
#include "vecteur.hpp"
#include "Matrice.hpp"

using namespace std;

int main(){
    cout << "Test en reel\n";
    /*
    //test de la classe vector
    vector V={1,0};
    vector W={0,1};
    cout<<"V = "<<V<<"\n";
    cout<<"W = "<<W<<"\n";
    cout<<"V+W = "<<V+W<<"\n";
    cout<<"V-W = "<<V-W<<"\n";
    double X=V|W;
    cout<<"V*W = "<<X<<"\n";*/

    //test de la classe matrice

    Matrice<double> J(3,2);
    cout<<"Matrice J = "<<J;
    J.remplissage();
    int n=10;
    Matrice<double> M(n,n);
    for(int i=0;i<=n;i++)
    {M(i,i)=2.;
     if(i>0) M(i,i-1)=-1;
     if(i<n) M(i,i+1)=-1;
    }
    M.remplissage();
    Matrice<double> I(n,n);
    for(int i=0;i<=n;i++) I(i,i)=1.;
    Matrice<double> operation=(M*3.-I*2.)/2.;
    cout<<"(3.*M-2*I)/2"<<operation<<"\n";
    operation.remplissage();
    cout<<"nouvelle matrice\n";
    Matrice<double> T(n,n+1);
    for(int i=0;i<=n;i++) T(i,n)=i+1;// on accède avec i de 0 a n
    T.remplissage();
    T.supprime(2,10);// colonnes entre 0 et m-1 puis lignes entre 0 et n-1
    T.remplissage();
    Matrice<double> MT=M+T;
    MT.remplissage();


}