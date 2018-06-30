%% Image gradient:
% The gradient can be thought as a generalization of the 1st order
% derivative. 
% The gradient of an image I at pixel (i,j) is  ((dI/dx)(i,j) , (dI/dy)(i,j))
% Suppose I has N rows and M colums, i ranges in {1,..,NN} and j in
% {1,...,M}.
% If we see the image as a collection of N  rows 
% f_1(x)
% ...
%f_N(x) then (dI/dx)(i,j) is nothing but f_i'(j), i.e. the classic 
% derivative of a numerical function. Likewise, if g_1,...g_M are
% the columns of I (dI/dy)(i,j)=g_j'(i).
% The image is given as a discrete array. We need a way to compte the
% derivatives. (We shall assume here that is is possible from a
% Mathematical point of view.) There are many methods to do so.
% The simplest one consits in estimating  ((dI/dx)(i,j) , (dI/dy)(i,j)) by
% (dI/dx)(i,j):=I(i+1,j)-I(i,j); and (dI/dy)(i,j):=I(i,j+1)-I(i,j).
% (This is called a "finite difference scheme".) Note that with these kind
% of schemes (dI/dx)(N,j) and (dI/dy)(i,M) is not defined. One usually
% assume "a boundary condition" to enlarge I sufficiently if the values of 
% (dI/dx)(N,j) or (dI/dy)(i,M) are needed. Classic conditions are: 
% entension of I by zeroes, entension by a constant (usually defined so 
% that the gradient is zero at the boundaries, periodic extionsion
% (This larger image needs not to be explicitly stored, of course.)
% Also note that except very
% specific cases the boundary condition has no reason to hold true in the
% sence that if one had a larger camera that produce a larger image
% containing I, then, of course the large image obtained by the large
% camera has no reason to coincide with I extended by some convention.
clear all;
close all;
 options.Interpreter = 'Latex';
I= double(imread('lena.bmp'))/255;
grad_x=I(1:end-1,:)-I(2:end,:);
grad_y=I(:,1:end-1)-I(:,2:end);
% Look at the Matlab doc to convince yourself that is indeed, implements
% the above formula. We chose not to extend I, the gradient is therefore
% smalles (in terms of number of pixels) than image I.
figure;
subplot(2,2,1)
imshow(I);title('Image I');
subplot(2,2,2)
imagesc(grad_x); colormap gray; title('Grad_x')% grad_x has no 
% reason to reason to be positive imagesc is equivalent to applying an 
% afine contrast change so that min(grad_x(:))=0 and max(grad_x(:))=1 and 
% then use imshow on the rescaled (contrast changed) image
% Interpretation: Very bright areas are areas in where the grey levels
% are increasing in the x direction. Dark areas where they are decresing.
% Most of the image is grey, i.e., |Grad_x| is small. This is due to the
% fact that images mostly contains "smooth" variations. The dark/bright
% areas correspond to regions of high variations of grey levels, and are
% related to "edges".
subplot(2,2,3)
imagesc(grad_y); colormap gray; title('Grad_y')%
% The interpretation is the same as Grad_x, except for the direction
% one might be interest in the direction of the vector (grad_x,grad_y)
% or in its magnitude:
subplot(2,2,4)
grad_mag=sqrt(grad_x(:,1:end-1).^2+grad_y(1:end-1,:).^2);
imagesc(grad_y); colormap gray;
title(['\begin{math}','\sqrt{Grad_x^2 + GRAD_y^2}','\end{math}'] , 'Interpreter','latex')
% The magnitude being the l2-norm of the gradient vector, is small only
% when Grad_x and Grad_y are small, i.e. in almost constant regions of the
% image. You can note that the magnitude of the gradient is very small for
% the vast majority of pixels. This simple observation is at the base of
% more addvenced image processing algorithms.

%% Laplacian.

clear all;
close all;
I= double(imread('lena.bmp'))/255;
d_dx=I(1:end-1,:)-I(2:end,:);
d2_dx2=d_dx(1:end-1,:)-d_dx(2:end,:);
d_dy=I(:,1:end-1)-I(:,2:end);
d2_dy2=d_dy(:,1:end-1)-d_dy(:,2:end);
d2_dx2=d2_dx2(:,1:end-2);
d2_dy2=d2_dy2(1:end-2,:);
figure; imshow(I);title('original image');
lap=d2_dx2+d2_dy2;
figure;imagesc(abs(lap));colormap gray;truesize;title('Absolute value of the Laplacian')
% We see some kind of silhouette or the edges are enhanced.
%% Like for the gradient many finite difference schemes are possible.
% Do not forget the boundaries conditions. Also, Laplacian can be
% approximated by difference of Gaussian (DoG) --more on that later on--
% due to its connection with the Heat Equation. This connection also
% permit to design the simplest sharpening algorithm:

%% Image sharpening by Laplacian subtraction.
% It consists in substracting from an image I a portion of its Laplacian
clear all;
close all;
I= double(imread('lena.bmp'))/255;
d_dx=I(1:end-1,:)-I(2:end,:);
d2_dx2=d_dx(1:end-1,:)-d_dx(2:end,:);
d_dy=I(:,1:end-1)-I(:,2:end);
d2_dy2=d_dy(:,1:end-1)-d_dy(:,2:end);
d2_dx2=d2_dx2(:,1:end-2);
d2_dy2=d2_dy2(1:end-2,:);
figure; imshow(I);title('original image');
lap=d2_dx2+d2_dy2; % complete the formula for the laplacian
figure;imagesc(I(1:end-2,1:end-2)-0.2*lap);colormap gray;truesize;title('Sharpened version')
% you can play with the 0.2 parameter above and see how the image evolves.

%% the algorithm also work for color images by appliying the procedure for
% each color : indepentently.
% Also, for better results, one iterate 1) compute laplacian(I) 
% 2) I:=I-epsilon*laplacian(I), 
% go to 1).

clear all;
close all;
I= double(imread('hibiscus.bmp'))/255;
figure; imshow(I);title('original image');
for iter=1:12
    lap = zeros(size(I));
    d_dx=I(1:end-1,:,:)-I(2:end,:,:);
    d2_dx2=d_dx(1:end-1,:,:)-d_dx(2:end,:,:);
    d_dy=I(:,1:end-1,:)-I(:,2:end,:);
    d2_dy2=d_dy(:,1:end-1,:)-d_dy(:,2:end,:);
    d2_dx2=d2_dx2(:,1:end-2,:);
    d2_dy2=d2_dy2(1:end-2,:,:);
    lap=d2_dx2+d2_dy2;
    I = I(1:end-2,1:end-2,:)-0.5*lap;
end;

imshow(I);title(['Sharpened version iteration ' num2str(iter)] );
I= double(imread('hibiscus.bmp'))/255;
figure; imshow(I);title('original image');
