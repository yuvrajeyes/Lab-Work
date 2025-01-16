% Given data points
x = [1, 2, 3, 4, 5];
y = [2.3, 4.1, 5.0, 7.2, 8.6];

% Calculate the sums needed for the equations
% y = ax^2 + bx + c
% xy = ax^3 + bx^2 + cx
% x^2y = ax^4 + bx^3 + cx^2

sum_x = sum(x);
sum_x2 = sum(x.^2);
sum_x3 = sum(x.^3);
sum_x4 = sum(x.^4);
sum_y = sum(y);
sum_xy = sum(x .* y);

% Calculate the coefficients a, b, and c
a = (sum_y * sum_x2 - sum_x * sum_xy) / (5 * sum_x4 - sum_x2^2);
b = (5 * sum_xy - sum_x * sum_y) / (5 * sum_x2 - sum_x^2);
c = (sum_y - b * sum_x - a * sum_x2) / 5;

% Display the results
fprintf('Optimal values of a: %f\n', a);
fprintf('Optimal values of b: %f\n', b);
fprintf('Optimal values of c: %f\n', c);

% Create a curve using the calculated parameters
curve_x = -200:0.1:200; % Generate x values for the curve
curve_y = a * curve_x.^2 + b * curve_x + c; % Calculate y values for the curve

% Plot the data points and the curve
figure;
scatter(x, y, 'o', 'filled', 'DisplayName', 'Data Points');
hold on;
plot(curve_x, curve_y, 'r', 'DisplayName', 'Curve: y = ax^2 + bx + c');
xlabel('x');
ylabel('y');
title('Parabolic Curve Fitting');
legend('Location', 'Northwest');
grid on;
hold off;