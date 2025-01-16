% Given
f = @(x, y) x^2 + y;
h = 0.1; % step size 
x0 = 0; % intial value of independent variable
y0 = 1; % intial value of dependent variable
xn = 0.50; % Point where we want to evaluate solution
n = (xn-x0)/h; % calculate step size
x(1) = x0;
y(1) = y0;

fprintf('\n  x           y ');
fprintf('\n %4.4f    %4.4f ',x0,y0);

for i=1:n
    x(i+1) = x0 +i*h; %calculate value of x+h
    temp = x(i)+ h/2;
    k1 = h*f(x(i),y(i)); %calculate value of k1
    k2 = h*f(temp,y(i)+k1/2); % calculate value of k2
    k3 = h*f(temp,y(i)+k2/2); %calculate value of k3
    k4 = h*f(x(i+1),y(i)+k3); % calculate value of k4
    y(i+1)= y(i)+(k1+2*k2+2*k3+k4)/6; % calculate value of y(i+1)

    fprintf('\n %4.4f    %4.4f ',x(i+1),y(i+1));
end
k = length(y);
fprintf('\n The value of y at x=%0.2f is y=%1.3f \n',xn,y(k));