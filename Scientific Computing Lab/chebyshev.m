% Define the degree of Chebyshev Polynomials
degree = 5;

% Define the interval [-1, 1]
a = -1;
b = 1;

% Calculate Chebyshev nodes
nodes = cos(pi * (2 * (degree:-1:0) + 1) / (2 * (degree + 1)));

% Evaluate Chebyshev Polynomials at the nodes
chebyshev_polynomials = zeros(degree + 1, length(nodes));
for k = 0:degree
    chebyshev_polynomials(k + 1, :) = cos(k * acos(nodes));
end

% Plot Chebyshev Polynomials
figure;
for k = 0:degree
    plot(nodes, chebyshev_polynomials(k + 1, :), 'DisplayName', sprintf('T_%d(x)', k));
    hold on;
end
xlabel('x');
ylabel('T_k(x)');
title('Chebyshev Polynomials of the First Kind');
legend('Location', 'Northwest');
grid on;
hold off;
