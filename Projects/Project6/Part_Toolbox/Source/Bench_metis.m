function [cut_recursive,cut_kway] = Bench_metis(picture)
% Compare recursive bisection and direct k-way partitioning,
% as implemented in the Metis 5.0.2 library.

%  Add necessary paths
addpaths_GP;

% Graphs in question

% Steps
% 1. Initialize the cases
cases = {
    'luxembourg_osm';
    'usroads';
    'GR-graph.mat';
    'CH-graph.mat';
    'VN-graph.mat';
    'NO-graph.mat';
    'RU-graph.mat';
    };
for c=1:length(cases)
    if c == 1 || c == 2
        l = load(cases{c});
        A = l.Problem.A;
        coords = l.Problem.aux.coord;
    else
        l = load(cases{c});
        A = l.A;
        coords = l.coords;
    end
    % 2. Call metismex to
    %     a) Recursively partition the graphs in 16 and 32 subsets.
    disp("");
    [map,edgecut] = metismex('PartGraphRecursive',A,16);
    [map32,edgecut32] = metismex('PartGraphRecursive',A,32);
    %     b) Perform direct k-way partitioning of the graphs in 16 and 32 subsets.
    [mapk,edgecutk] = metismex('PartGraphKway',A,16);
    [mapk32,edgecutk32] = metismex('PartGraphKway',A,32);
    disp(cases{c});
    fprintf('%6d & %6d & %6d & %6d\n', edgecut, edgecutk, edgecut32, edgecutk32);
    % 3. Visualize the results for 32 partitions.
    figure();
    gplotmap(A, coords, map32);
    title("Recursive");

    figure();
    gplotmap(A, coords, mapk32);
    title("K-Way");
    pause;
end



end