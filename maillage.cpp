#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Point {
    public:
    double x, y;
    Point(double a=0.0, double b=0.0) {x=a; y=b;}
    void afficher() const {
        cout << "(" << x << ", " << y << ")";
    }

};

class Triangle {
    int ordre; 
    int s1, s2, s3;
    vector<Point> milieux;
    Triangle(int a, int b, int c, int o)
        : s1(a), s2(b), s3(c), ordre(o) {}
    void ajouterMilieux(Point m1, Point m2, Point m3) {
        if (ordre == 2) {
            milieux = {m1, m2, m3};
        }}
    void afficher() const {
        cout << "Triangle (" << sommet1 << ", " << sommet2 << ", " << sommet3 << ")";
        if (ordre == 2) {
            cout << " | Milieux: (" << milieux[0] << ", " << milieux[1] << ", " << milieux[2] << ")";
        }
        cout << endl;
    }
};




class Maillage {
    private:
    vector<Point> points;
    vector<Triangle> triangles;
    int nx, ny; 

public:
    Maillage(int a, int b, int ordre = 1) : nx(a), ny(b) {
        genererMaillage(ordre);
    }

    void genererMaillage(int ordre) {
        double dx = 1.0 / nx;
        double dy = 1.0 / ny;

        // Générer les points du maillage
        for (int j = 0; j <= ny; ++j) {
            for (int i = 0; i <= nx; ++i) {
                points.emplace_back(i * dx, j * dy);
            }
        }

        // Générer les triangles
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                int p1 = j * (nx + 1) + i;
                int p2 = p1 + 1;
                int p3 = p1 + (nx + 1);
                int p4 = p3 + 1;

        // Décomposition en 2 triangles
                Triangle t1(p1, p2, p4, ordre);
                Triangle t2(p1, p4, p3, ordre);


                triangles.push_back(t1);
                triangles.push_back(t2);
            }
        }
    }

    void afficher() const {
        cout << "Maillage : " << endl;
        cout << "Points : " << endl;
        for (size_t i = 0; i < points.size(); ++i) {
            cout << i << ": ";
            points[i].afficher();
            cout << endl;
        }

        cout << "\nTriangles : " << endl;
        for (const auto& tri : triangles) {
            tri.afficher();
        }
    }
};


        