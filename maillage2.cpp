#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// Classe représentant un point dans le maillage
class Point {
public:
    double x, y;
    Point(double x_val, double y_val) : x(x_val), y(y_val) {}
};

// Classe représentant un triangle avec 3 indices de sommets
class Triangle {
public:
    int s1, s2, s3; // Indices des sommets
    int ordre;      // Ordre de l'élément (1 ou 2)

    Triangle(int sommet1, int sommet2, int sommet3, int ordre_element)
        : s1(sommet1), s2(sommet2), s3(sommet3), ordre(ordre_element) {}
};

// Classe gérant le maillage
class Maillage {
private:
    vector<Point> points;      // Liste des points
    vector<Triangle> triangles; // Liste des triangles
    int nx, ny;               // Nombre de divisions en x et y

public:
    Maillage(double a, double b, double c, double d, int nx_, int ny_) : nx(nx_), ny(ny_) {
        genererPoints(a, b, c, d);
        genererTriangles();
    }

    // Générer les points du maillage
    void genererPoints(double a, double b, double c, double d) {
        double dx = (b - a) / nx;
        double dy = (d - c) / ny;

        for (int j = 0; j <= ny; ++j) {
            for (int i = 0; i <= nx; ++i) {
                points.emplace_back(a + i * dx, c + j * dy);
            }
        }
    }

    // Générer les triangles du maillage en connectant les points
    void genererTriangles() {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                int s1 = j * (nx + 1) + i;
                int s2 = s1 + 1;
                int s3 = s1 + (nx + 1);
                int s4 = s3 + 1;

                // Deux triangles par carré
                triangles.emplace_back(s1, s2, s3, 1);
                triangles.emplace_back(s2, s3, s4, 1);
            }
        }
    }

    // Écrire le maillage dans un fichier texte
    void sauvegarderMaillage(const string& filename) {
        ofstream fichier(filename);
        if (!fichier) {
            cerr << "Erreur lors de l'ouverture du fichier " << filename << endl;
            return;
        }

        // Écrire le nombre de points et de triangles
        fichier << "Nombre de points: " << points.size() << "\n";
        fichier << "Nombre de triangles: " << triangles.size() << "\n";

        // Écrire les coordonnées des points
        fichier << "\nPoints:\n";
        for (size_t i = 0; i < points.size(); ++i) {
            fichier << i << " " << points[i].x << " " << points[i].y << "\n";
        }

        // Écrire la liste des triangles
        fichier << "\nTriangles:\n";
        for (size_t i = 0; i < triangles.size(); ++i) {
            fichier << i << " " << triangles[i].s1 << " " << triangles[i].s2 << " " << triangles[i].s3 << " " << triangles[i].ordre << "\n";
        }

        fichier.close();
        cout << "Maillage sauvegardé dans " << filename << endl;
    }
};

int main() {
    // Paramètres du maillage
    double a = 0, b = 1, c = 0, d = 1; // Domaine [0,1] x [0,1]
    int nx = 4, ny = 4; // Nombre de subdivisions

    // Génération du maillage
    Maillage maillage(a, b, c, d, nx, ny);
    maillage.sauvegarderMaillage("maillage.txt");

    return 0;
}
