% Benchmark for recursively partitioning meshes, based on various
% bisection approaches
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich



% add necessary paths
addpaths_GP;
nlevels_a = 3;
nlevels_b = 4;

fprintf('       *********************************************\n')
fprintf('       ***  Recursive graph bisection benchmark  ***\n');
fprintf('       *********************************************\n')

% load cases
cases = {
    'airfoil1.mat';
    'netz4504_dual.mat';
    'stufe.mat';
    '3elt.mat';
    'barth4.mat';
    'ukerbe1.mat';
    'crack.mat';
    };

nc = length(cases);
maxlen = 0;
for c = 1:nc
    if length(cases{c}) > maxlen
        maxlen = length(cases{c});
    end
end

for c = 1:nc
    fprintf('.');
    sparse_matrices(c) = load(cases{c});
end


fprintf('\n\n Report Cases         Nodes     Edges\n');
fprintf(repmat('-', 1, 40));
fprintf('\n');
for c = 1:nc
    spacers  = repmat('.', 1, maxlen+3-length(cases{c}));
    [params] = Initialize_case(sparse_matrices(c));
    fprintf('%s %s %10d %10d\n', cases{c}, spacers,params.numberOfVertices,params.numberOfEdges);
end

%% Create results table
fprintf('\n%7s %16s %20s %16s %16s\n','Bisection','Spectral','Metis 5.0.2','Coordinate','Inertial');
fprintf('%10s %10d %6d %10d %6d %10d %6d %10d %6d\n','Partitions',8,16,8,16,8,16,8,16);
fprintf(repmat('-', 1, 100));
fprintf('\n');


for c = 1:nc
    spacers = repmat('.', 1, maxlen+3-length(cases{c}));
    fprintf('%s %s', cases{c}, spacers);
    sparse_matrix = load(cases{c});
    

    % Recursively bisect the loaded graphs in 8 and 16 subgraphs.
    % Steps
    % 1. Initialize the problem
    [params] = Initialize_case(sparse_matrices(c));
    W      = params.Adj;
    coords = params.coords;
    % 2. Recursive routines
    % i. Spectral
    [map_s,~,~] = rec_bisection("bisection_spectral", 3, W, coords, 0);
    [map_s4,~,~] = rec_bisection("bisection_spectral", 4, W, coords, 0);
    
    % ii. Metis
    [map_m, ~, ~] = rec_bisection("bisection_metis", 3, W, coords, 0);
    [map_m4, ~, ~] = rec_bisection("bisection_metis", 4, W, coords, 0);

    % iii. Coordinate
    [map_c, ~, ~] = rec_bisection("bisection_coordinate", 3, W, coords, 0);   
    [map_c4, ~, ~] = rec_bisection("bisection_coordinate", 4, W, coords, 0);    

    % iv. Inertial
    [map_i, ~, ~] = rec_bisection("bisection_inertial", 3, W, coords, 0);
    [map_i4, ~, ~] = rec_bisection("bisection_inertial", 4, W, coords, 0);

    % 3. Calculate number of cut edges
    [cut_spectral] = cutsize(W,map_s);
    [cut_spectral4] = cutsize(W,map_s4);
    [cut_metis] = cutsize(W,map_m);
    [cut_metis4] = cutsize(W,map_m4);
    [cut_coord] = cutsize(W,map_c);
    [cut_coord4] = cutsize(W,map_c4);
    [cut_inertial] = cutsize(W,map_i);
    [cut_inertial4] = cutsize(W,map_i4);
    % 4. Visualize the partitioning result
    if strcmp(cases{c}, 'crack.mat')
        figure();
        gplotmap(W, coords, map_s4);
        title("Spectral");
        
        figure();
        gplotmap(W, coords, map_m4);
        title("Metis");

        figure();
        gplotmap(W, coords, map_c4);
        title("Coordinates");

        figure();
        gplotmap(W, coords, map_i4);
        title("Inertial");
    end
    
    fprintf('%6d %6d %10d %6d %10d %6d %10d %6d\n',cut_spectral,cut_spectral4,cut_metis,cut_metis4,cut_coord,cut_coord4,cut_inertial,cut_inertial4);
    
end
