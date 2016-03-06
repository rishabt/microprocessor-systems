clear;
load microp.mat

uv = ones(500,1);                   % unit vector
zv = zeros(500,1);                  % zero vector
w1 = cat(2, x_zd/1000,y_zd/1000,z_zd/1000,uv);
w2 = cat(2, x_zu/1000,y_zu/1000,z_zu/1000,uv);
w3 = cat(2, x_yd/1000,y_yd/1000,z_yd/1000,uv);
w4 = cat(2, x_yu/1000,y_yu/1000,z_yu/1000,uv);
w5 = cat(2, x_xd/1000,y_xd/1000,z_xd/1000,uv);
w6 = cat(2, x_xu/1000,y_xu/1000,z_xu/1000,uv);

W = cat(1,w1,w2,w3,w4,w5,w6);

y1 = cat(2,zv,zv,uv);
y2 = cat(2,zv,zv,-1*uv);
y3 = cat(2,zv,uv,zv);
y4 = cat(2,zv,-1*uv,zv);
y5 = cat(2,uv,zv,zv);
y6 = cat(2,-1*uv,zv,zv);

Y = cat(1,y1,y2,y3,y4,y5,y6);


X = inv(transpose(W)*W)*transpose(W)*Y;



