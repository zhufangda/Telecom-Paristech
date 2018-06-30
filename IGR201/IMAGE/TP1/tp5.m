clear all;
close all;
sigma=5 %standard deviation of the Gaussian. The bigger the blurrier. Unit: pixel
sigma_2=sigma*sigma; %variance
pi_2=pi*pi; % useful constant
I= double(imread('hibiscus.bmp'))/255;
%I=mean(I,3);
DFT2d_I=fft2(I);
[M , N, nb_color_channels]=size(I);
%%% We use directly the solution without the loop here%%%
 Nr = ifftshift((-fix(M/2):ceil(M/2)-1));
 Nc = ifftshift((-fix(N/2):ceil(N/2)-1));
 [Nc,Nr] = meshgrid(Nc,Nr);
 % The Fourier transform of exp(-ax^2) 
 % is \sqrt(pi/a)\exp(pi^2 \xi^2/a).
 % see e.g. Bracewell, R. The Fourier Transform and Its Applications,
 % p. 98-101, 1999. 
 dft_gauss_kernel=exp(-sigma_2*pi_2*((Nr/M).^2+(Nc/N).^2)/2);
 % In the above Nr/M and Nc/N are the (x,y) frequency they are just placed
 % in order that match the matlab implementation of the DFT values
 % ordering.
 DFT2d_I_convolved=DFT2d_I.*repmat(dft_gauss_kernel,[1,1,nb_color_channels]);
 
figure; plot((-(fix(M/2)):(ceil(M/2)-1))/M,fftshift(abs(dft_gauss_kernel(:,floor(N/2)))));title('Vertical axis of the DFT of the kernel. Low pass filters have their modulus bell shaped like that')
% The x axis is the normalized frequency
figure;imagesc(fftshift(real(ifft2(dft_gauss_kernel))));colormap gray; title('The kernel, (remember the Fourier transform of a Gaussian is a Gaussian');
% We use the shit here, as usual when looing at DFTs. 
% At the center is the (0,0) frequency. The frequencies are attenuated,
% according to a Gaussian coefficient that depends on the squared norm of
% (\xi_1,\xi_2), where x_1 and x_2 represent the x and y frequencies.
I_convolved=ifft2(DFT2d_I_convolved);
I_convolved=real(I_convolved);
% Due to rounding errors I_with_modulus_of_J can have very small
% but non zero imaginary part. We want a real image for displaying.
figure; imshow(I);title('Original image I'); 
figure; imshow(I_convolved); title('I Convolved');