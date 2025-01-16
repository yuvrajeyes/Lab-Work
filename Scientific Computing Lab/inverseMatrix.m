invMatrix = inverse_matrix([1,2,3;3,2,1;2,2,3]);
disp(invMatrix);

function invMatrix = inverse_matrix(inputMatrix)
    [rows, cols] = size(inputMatrix);
    if rows ~= cols
        error('Input matrix must be square.');
    end

    if det(inputMatrix) == 0
        error('Input matrix is singular and does not have an inverse.');
    end

    invMatrix = inv(inputMatrix);
end