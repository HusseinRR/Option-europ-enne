//#ifndef VECTEUR_HPP  // Vérifie si VECTEUR_HPP n'a pas encore été défini
//#define VECTEUR_HPP

#include <ostream>
//#include "vecteur.hpp"
#include <vector>
#include <map>
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

  //factorisation LU
  void axpy(double alpha, const Matrice &B) {
    for (size_t i=0; i<n*n; i++) {
        coeffs[i] += alpha*B.coeffs[i];
    }
  }
  void factorLU() {
    for (size_t k=0; k<n; k++) {
        // No pivoting check, assume no zero pivot
        for (size_t i=k+1; i<n; i++) {
            coeffs(i,k) /= coeffs(k,k);
            for (size_t j=k+1; j<n; j++) {
                coeffs(i,j) -= coeffs(i,k)*coeffs(k,j);
            }
        }
    }
}

  
// Solve for x in LU x = b (in-place on x)
void solveLU(vector<T> &x) const {
    // Forward substitution for L
    for (size_t i=1; i<n; i++) {
        double sum = x[i];
        for (size_t j=0; j<i; j++) {
            sum -= coeffs(i,j)*x[j];
        }
        x[i] = sum;
    }
    for (int i=(int)n-1; i>=0; i--) {
        double sum = x[i];
        for (size_t j=i+1; j<n; j++) {
            sum -= coeffs(i,j)*x[j];
        }
        x[i] = sum / coeffs(i,i);
    }
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