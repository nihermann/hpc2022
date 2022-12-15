% Script to load .csv lists of adjacency matrices and the corresponding 
% coordinates. 
% The resulting graphs should be visualized and saved in a .csv file.
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich

addpaths_GP;

% Steps
% 1. Load the .csv files
% help csvread
adj = csvread("VN-4031-adj.csv", 1, 0);
coords = csvread("VN-4031-pts.csv", 1, 0);
% pause;
% 2. Construct the adjaceny matrix (NxN). There are multiple ways
%    to do so.
num_nodes = max(adj(:));
adj_matrix = accumarray(adj, 1, [num_nodes, num_nodes]);
if ~issymmetric(adj_matrix)
    adj_matrix = (adj_matrix + transpose(adj_matrix))/2;
end

% if there is a connection we set it to zero to remove duplicates etc.
adj_matrix = adj_matrix > 0;

A = sparse(adj_matrix);


% 3. Visualize the resulting graphs
plot = gplotg(A, coords);

% 4. Save the resulting graphs
save('../Datasets/Countries_Meshes/mat/VN-graph.mat', 'A', 'coords');


% Example of the desired graph format for CH
% load Swiss_graph.mat
% W      = CH_adj;
% coords = CH_coords;   
% whos
% 
% figure;
% gplotg(W,coords);
