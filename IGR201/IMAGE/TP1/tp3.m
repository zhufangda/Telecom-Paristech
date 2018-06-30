%%  After this you need to be able to compute DFT, look at the magnitude of
% the DFT, to understand what kind of information is in the modulus and
% argument of the DFT, to perform a sub-pixelian translation of an image,
% to perform linear filtering (by DFT-based convolutions/deconvolutions).

clear all;
close all;
I= double(imread('hibiscus.bmp'))/255;
I=mean(I,3);
% To compute the DFT we just do
DFT2d_I=fft2(I);
% There're two main conventions to store the DCT valuees.
% The following command reorder the coefficient for "human" visualization. 
DFT2d_I=fftshift(DFT2d_I);
% We wish to look at the DFT (it's modulus). However, the coeffficients 
% have a very high dynamic: for an image the min is nearly zero, the max
% can be 10^6. If we just do
figure;imagesc(abs(DFT2d_I));colormap gray; truesize;
% We see almost nothing (just a white dot at the center of the image). 
% As usual, in this case we use the Log function
figure;imagesc(log(abs(DFT2d_I)+1));colormap gray; truesize;
title('Modulus of the 2D-DFT');
% we add one so that if abs(DFT2d_I)(i,j)=0 then log(abs(DFT2d_I(i,j))+1)=0
% (this pixel (i,j) is black).
% 1) we notice the symetry in the image. This is due to the Hermitian
% property of the DFT. We also see that most of the bright part of the
% image is concentrated around the center. This is usual for natural images
% their spectrum (the name for the modulus of a DFT) decay rapidly. 
% You can think that this is due to the acquisition model ("band-
% limitedness")/ Shannon-Whittaker sampling theorem). 
% This fast decay implies a certain degree of smoothness of the image (gray
% levels vary relatively slowly.) This is the decay/number of derivative
% Fourier formula. [CITE]
% This also implies that a sharper image decay more slowly than a blurry
% image.
% DFT2d_I is complex valued. We looked at the modulus, let now look at the
% argument. 
figure;imagesc(angle(DFT2d_I));colormap gray; truesize;
title('Argument of the 2D-DFT');
% The argument moree difficult to understand, it looks almost like noise.
% Yet, it contains very important information. Before, we do two classic 
% experiments, lets just check that the DFT transform is invertible.
% This means that observing the DFT is equivalent to observing the image
% itself:
% 1) We invert the reordering we used for visualization.
DFT2d_I=ifftshift(DFT2d_I);
I_from_its_DFT=ifft2(DFT2d_I); % the command for inverse transformation
% let's check that it is indeed the same as the original image I
figure: imshow(I);title('Original image');
figure; imshow(I_from_its_DFT); title('Image calculated from the 2d-DFT');

%% The fft stands for fast Fourier transform is an algorithm to compute DFT
% Cooley, J. W. and J. W. Tukey, "An Algorithm for the Machine 
% Computation of the Complex Fourier Series,"Mathematics of Computation, 
% Vol. 19, April 1965, pp. 297-301. 


%% Modulus/phase exchange experiment: 1) We going to change the modulus of 
% 2D-DFT of an image for the modulus of the 2D-DFT of another image and see
% what happens.
% NB: this is the simplest case where the two images have same sizes.
% You'll be able to generalize this experiment once you'll know how to zoom
% in/out images.
clear all;
close all;
I= double(imread('hibiscus.bmp'))/255;
I=mean(I,3);
J= double(imread('flowers.bmp'))/255;
J=mean(J,3);
DFT2d_I=fft2(I);
DFT2d_J=fft2(J);
% Note that there's no need to reorder, since we don't want to display the
% 2D-DFTs
% Now crease a new image by specifyiong the 2d-DFT values. We keep the
% angle in DFT2d_I and replace the modulus in DFT2d_I by the modulus in 
% DFT2d_J=fft2(J);
modulus_DFT2d_J=abs(fft2(J));
angle_DFT2d_I=angle(DFT2d_I);
[real_part, imaginary_part]=pol2cart(angle_DFT2d_I,modulus_DFT2d_J);
% The above function perform the change of coordinate polar to cartesian
% We now create the 2D-DFT of the new image:
DFT2d_I_with_modulus_of_J=real_part+i*imaginary_part;
% now, inverse transform and display
I_with_modulus_of_J=ifft2(DFT2d_I_with_modulus_of_J);
I_with_modulus_of_J=real(I_with_modulus_of_J);
% Due to rounding errors I_with_modulus_of_J can have very small
% but non zero imaginary part. We want a real image for displaying.
figure; imagesc(I);colormap gray; title('Original image I'); truesize;
figure; imagesc(J);colormap gray; title('Original image J'); truesize;
figure; imagesc(I_with_modulus_of_J);colormap gray; title('Image with the arguement of I and the modulus of J'); truesize;
clear all;
close all;
I= double(imread('hibiscus.bmp'))/255;
I=mean(I,3);
J= double(imread('flowers.bmp'))/255;
J=mean(J,3);
DFT2d_I=fft2(I);
DFT2d_J=fft2(J);
angle_DFT2d_J=angle(DFT2d_J);
modulus_DFT2d_I=abs(DFT2d_I);
[real_part, imaginary_part]=pol2cart(angle_DFT2d_J,modulus_DFT2d_I);
% The above function perform the change of coordinate polar to cartesian
% We now create the 2D-DFT of the new image:
DFT2d_I_with_phase_of_J=real_part+i*imaginary_part;
% now, inverse transform and display
I_with_phase_of_J=ifft2(DFT2d_I_with_phase_of_J);
I_with_phase_of_J=real(I_with_phase_of_J);
% Due to rounding errors I_with_modulus_of_J can have very small
% but non zero imaginary part. We want a real image for displaying.
figure; imagesc(I);colormap gray; title('Original image I'); truesize;
figure; imagesc(J);colormap gray; title('Original image J'); truesize;
figure; imagesc(I_with_phase_of_J);colormap gray; title('Image with the arguement of J and the modulus of I'); truesize;
% This image looks closer to J than to I.
% So, to recognize an object what is more important? phase or modulus?
