#include <ostream>
#include<vecteur.hpp>
#include <map>
#include <vector>
#include <utility>
#include <cmath>
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
class Matrice : public map<Pint,T>
{public:
  int m,n; //dimensions de la matrice
  Matrice(int mi=0,int ni=0): map<Pint,T>(), m(mi),n(ni){}

  friend ostream& operator<<(ostream& os,const Matrice<T>& M){

    os<<"Matrice de dim : "<<M.m<<"*"<<M.n<<"\n";
    for (auto& pair : M) {
        os<<pair.first<< " : "<<pair.second<<"\n";
    }
    return os;
  }

  T& operator()(int i, int j){
    pair<int,int> key = {i,j};
    return(*this)[key];//modif possible, 
  }

  T operator()(int i, int j) const{
    pair<int,int> key = {i,j};
    if (this->find(key)==this->end())//pas trouver
    {
        return 0;
    }
    else{
        return this->find(key)->second;
    }
  }
  void supprime(int i,int j){
    pair<int,int> key = {i,j};
    this->erase(key);
    return;
  }

  Matrice<T> operator+(const Matrice<T>& M){
    Matrice<T> result =*this;
    for (auto& pair : M) {
        auto& key = pair.first;
        auto& value = pair.second;
        result[key] += value;
    }
    return result;
  }
  Matrice<T>& operator+=(const Matrice<T>& M){
    return (*this)+M;
  }  

  Matrice<T> operator-(const Matrice<T>& M){
    Matrice<T> result =*this;
    for (auto& pair : M) {
        auto& key = pair.first;
        auto& value = pair.second;
        result[key] -= value;
    }
    return result;
  }
  Matrice<T>& operator-=(const Matrice<T>& M){
    return (*this)-M;
  }

  Matrice<T> operator*(const T& s){
    Matrice<T> result =*this;
    for (auto& pair : *this) {
        auto& key = pair.first;
        auto& value = pair.second;
        result[key] = value*s;
    }
    return result;
  }
  Matrice<T>& operator*=(const T& s){
    return (*this)*s;
  }
  

  Matrice<T> operator/(const T& s){
    Matrice<T> result =*this;
    for (auto& pair : *this) {
        auto& key = pair.first;
        auto& value = pair.second;
        result[key] = value/s;
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
    cout<<"Matrice de taille : "<<(*this).m<<"*"<<(*this).n<<"\n";
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