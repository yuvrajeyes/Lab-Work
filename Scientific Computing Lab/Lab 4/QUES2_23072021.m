chebyshev_interpolation(4);

function chebyshev_interpolation(N)
    % Performs Chebyshev polynomial interpolation of degree N for the function f(x) = e^x.
    % N: Degree of the interpolating Chebyshev polynomial

    % Step 1: Compute N+1 Chebyshev nodes (x values) in the interval [-1, 1].
    x_cheb = cos((2*(0:N)+1)*pi/(2*(N+1)));
    
    % Step 2: Evaluate the function f(x) = e^x at the Chebyshev nodes to obtain corresponding function values.
    y_cheb = exp(x_cheb);
    
    % Step 3: Calculate coefficients of interpolating Chebyshev polynomial
    coefficients = zeros(N+1, 1);
    for i = 1:N+1
        % Calculate the ith coefficient
        coefficients(i) = sum(y_cheb .* cos((i-1)*acos(x_cheb))) / (N+1);
    end
    
    % Step 4: Evaluate the interpolating polynomial PN(x)
    % Generate a set of x values for evaluating the interpolating polynomial.
    x_interp = linspace(-1, 1, 1000);
    % Initialize the interpolating polynomial PN(x).
    PN = zeros(size(x_interp));
    for i = 1:N+1
        % Update PN(x) by adding terms of the Chebyshev polynomial
        % multiplied by their corresponding coefficients.
        PN = PN + coefficients(i) * cos((i-1)*acos(x_interp));
    end
    
    % Step 5: Plot the original function and the approximated polynomial
    % original function f(x)=e^x in blue and the interpolating polynomial PN(x) in red.
    plot(x_interp, exp(x_interp), 'b', x_interp, PN, 'r');
    xlabel('x');
    ylabel('f(x) and P_N(x)');
    legend('f(x) = e^x', ['P_N(x) (N = ', num2str(N), ')']);
    title('Chebyshev Polynomial Interpolation');
    grid on;
end