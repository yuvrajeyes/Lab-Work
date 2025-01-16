A = [3, 1, -2, 1; 1, 4, 1, -1; 2, -1, 5, 2; 1, -1, 1, 3];
b = [6, 2, 12, 4];

% Initial guess and convergence parameters
x0 = [0; 0; 0; 0];
maxIterations = 4;
tolerance = 1e-3;

[x, iterations] = gaussSeidelSolver(A, b, x0, maxIterations, tolerance);
disp('Final Solution:');
disp(x);
disp('Number of iterations:');
disp(iterations);

% Gauss-Seidel Iterative Method for Solving Linear Systems
function [x, iterations] = gaussSeidelSolver(A, b, x0, maxIterations, tolerance)
    n = length(b);
    x = x0;
    for iteration = 1:maxIterations
        x_prev = x;
        % Gauss-Seidel iteration
        for i = 1:n
            sigma = 0;
            for j = 1:n 
                if j ~= i
                    sigma = sigma + A(i, j) * x(j);
                end
            end
            x(i) = (b(i) - sigma) / A(i, i);
        end
        % Check for convergence
        if norm(x - x_prev, inf) < tolerance
            fprintf('Converged after %d iterations.\n', iteration);
            iterations = iteration;
            return;
        end
        fprintf('Result after %d iteration:\n', iteration);
        disp(x);
    end
    iterations = maxIterations; % Indicate that the maximum iterations were reached
    fprintf('Convergence not achieved after %d iterations.\n', maxIterations);
end