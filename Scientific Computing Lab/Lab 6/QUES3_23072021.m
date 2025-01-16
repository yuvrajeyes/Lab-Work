% Define the function f(x) for which we want to find the root.
f = @(x) e^x-2*x^2;
% Define the derivative of the function f(x).
df = @(x) e^x-4*x;
% Initial guess for the root.
x0 = 1;
% Set the desired tolerance for the root approximation.
tolerance = 1e-6;
% Set a maximum number of iterations to avoid infinite loops.
maxIterations = 100;
% Initialize variables
x = x0;
iteration = 0;

% Perform the Newton-Raphson iterations
while abs(f(x)) > tolerance && iteration < maxIterations
    % Calculate the next approximation using the Newton-Raphson formula
    x = x - f(x) / df(x);
    % Increment the iteration counter
    iteration = iteration + 1;
    % Display the current iteration and the current approximation
    fprintf('Iteration %d: x = %f\n', iteration, x);
end
% Check if the method converged within the maximum number of iterations
if iteration >= maxIterations
    fprintf('The method did not converge within %d iterations.\n', maxIterations);
else
    % Display the final root approximation
    fprintf('Approximated root: x = %f\n', x);
end

