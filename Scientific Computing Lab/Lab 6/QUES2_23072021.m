% Define the function f(x) for which we want to find the root
f = @(x) 2 * x^4 - 3 * x^2 + 4 * x - 5;

% Initial interval [a, b]
a = 1;  % Initial value where f(a) is negative
b = 2;  % Initial value where f(b) is positive

% Maximum number of iterations
maxIterations = 100;

% Tolerance for convergence
tolerance = 0.001;

% Call the regulaFalsi function to find the root
root = regulaFalsi(f, a, b, maxIterations, tolerance);

% Display the final root approximation
fprintf('Approximated root: %.4f\n', root);

% Function to find the root using Regula Falsi (False Position) method
function root = regulaFalsi(f, a, b, maxIterations, tolerance)
    if f(a) * f(b) >= 0 || a >= b
        error('Invalid initial interval [a, b]. The function values must have opposite signs, and a < b.');
    end
    
    for i = 1:maxIterations
        % Calculate the next approximation using the Regula Falsi method
        c = (a * f(b) - b * f(a)) / (f(b) - f(a));
        
        % Display the current iteration and the current approximation
        fprintf('Iteration %d: c = %.4f\n', i, c);
        
        % Check for convergence
        if abs(f(c)) < tolerance
            root = c;
            return;
        end
        
        % Update the interval [a, b] based on the signs of f(a) and f(c)
        if f(a) * f(c) < 0
            b = c;
        else
            a = c;
        end
    end
    
    error('Regula Falsi method did not converge within the specified number of iterations.');
end
