% Define the matrix
matrix = [1, 5, 7; 2, 3, 9; 4, 9, 13];

% Calculate the inverse using Gauss-Jordan method
inverse = gaussJordanInverse(matrix);

% Display the original matrix and its inverse
disp("Original Matrix:");
disp(matrix);
disp("Inverse Matrix:");
disp(inverse);

function inverse_matrix = gaussJordanInverse(matrix)
    % Check if the matrix is square
    [n, m] = size(matrix);
    if n ~= m
        disp('The matrix is not square.');
    end
    
    % Check if the matrix is non-singular
    if det(matrix) == 0
        disp('The matrix is singular.');
    end
    

    % Form the augmented matrix with Identity matrix
    augmented_matrix = [matrix, eye(n)];

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
    % Extract the inverse matrix from the augmented matrix
    inverse_matrix = augmented_matrix(:, n+1:end);
end