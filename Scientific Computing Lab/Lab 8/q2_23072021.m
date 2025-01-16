% Given
x0 = 0; xn = 1; h = 0.2; y0 = 1;
f = @(x,y) exp(-x) - 2*y;

% Computation
x = x0:h:xn;
y(1) = y0; % IC: y(x0)=y0 [when n = 0]

fprintf('\n  x           y ');
fprintf('\n %4.4f    %4.4f ',x0,y0);

% RK4 scheme
for n = 1:3
    k1 = h*f(x(n),y(n));
    k2 = h*f(x(n)+h/2,y(n)+k1/2);
    k3 = h*f(x(n)+h/2,y(n)+k2/2);
    k4 = h*f(x(n)+h,y(n)+k3);
    k = (1/6)*(k1 + 2*k2 + 2*k3 + k4);
    y(n+1) = y(n) + k;

    fprintf('\n %4.4f    %4.4f ',x(n+1),y(n+1));
end

% Adams-Bashforth Adams-Moulton Predictor-Corrector
for n = 1:length(x) - 4
    % AB Predictor Formula
    yp(n+4) = y(n+3) + (h/24)*(55*f(x(n+3),y(n+3)) - 59*f(x(n+2),y(n+2)) + 37*f(x(n+1),y(n+1)) - 9*f(x(n),y(n)) );

    % AM Corrector Formula
    y(n+4) = y(n+3) + (h/24)*(9*f(x(n+4), yp(n+4)) + 19*f(x(n+3),y(n+3)) - 5*f(x(n+2), y(n+2)) + f(x(n+1),y(n+1)) );
    
    fprintf('\n %4.4f    %4.4f ',x(n+4),y(n+4));
end

answer=y(n+4);
fprintf('\n The value of y at x=%0.2f is y=%1.3f \n',xn,answer);