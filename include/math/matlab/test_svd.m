
clc;
clear all;

a=[1,2,3;4,5,6;7,8,9];
a=[1,2,0;
3,4,0;
0,0,0
];
a1=[1,2;
3,4;
];
a=[1 0.015277 0.0455724
-0.015277 1 -0.0258394
-0.0455724 0.0258394 1];
[U,S,V]=svd(a);
%[U1,S1,V1]=svd(a1);



