#include <ostream>
//#include<vecteur.hpp>
#include <map>
#include <vector>
#include <utility>
#include <cmath>
#include <iostream>
using namespace std;

typedef pair<int,int> Pint;
inline bool operator<(const Pint& ij1, const Pint& ij2)
{
  if (ij1.first<ij2.first){
    return true;
  }
  else if ((ij1.first==ij2.first) and (ij1.second<ij2.second))
  {
    return true;
  }
  else{
    return false;
  }
  
}
inline ostream& operator<<(ostream& os,const Pint& ij)
{
    return os<< '('<< ij.first<<','<< ij.second<<')';
    }

//---------------------------------------------------------------------------
//    Classe Matrice<T>
//---------------------------------------------------------------------------
template <typename T>
//ne pas faire d'heritage, juste mettre en argument une std map
class Matrice //public map<Pint,T> on arrete l'heritage, on fait une sparse et on utilise le dictionnaire map
{public:
  int m,n; //dimensions de la matrice
  map<Pint,T> coeffs;
  Matrice(int mi=0,int ni=0): m(mi),n(ni),coeffs(){}

  friend ostream& operator<<(ostream& os,const Matrice<T>& M){

    os<<" Matrice de "<<M.m<<" colonnes et "<<M.n<<" lignes"<<"\n";
    for (auto& pair : M.coeffs) {
        os<<pair.first<< " : "<<pair.second<<"\n";
    }
    return os;
  }

  T& operator()(int i, int j){
    pair<int,int> key = {i,j};
    return coeffs[key];//modif possible, 
  }

  T operator()(int i, int j) const{
    pair<int,int> key = {i,j};
    if (coeffs.find(key)==coeffs.end())//pas trouver
    {
        return 0;
    }
    else{
        return coeffs.find(key)->second;
    }
  }
  void supprime(int i,int j){
    pair<int,int> key = {i,j};
    coeffs.erase(key);
    return;
  }

  Matrice<T> operator+(const Matrice<T>& M){
    Matrice<T> result =*this;
    for (auto& pair : M.coeffs) {
        auto& key = pair.first;
        auto& value = pair.second;
        result.coeffs[key] += value;
    }
    return result;
  }
  Matrice<T>& operator+=(const Matrice<T>& M){
    return (*this)+M;
  }  

  Matrice<T> operator-(const Matrice<T>& M){
    Matrice<T> result =*this;
    for (auto& pair : M.coeffs) {
        auto& key = pair.first;
        auto& value = pair.second;
        result.coeffs[key] -= value;
    }
    return result;
  }
  Matrice<T>& operator-=(const Matrice<T>& M){
    return (*this)-M;
  }

  Matrice<T> operator*(const T& s){
    Matrice<T> result =*this;
    for (auto& pair : (*this).coeffs) {
        auto& key = pair.first;
        auto& value = pair.second;
        result.coeffs[key] = value*s;
    }
    return result;
  }
  Matrice<T>& operator*=(const T& s){
    return (*this)*s;
  }
  

  Matrice<T> operator/(const T& s){
    Matrice<T> result =*this;
    for (auto& pair : (*this).coeffs) {
        auto& key = pair.first;
        auto& value = pair.second;
        result.coeffs[key] = value/s;
    }
    return result;
  }
  Matrice<T>& operator/=(const T& s){
    return (*this)/s;
  }
  
  void remplissage() const{
    //matrice de taille n,m
    long unsigned int n=(*this).n;
    long unsigned int m=(*this).m;
    cout<<"Matrice de taille : "<<(*this).m<<" colonnes * "<<(*this).n<<" lignes \n";
    for(size_t i = 0; i < n; i++){
        for (size_t j = 0; j < m; j++)
        {
            //pair<int,int> key={i,j};
            T value= (*this)(j,i);
            cout << value<<" ";
        }
        cout<<"\n";
    }
    cout<<"\n";
  }
};

template <typename T>
class Matrice_sym: public Matrice<T>
{public:
    int m,n;
    map<Pint,T> coeffs;
    Matrice_sym (int mi=0,int ni=0):Matrice<T>(mi,ni),m(mi),n(ni),coeffs() {}

    //on optimisera plus tard
};

template <typename T>
class Matrice_nonsym: public Matrice<T>
{public:
    int m,n;
    map<Pint,T> coeffs;
    Matrice_nonsym (int mi=0,int ni=0):Matrice<T>(mi,ni),m(mi),n(ni),coeffs() {}
};