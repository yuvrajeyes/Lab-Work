% Define the function f(x)
f = @(x) 1 ./ (1 + x.^2);  % Use element-wise power with '.'

% Define the integration interval [a, b]
a = 0;  % Lower limit
b = 1;  % Upper limit

% Values of n (number of subintervals)
n_values = [4, 8, 16];

% Initialize an array to store the results
approximations = zeros(size(n_values));

% Calculate the approximations using Simpson's Rule for each value of n
for i = 1:length(n_values)
    n = n_values(i);
    h = (b - a) / n;  % Width of each subinterval
    x = a:h:b;        % Generate the x values for the subintervals

    % Calculate the Simpson's Rule approximation
    % The formula for Simpson's Rule is: h/3 * [f(a) + 4*f(x1) + 2*f(x2) + 4*f(x3) + ... + 2*f(x_(n-2)) + 4*f(x_(n-1)) + f(b)]
    % where 'a' and 'b' are the limits of integration, 'x1', 'x2', ..., 'x_(n-1)' are
    % the interior points of the subintervals, and 'h' is the width of each subinterval.
    approximations(i) = (h/3) * (f(x(1)) + 4*sum(f(x(2:2:end-1))) + 2*sum(f(x(3:2:end-2))) + f(x(end)));
end

% Display the results
fprintf('Approximations using Simpson''s Rule:\n');
for i = 1:length(n_values)
    fprintf('n = %d: %.6f\n', n_values(i), approximations(i));
end
