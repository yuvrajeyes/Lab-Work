A = [4, -2, 4; -2, 5, -2; 4, -2, 8];
B = [6; 0; 16];

x = choleskySolver(A, B);

disp("Solution:");
for k = 1:length(x)
    disp("x" + k + " : " + x(k));
end

function x = choleskySolver(A, B)
    n = length(B);
    U = zeros(n, n);
    UT = zeros(n, n);

    % Cholesky decomposition: A = U^T * U
    U(1, 1) = sqrt(A(1, 1));
    UT(1, 1) = U(1, 1);
    disp(U);

    for k = 2:n
        U(1, k) = A(1, k) / U(1, 1);
        UT(k, 1) = A(k, 1) / U(1, 1);
    end
    disp(U);
    disp(UT);

    for i = 2:n
        for j = i:n
            if i == j
                UT(i, j) = sqrt(A(i, j) - UT(j, 1:i-1) * U(1:i-1, j));
                U(j, i) = UT(i, j);
            else
                UT(j, i) = (A(i, j) - (UT(j-1, 1:i-1) * U(1:i-1, j))) / UT(i, i);
                U(i, j) = UT(j, i);
            end
        end
    end

    % Solving U^T * Y = B
    Y = zeros(n, 1);
    Y(1) = B(1) / UT(1, 1);

    for k = 2:n
        Y(k) = (B(k) - (UT(k, 1:k-1) * Y(1:k-1))) / UT(k, k);
    end

    % Solving U * X = Y
    x = zeros(n, 1);
    x(n) = Y(n) / U(n, n);

    for k = n-1:-1:1
        x(k) = (Y(k) - (U(k, k+1:n) * x(k+1:n))) / U(k, k);
    end
end
