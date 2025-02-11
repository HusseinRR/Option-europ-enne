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
    int sommet1, sommet2, sommet3; // Indices des sommets dans le maillage
    vector<int> milieux;           // Indices des nœuds milieux si P2
    int ordre;                     // 1 pour P1, 2 pour P2

    Triangle(int s1, int s2, int s3, int ordre = 1)
        : sommet1(s1), sommet2(s2), sommet3(s3), ordre(ordre) {}

    void ajouterMilieux(int m1, int m2, int m3) {
        if (ordre == 2) {
            milieux = {m1, m2, m3};
        }
    }

};


class Maillage {
private:
    vector<Point> points;      // Liste des points
    vector<Triangle> triangles; // Liste des triangles
    int nx, ny;               // Nombre de divisions en x et y

public:
    Maillage(double a, double b, double c, double d, int nx_, int ny_, int ordre=1) : nx(nx_), ny(ny_) {
        genererPoints(a, b, c, d);
        genererTriangles(ordre);
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
    void genererTriangles(int ordre) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                int s1 = j * (nx + 1) + i;
                int s2 = s1 + 1;
                int s3 = s1 + (nx + 1);
                int s4 = s3 + 1;

                // Deux triangles par carré
                Triangle t1(s1, s2, s4, ordre);
                Triangle t2(s1, s4, s3, ordre);

                if (ordre == 2) {
                    int m1 = points.size();
                    points.emplace_back((points[s1].x + points[s2].x) / 2, (points[s1].y + points[s2].y) / 2);
                    int m2 = points.size();
                    points.emplace_back((points[s2].x + points[s4].x) / 2, (points[s2].y + points[s4].y) / 2);
                    int m3 = points.size();
                    points.emplace_back((points[s4].x + points[s1].x) / 2, (points[s4].y + points[s1].y) / 2);

                    t1.ajouterMilieux(m1, m2, m3);

                    int m4 = points.size();
                    points.emplace_back((points[s1].x + points[s4].x) / 2, (points[s1].y + points[s4].y) / 2);
                    int m5 = points.size();
                    points.emplace_back((points[s4].x + points[s3].x) / 2, (points[s4].y + points[s3].y) / 2);
                    int m6 = points.size();
                    points.emplace_back((points[s3].x + points[s1].x) / 2, (points[s3].y + points[s1].y) / 2);

                    t2.ajouterMilieux(m4, m5, m6);
            }
            triangles.push_back(t1);
            triangles.push_back(t2);
        }
    } }

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
        if (triangles[i].ordre == 1) {
            fichier << i << " " << triangles[i].sommet1 << " " << triangles[i].sommet2 << " " << triangles[i].sommet3 << " " << triangles[i].ordre << "\n";}
        if (triangles[i].ordre == 2) {
            fichier << i << " " << triangles[i].sommet1 << " " << triangles[i].sommet2 << " " << triangles[i].sommet3 << " " << triangles[i].ordre << " | Milieux: (" << triangles[i].milieux[0] << ", " << triangles[i].milieux[1] << ", " << triangles[i].milieux[2] << ")"<< "\n";
        }}

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
