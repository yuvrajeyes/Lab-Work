% Call the function and display the roots
disp('Roots:');
disp(find(1, 2, 2));

% Define a function 'find' that takes coefficients a, b, and c of 
% equation ax^2 + bx + c as inputs
function roots = find(a, b, c)
    % Calculate the discriminant of the quadratic equation
    discriminant = b^2 - 4*a*c;
    
    % Check if discriminant is positive (real and different roots)
    if discriminant > 0
        % Calculate the two real roots
        root1 = (-b + sqrt(discriminant)) / (2*a);
        root2 = (-b - sqrt(discriminant)) / (2*a);
        % Store roots in an array
        roots = [root1, root2]; 

    % Check if discriminant is zero (real and equal roots)
    elseif discriminant == 0
        % Calculate the single real root
        root1 = -b / (2*a); 
        % Store the repeated root in an array
        roots = [root1, root1]; 

    % If discriminant is negative (complex roots)
    else
        % Calculate the real and imaginary parts of the complex roots
        realPart = -b / (2*a);
        imaginaryPart = sqrt(abs(discriminant)) / (2*a);
        % Store complex roots in an array
        roots = [realPart + 1i*imaginaryPart, realPart - 1i*imaginaryPart];
    end
end
