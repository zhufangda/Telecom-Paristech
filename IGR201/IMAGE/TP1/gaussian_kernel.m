function dft_gauss_kernel=gaussian_kernel(I,sigma)
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
 dft_gauss_kernel=repmat(exp(-sigma_2*pi_2*((Nr/M).^2+(Nc/N).^2)/2),[1,1,nb_color_channels]);
end