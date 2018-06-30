function I_convolved=gaussian_convolution(I,sigma)
sigma_2=sigma*sigma;
pi_2=pi*pi;
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
 
I_convolved=ifft2(DFT2d_I_convolved);
I_convolved=real(I_convolved);
% Due to rounding errors I_with_modulus_of_J can have very small
% but non zero imaginary part. We want a real image for displaying.
end