  %
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich

function [part1,part2] = bisection_inertial(A,xy,picture)
% bisection_inertial : Inertial partition of a graph.
%
% [p1,p2] = bisection_inertial(A,xy) returns a list of the vertices on one side of a partition
%     obtained by bisection with a line or plane normal to a moment of inertia
%     of the vertices, considered as points in Euclidean space.
%     Input A is the adjacency matrix of the mesh (used only for the picture!);
%     each row of xy is the coordinates of a point in d-space.
%
% bisection_inertial(A,xy,1) also draws a picture.


% disp(' ');
% disp(' HPC Lab at USI:   ');
% disp(' Implement inertial bisection');
% disp(' ');


% Steps
% 1. Calculate the center of mass.
m_xy = mean(xy);


% 2. Construct the matrix M.
%  (Consult the pdf of the assignment for the creation of M) 
d_xx = sum((xy(:, 1)-m_xy(1)) .* (xy(:, 1)-m_xy(1)));
d_yy = sum((xy(:, 2)-m_xy(2)) .* (xy(:, 2)-m_xy(2)));
d_xy = sum((xy(:, 1)-m_xy(1)) .* (xy(:, 2)-m_xy(2)));
M = [d_xx, d_xy; d_xy, d_yy];

% 3. Calculate the smallest eigenvector of M.  
[u, ~] = eigs(M, 1, "smallestabs");

% 4. Find the line L on which the center of mass lies.
normal = [0, 1; -1,0]*u;

% 5. Partition the points around the line L.
%   (you may use the function partition.m)

[part1,part2] = partition(xy, normal);

if picture == 1
    gplotpart(A,xy,part1);
    title('Inertial bisection using the Fiedler Eigenvector');
end


end
