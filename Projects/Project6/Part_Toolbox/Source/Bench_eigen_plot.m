% Visualize information from the eigenspectrum of the graph Laplacian
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich

% add necessary paths
addpaths_GP;

% Graphical output at bisection level
picture = 0;

% Cases under consideration
load airfoil1.mat;
% load 3elt.mat;
% load barth4.mat;
% load mesh3e1.mat;
% load crack.mat;

% Initialize the cases
W      = Problem.A;
coords = Problem.aux.coord;

% Steps
% 1. Construct the graph Laplacian of the graph in question.
sums = sum(W);
L = -W;
for i = 1:length(W)
    L(i, i) = L(i, i) + sums(i);
end

% 2. Compute eigenvectors associated with the smallest eigenvalues.
[Vecs, ~] = eigs(L, 3, "smallestabs");

% 3. Perform spectral bisection.
[part1, part2] = other(Vecs(:, 2) < median(Vecs(:, 2)));

% 4. Visualize:
%   i.   The first and second eigenvectors.
figure();
plot(Vecs(:, 1:2), ".");
title("Smallest Two Eigenvectors");


%   ii.  The second eigenvector projected on the coordinate system space of graphs.
figure();
coords(:, 3) = Vecs(:,2);
scatter3(coords(:, 1), coords(:, 2), coords(:, 3), 20, coords(:, 3), "filled");
hold on
gplot(W(part1, part1), coords(part1, :), "-k");
gplot(W(part2, part2), coords(part2, :), "-r");
colorbar
hold off


%   iii. The spectral bi-partitioning results using the spectral coordinates of each graph.
figure();
gplotpart(W, Vecs(:,2:3), part1);
ylabel("v_3");
xlabel("v_2");
title("Spectral bi-partitioning using spectral coordinates");
