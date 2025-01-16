% 2x+2y+4z=18
% x+3y+2z =13
% 3x+y+3z =14   Linear system eq

%augmented matrix: 
a = [2 2 4 18
    1 3 2 13
    3 1 3 14];

%Gaussian Elimination Method [m,n)=size(a);
[m,n]=size(a);
for j=1:m-1
    for z=2:m
        if a(j,j)==0
            t=a(j,:);
            a(j,:)=a(z,:);
            a(z,:)=t;
        end
    end
    for i=j+1:m
        a(i,:)=a(i,:)-a(j,:)*(a(i,j)/a(j,j));
    end
end

x=zeros(1,m);
for s=m:-1:1
    c=0;
    for k=2:m
        c=c+a(s,k)*x(k);
    end
    x(s)=(a(s,n)-c)/a(s,s);
end
disp('Final matrix after performing row reduction operations :');
disp(a);
disp('Val: x    y    z : ')
disp(x);
