% Step 1: Read data from the file
data = importdata('height_data.txt', ',', 1);
roll_numbers = data.data(:, 1);
heights = data.data(:, 2);

% my roll number and height 
my_roll_number = 21;
my_height = heights(my_roll_number);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Step 3: Perform Lagrange interpolation and estimate my own height
index = find(roll_numbers == my_roll_number);
if ~isempty(index)
    % Exclude my own data for interpolation
    roll_number_without_myself = roll_numbers([1:index-1, index+1:end]);
    height_without_myself = heights([1:index-1, index+1:end]);
    
    % Estimate my height using interpolation
    my_height = lagrange_interpolation(roll_number_without_myself, height_without_myself, my_roll_number);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Step 4: Generate a plot
figure;
plot(roll_numbers, heights, 'o', 'MarkerFaceColor', 'b', 'MarkerEdgeColor', 'b');
hold on;

% Plot my estimated height
plot(my_roll_number, my_height, 'ro', 'MarkerFaceColor', 'r', 'MarkerSize', 8);
xlabel('Roll Number');
ylabel('Height (cm)');
title('Height Data and Interpolated Height');
legend('Original Data', 'My Estimated Height', 'Location', 'Best');
grid on;

% Display my estimated height
fprintf('my estimated height (Roll Number %d): %.2f cm\n', my_roll_number, my_height);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Step 2: Define Lagrange interpolation function
function interpolated_height = lagrange_interpolation(x, y, x0)
    n = length(x);
    interpolated_height = 0;

    for i = 1:n
        L = 1;
        for j = 1:n
            if i ~= j
                L = L * (x0 - x(j)) / (x(j) - x(i));
            end
        end
        interpolated_height = interpolated_height + y(i) * L;
    end
end