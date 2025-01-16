% Example matrix A and vector b
% A = [3, 1, -2; 1, 7, 3; 3, -2, 10];
% b = [7; -19; 71];

A = input('Enter your coefficient matrix:');
b = input('Enter source vector: ');

% Solve the system of equations using Gaussian elimination with partial pivoting
solution = gaussianElimination(A, b);

disp("Solution:");
disp(solution);

function [x] = gaussianElimination(A, b)
    n = length(b);
    % Augment the coefficient matrix A with the right-hand side vector b
    augmented_matrix = [A, b];
    
    % Perform Gauss Jordan Elimination Method
    for col = 1:1:n
        % Find the pivot row (row with the largest value in the current column)
        pivot_row = col;
        maxVal = abs(augmented_matrix(pivot_row, col));

        % Find larger pivot element in rows below the current row
        for row = col+1:1:n
            if (abs(augmented_matrix(row, col))>maxVal)
                maxVal = abs(augmented_matrix(row, col));
                pivot_row = row;
            end
        end

        % Swap the pivot row with the current row
        augmented_matrix([col, pivot_row], :) = augmented_matrix([pivot_row, col], :);

        % Make the pivot element 1
        pivot = augmented_matrix(col, col);
        augmented_matrix(col, :) = augmented_matrix(col, :)/pivot;

        % Make other element of the current column 0
        for row = 1:1:n
            if row ~= col
                factor = augmented_matrix(row, col);
                augmented_matrix(row,:) = augmented_matrix(row,:) - factor*augmented_matrix(col,:);
            end
        end
    end
    
    % Back substitution
    x = zeros(n, 1);
    for i = n:-1:1
        x(i) = (augmented_matrix(i, n+1) - augmented_matrix(i, i+1:n) * x(i+1:n)) / augmented_matrix(i, i);
    end
end