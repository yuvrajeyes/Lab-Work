% Define the equation for which we want to find the root
f = @(x) x^3 - 2*x^2 - 4*x - 8;

% Define the initial interval [a, b] where we believe the root exists
a = 3;
b = 4;

% Specify the maximum number of iterations
n = 30;

% Define the tolerance to stop the iteration when the interval is small enough
e = 0.0001;

% Check if the signs of f(a) and f(b) are different, indicating a potential root in the interval
if f(a) * f(b) < 0
    % Start the bisection loop
    for i = 1:n
        % Compute the midpoint of the interval
        c = (a + b) / 2;

        % Check if the difference between c and b or c and a is smaller than the tolerance
        if abs(c - b) < e || abs(c - a) < e
            break; % Stop the iteration if the interval is small enough
        end

        % Print the current iteration number and the value of c
        fprintf('Iteration %d = %.4f\n', i, c);

        % Determine which half of the interval has opposite signs
        if f(a) * f(c) < 0
            b = c; % Update the interval [a, b] to [a, c]
        elseif f(b) * f(c) < 0
            a = c; % Update the interval [a, b] to [c, b]
        end
    end
else
    % If f(a) and f(b) have the same sign, no root exists in the given interval
    disp('No root of the equation in the given range');
end
