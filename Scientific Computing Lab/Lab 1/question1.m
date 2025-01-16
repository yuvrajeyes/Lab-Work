% matrixMultiplication: Perform matrix multiplication of two square matrices.
% Inputs:
%   A - First matrix (n x n)
%   B - Second matrix (n x n)
% Outputs:
%   C - Resultant matrix (n x n)

% Define the size of the matrices
n = 3;

% Generate random matrices A and B with values between 1 and 10
A = randi(10, n, n);
B = randi(10, n, n);

C = matrixMultiplication(A, B);  % Perform matrix multiplication
disp('Matrix C:');
disp(C);

function C = matrixMultiplication(A, B)
    % Get the size of matrices A and B
    [row1, col1] = size(A);
    [row2, col2] = size(B);

    % Check if the matrices can be multiplied
    if col1 ~= row2
        error('Matrices cannot be multiplied due to size mismatch.');
    end

    % Initialize matrix C with zeros
    C = zeros(row1, col2);

    % Perform matrix multiplication using nested loops
    for i = 1:row1 % Iterate over rows of A and C
        for j = 1:col2 % Iterate over columns of B and C
            for k = 1:col1 % Iterate over columns of A and rows of B
                % Calculate the element C(i, j) by summing the products 
                % of corresponding elements from A and B
                C(i, j) = C(i, j) + A(i, k) * B(k, j);
            end
        end
    end
end
