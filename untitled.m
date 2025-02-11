clc; clear; close all;

% Ouvrir le fichier
fichier = 'maillage.txt';
fid = fopen(fichier, 'r');

% Lire le nombre de points et de triangles
fgetl(fid); % Ignore "Nombre de points:"
nbPoints = fscanf(fid, '%d', 1);
fgetl(fid); % Ignore "Nombre de triangles:"
nbTriangles = fscanf(fid, '%d', 1);
fgetl(fid); % Ignore la ligne vide

% Lire les points
fprintf('Lecture des points...\n');
points = zeros(nbPoints, 2);
for i = 1:nbPoints
    data = fscanf(fid, '%d %f %f', 3);
    points(i, :) = data(2:3); % Récupère seulement (x, y)
end

% Lire les triangles
fprintf('Lecture des triangles...\n');
triangles = zeros(nbTriangles, 3);
ordre = zeros(nbTriangles, 1); % Stocke l'ordre du maillage
milieux = [];

for i = 1:nbTriangles
    ligne = fgetl(fid); % Lire la ligne
    tokens = str2num(ligne); % Convertir en nombres
    triangles(i, :) = tokens(2:4); % Indices des sommets
    ordre(i) = tokens(5); % Ordre du triangle

    % Si ordre P2, stocker les milieux
    if ordre(i) == 2
        milieux = [milieux; tokens(7:9)]; % Stocke les indices des milieux
    end
end

fclose(fid);

% Affichage du maillage
figure; hold on; axis equal;
triplot(triangles + 1, points(:,1), points(:,2), 'k'); % Trace les triangles
plot(points(:,1), points(:,2), 'ro', 'MarkerSize', 3, 'MarkerFaceColor', 'r'); % Points en rouge

% Si P2, afficher les milieux en bleu
if ~isempty(milieux)
    plot(points(milieux,1), points(milieux,2), 'bo', 'MarkerSize', 4, 'MarkerFaceColor', 'b');
end

title('Visualisation du maillage');
legend('Triangles', 'Noeuds', 'Milieux (P2)', 'Location', 'Best');
hold off;
