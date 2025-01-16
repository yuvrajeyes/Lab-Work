% Define the function f(x)
f = @(x) exp(-x.^2);  % Use element-wise power with '.'

% Define the integration interval [a, b]
a = 0;  % Lower limit
b = 1;  % Upper limit

% Values of n (number of subintervals)
n_values = [4, 8, 16];

% Initialize an array to store the results
approximations = zeros(size(n_values));

% Calculate the approximations using the Trapezoidal Rule for each value of n
for i = 1:length(n_values)
    n = n_values(i);
    h = (b - a) / n;  % Width of each subinterval
    x = a:h:b;        % Generate the x values for the subintervals

    % Calculate the Trapezoidal Rule approximation
    % It uses the formula: h/2 * [f(a) + 2*f(x1) + 2*f(x2) + ... + 2*f(x_(n-1)) + f(b)]
    % where 'a' and 'b' are the limits of integration and 'x1', 'x2', ..., 'x_(n-1)' are
    % the points at which we evaluate the function 'f'.
    approximations(i) = (h / 2) * (f(x(1)) + 2 * sum(f(x(2:end-1))) + f(x(end)));
end

% Display the results
fprintf('Approximations using the Trapezoidal Rule:\n');
for i = 1:length(n_values)
    fprintf('n = %d: %.6f\n', n_values(i), approximations(i));
end