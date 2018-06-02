% CANNY - Canny edge detection
%
% Function to perform Canny edge detection. Code uses modifications as
% suggested by Fleck (IEEE PAMI No. 3, Vol. 14. March 1992. pp 337-345)
%
% Usage: [gradient or] = canny(im, sigma)
%
% Arguments:   im       - image to be procesed
%              sigma    - standard deviation of Gaussian smoothing filter
%                      (typically 1)
%		       scaling  - factor to reduce input image by
%		       vert     - weighting for vertical gradients
%		       horz     - weighting for horizontal gradients
%
% Returns:     gradient - edge strength image (gradient amplitude)
%              or       - orientation image (in degrees 0-180, positive
%                         anti-clockwise)
%
% See also:  NONMAXSUP, HYSTHRESH

% Author: 
% Peter Kovesi   
% Department of Computer Science & Software Engineering
% The University of Western Australia
% pk@cs.uwa.edu.au  www.cs.uwa.edu.au/~pk
%
% April 1999    Original version
% January 2003  Error in calculation of d2 corrected
% March 2003	Modified to accept scaling factor and vertical/horizontal
%		        gradient bias (Libor Masek)

% gaussian7_7 = [1.96519161240319e-05     0.000239409349497270	0.00107295826497866	0.00176900911404382	0.00107295826497866	0.000239409349497270	1.96519161240319e-05
%                0.000239409349497270     0.00291660295438644     0.0130713075831894	0.0215509428482683	0.0130713075831894	0.00291660295438644	0.000239409349497270
%                0.00107295826497866      0.0130713075831894      0.0585815363306070	0.0965846250185641	0.0585815363306070	0.0130713075831894	0.00107295826497866
%                0.00176900911404382      0.0215509428482683      0.0965846250185641	0.159241125690702	0.0965846250185641	0.0215509428482683	0.00176900911404382
%                0.00107295826497866      0.0130713075831894      0.0585815363306070	0.0965846250185641	0.0585815363306070	0.0130713075831894	0.00107295826497866
%                0.000239409349497270     0.00291660295438644     0.0130713075831894	0.0215509428482683	0.0130713075831894	0.00291660295438644	0.000239409349497270
%                1.96519161240319e-05     0.000239409349497270	0.00107295826497866	0.00176900911404382	0.00107295826497866	0.000239409349497270	1.96519161240319e-05];
% gaussian13_13 =[4.92039284956766e-06,1.94605312269499e-05,5.99426561741466e-05,0.000143794956950925,0.000268644347022705,0.000390875218503749,0.000442919649189681,0.000390875218503749,0.000268644347022705,0.000143794956950925,5.99426561741466e-05,1.94605312269499e-05,4.92039284956766e-06;
%                 1.94605312269499e-05,7.69678940713786e-05,0.000237077804144399,0.000568720087109968,0.00106250900365370, 0.00154594147827067, 0.00175178119463426, 0.00154594147827067, 0.00106250900365370, 0.000568720087109968,0.000237077804144399,7.69678940713786e-05,1.94605312269499e-05;
%                 5.99426561741466e-05,0.000237077804144399,0.000730251046830066,0.00175178119463426, 0.00327275813518124, 0.00476183498881057, 0.00539586595133019, 0.00476183498881057, 0.00327275813518124, 0.00175178119463426, 0.000730251046830066,0.000237077804144399,5.99426561741466e-05;
%                 0.000143794956950925,0.000568720087109968,0.00175178119463426, 0.00420230462824428, 0.00785093863361610, 0.0114230483086058,  0.0129440095202043,  0.0114230483086058,  0.00785093863361610, 0.00420230462824428, 0.00175178119463426, 0.000568720087109968,0.000143794956950925;
%                 0.000268644347022705,0.00106250900365370, 0.00327275813518124, 0.00785093863361610, 0.0146674843643017,  0.0213410638241057,  0.0241825934590858,  0.0213410638241057,  0.0146674843643017,  0.00785093863361610, 0.00327275813518124, 0.00106250900365370, 0.000268644347022705;
%                 0.000390875218503749,0.00154594147827067, 0.00476183498881057, 0.0114230483086058,  0.0213410638241057,  0.0310510646428929,  0.0351854658661721,  0.0310510646428929,  0.0213410638241057,  0.0114230483086058,  0.00476183498881057, 0.00154594147827067, 0.000390875218503749;
%                 0.000442919649189681,0.00175178119463426, 0.00539586595133019, 0.0129440095202043,  0.0241825934590858,  0.0351854658661721,  0.0398703562166886,  0.0351854658661721,  0.0241825934590858,  0.0129440095202043,  0.00539586595133019, 0.00175178119463426, 0.000442919649189681;
%                 0.000390875218503749,0.00154594147827067, 0.00476183498881057, 0.0114230483086058,  0.0213410638241057,  0.0310510646428929,  0.0351854658661721,  0.0310510646428929,  0.0213410638241057,  0.0114230483086058,  0.00476183498881057, 0.00154594147827067, 0.000390875218503749;
%                 0.000268644347022705,0.00106250900365370, 0.00327275813518124, 0.00785093863361610, 0.0146674843643017,  0.0213410638241057,  0.0241825934590858,  0.0213410638241057,  0.0146674843643017,  0.00785093863361610, 0.00327275813518124, 0.00106250900365370, 0.000268644347022705;
%                 0.000143794956950925,0.000568720087109968,0.00175178119463426, 0.00420230462824428, 0.00785093863361610, 0.0114230483086058,  0.0129440095202043,  0.0114230483086058,  0.00785093863361610, 0.00420230462824428, 0.00175178119463426, 0.000568720087109968,0.000143794956950925;
%                 5.99426561741466e-05,0.000237077804144399,0.000730251046830066,0.00175178119463426, 0.00327275813518124, 0.00476183498881057, 0.00539586595133019, 0.00476183498881057, 0.00327275813518124, 0.00175178119463426, 0.000730251046830066,0.000237077804144399,5.99426561741466e-05;
%                 1.94605312269499e-05,7.69678940713786e-05,0.000237077804144399,0.000568720087109968,0.00106250900365370, 0.00154594147827067, 0.00175178119463426, 0.00154594147827067, 0.00106250900365370, 0.000568720087109968,0.000237077804144399,7.69678940713786e-05,1.94605312269499e-05;
%                 4.92039284956766e-06,1.94605312269499e-05,5.99426561741466e-05,0.000143794956950925,0.000268644347022705,0.000390875218503749,0.000442919649189681,0.000390875218503749,0.000268644347022705,0.000143794956950925,5.99426561741466e-05,1.94605312269499e-05,4.92039284956766e-06;]
function [gradient, or] = canny(im, sigma, scaling, vert, horz)

xscaling = vert;
yscaling = horz;

% hsize = [6*sigma+1, 6*sigma+1];   % The filter size.     
% gaussian = fspecial('gaussian',hsize,sigma);
% im = filter2(gaussian,im);        % Smoothed image.

% gaussian 的来源
%  siz   = (p2-1)/2;   % p2 = [6*sigma+1, 6*sigma+1]
%  std   = p3;      % p3 为1
%  [x,y] = meshgrid(-siz(2):siz(2),-siz(1):siz(1));
%  arg   = -(x.*x + y.*y)/(2*std*std); 
%  h     = exp(arg);
%  h(h<eps*max(h(:))) = 0;   % eps = 2^(-52)
%  sumh = sum(h(:));
%  if sumh ~= 0,
%    h  = h/sumh;
%  end;

if sigma == 1
    gaussian = [1.96519161240319e-05     0.000239409349497270	 0.00107295826497866	0.00176900911404382	0.00107295826497866	0.000239409349497270	1.96519161240319e-05
                0.000239409349497270     0.00291660295438644     0.0130713075831894	    0.0215509428482683	0.0130713075831894	0.00291660295438644     0.000239409349497270
                0.00107295826497866      0.0130713075831894      0.0585815363306070     0.0965846250185641	0.0585815363306070	0.0130713075831894      0.00107295826497866
                0.00176900911404382      0.0215509428482683      0.0965846250185641     0.159241125690702	0.0965846250185641	0.0215509428482683      0.00176900911404382
                0.00107295826497866      0.0130713075831894      0.0585815363306070     0.0965846250185641	0.0585815363306070	0.0130713075831894      0.00107295826497866
                0.000239409349497270     0.00291660295438644     0.0130713075831894     0.0215509428482683	0.0130713075831894	0.00291660295438644     0.000239409349497270
                1.96519161240319e-05     0.000239409349497270    0.00107295826497866	0.00176900911404382	0.00107295826497866	0.000239409349497270	1.96519161240319e-05];
elseif sigma == 2
    gaussian = [4.92039284956766e-06,1.94605312269499e-05,5.99426561741466e-05,0.000143794956950925,0.000268644347022705,0.000390875218503749,0.000442919649189681,0.000390875218503749,0.000268644347022705,0.000143794956950925,5.99426561741466e-05,1.94605312269499e-05,4.92039284956766e-06;
                1.94605312269499e-05,7.69678940713786e-05,0.000237077804144399,0.000568720087109968,0.00106250900365370, 0.00154594147827067, 0.00175178119463426, 0.00154594147827067, 0.00106250900365370, 0.000568720087109968,0.000237077804144399,7.69678940713786e-05,1.94605312269499e-05;
                5.99426561741466e-05,0.000237077804144399,0.000730251046830066,0.00175178119463426, 0.00327275813518124, 0.00476183498881057, 0.00539586595133019, 0.00476183498881057, 0.00327275813518124, 0.00175178119463426, 0.000730251046830066,0.000237077804144399,5.99426561741466e-05;
                0.000143794956950925,0.000568720087109968,0.00175178119463426, 0.00420230462824428, 0.00785093863361610, 0.0114230483086058,  0.0129440095202043,  0.0114230483086058,  0.00785093863361610, 0.00420230462824428, 0.00175178119463426, 0.000568720087109968,0.000143794956950925;
                0.000268644347022705,0.00106250900365370, 0.00327275813518124, 0.00785093863361610, 0.0146674843643017,  0.0213410638241057,  0.0241825934590858,  0.0213410638241057,  0.0146674843643017,  0.00785093863361610, 0.00327275813518124, 0.00106250900365370, 0.000268644347022705;
                0.000390875218503749,0.00154594147827067, 0.00476183498881057, 0.0114230483086058,  0.0213410638241057,  0.0310510646428929,  0.0351854658661721,  0.0310510646428929,  0.0213410638241057,  0.0114230483086058,  0.00476183498881057, 0.00154594147827067, 0.000390875218503749;
                0.000442919649189681,0.00175178119463426, 0.00539586595133019, 0.0129440095202043,  0.0241825934590858,  0.0351854658661721,  0.0398703562166886,  0.0351854658661721,  0.0241825934590858,  0.0129440095202043,  0.00539586595133019, 0.00175178119463426, 0.000442919649189681;
                0.000390875218503749,0.00154594147827067, 0.00476183498881057, 0.0114230483086058,  0.0213410638241057,  0.0310510646428929,  0.0351854658661721,  0.0310510646428929,  0.0213410638241057,  0.0114230483086058,  0.00476183498881057, 0.00154594147827067, 0.000390875218503749;
                0.000268644347022705,0.00106250900365370, 0.00327275813518124, 0.00785093863361610, 0.0146674843643017,  0.0213410638241057,  0.0241825934590858,  0.0213410638241057,  0.0146674843643017,  0.00785093863361610, 0.00327275813518124, 0.00106250900365370, 0.000268644347022705;
                0.000143794956950925,0.000568720087109968,0.00175178119463426, 0.00420230462824428, 0.00785093863361610, 0.0114230483086058,  0.0129440095202043,  0.0114230483086058,  0.00785093863361610, 0.00420230462824428, 0.00175178119463426, 0.000568720087109968,0.000143794956950925;
                5.99426561741466e-05,0.000237077804144399,0.000730251046830066,0.00175178119463426, 0.00327275813518124, 0.00476183498881057, 0.00539586595133019, 0.00476183498881057, 0.00327275813518124, 0.00175178119463426, 0.000730251046830066,0.000237077804144399,5.99426561741466e-05;
                1.94605312269499e-05,7.69678940713786e-05,0.000237077804144399,0.000568720087109968,0.00106250900365370, 0.00154594147827067, 0.00175178119463426, 0.00154594147827067, 0.00106250900365370, 0.000568720087109968,0.000237077804144399,7.69678940713786e-05,1.94605312269499e-05;
                4.92039284956766e-06,1.94605312269499e-05,5.99426561741466e-05,0.000143794956950925,0.000268644347022705,0.000390875218503749,0.000442919649189681,0.000390875218503749,0.000268644347022705,0.000143794956950925,5.99426561741466e-05,1.94605312269499e-05,4.92039284956766e-06;];
end

[row_gaussian, col_gaussian] = size(gaussian);
[row, col] = size(im);
im1 = zeros(row,col);
ceil_row = ceil(row_gaussian/2);     % 与ceil_rol 相等
floor_row = floor(row_gaussian/2);   % 与floor_col相等
ceil_col = ceil(col_gaussian/2);     % 与ceil_rol 相等
floor_col = floor(col_gaussian/2);   % 与floor_col相等
for ii = ceil_row : row - floor_row
    for jj = ceil_col : col - floor_col
        im1(ii,jj) = sum(sum(double(im(ii-floor_row:ii+floor_row, jj-floor_col:jj+floor_col)) .* gaussian));
    end
end
im = im1;

im = imresize(im, scaling);

[rows, cols] = size(im);

h =  [  im(:,2:cols)  zeros(rows,1) ] - [  zeros(rows,1)  im(:,1:cols-1)  ];
v =  [  im(2:rows,:); zeros(1,cols) ] - [  zeros(1,cols); im(1:rows-1,:)  ];
d1 = [  im(2:rows,2:cols) zeros(rows-1,1); zeros(1,cols) ] - ...
                               [ zeros(1,cols); zeros(rows-1,1) im(1:rows-1,1:cols-1)  ];
d2 = [  zeros(1,cols); im(1:rows-1,2:cols) zeros(rows-1,1);  ] - ...
                               [ zeros(rows-1,1) im(2:rows,1:cols-1); zeros(1,cols)   ];

X = ( h + (d1 + d2)/2.0 ) * xscaling;
Y = ( v + (d1 - d2)/2.0 ) * yscaling;

gradient = sqrt(X.*X + Y.*Y); % Gradient amplitude.

or = atan2(-Y, X);            % Angles -pi to + pi.
neg = or<0;                   % Map angles to 0-pi.
or = or.*~neg + (or+pi).*neg; 
or = or*180/pi;               % Convert to degrees.
