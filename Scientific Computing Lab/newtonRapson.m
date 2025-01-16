% Example usage
number = 29;
epsilon = 1e-6; % Desired accuracy
result = mySqrt(number, epsilon);

disp(['Square root of ', num2str(number), ' is approximately ', num2str(result)]);


function sqrtValue = mySqrt(x, epsilon)
    % Initialize the guess
    guess = x / 2;
    
    % Iterate until the guess is close enough to the actual square root
    while abs(guess * guess - x) > epsilon
        guess = 0.5 * (guess + x / guess); % Newton-Raphson formula
    end
    
    sqrtValue = guess;
end