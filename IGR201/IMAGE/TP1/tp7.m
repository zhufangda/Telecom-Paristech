%% Zooming in an image: zero padding algorithm
% This is a very classic way to zoom an image. It is based on the following
% We recall the 1D-case the 2D case follows from the separability propertty
% of the 2D-DFT transform and inverse transform.
% The inverse formula for the DFT of a signal f(1),...,f(N), at pixel i is
% (1) f(i)=\frac 1 N \sum_{k=1}^N \hat f(k) \omega_N^{-(j-1)(k-1)}, where
% \omega_N is the N-th complex root of -1 and \hat f(k) is the k-th DFT
% coefficient of f. In formula (1), we see that f is a (trigonometric)
% polynomial. In addition, we can evaluate (1) for i=1,...,N using an
% inverse fft algorithm.
% Let M>N and change (1) for 
% (2) \frac 1 M \sum_{k=1}^M \hat f(k) \omega_M^{-(j-1)(k-1)}
% we set \hat f(k):=0 for k=N+1,...,M. and keep the others.
% The polynomial in (2) is equal to the polynomial in (1). 
% This is obvious we added zeroes. This is the same argument as
% x->1+x^2 is equal to x->1+x^2 + 0 x^3 + 0 x^4. The important difference
% is that (2) is valid for i=1,...,N,N+1,...M. So by using (2) we can
% evaluate (1) between the pixels. This is precisely zooming in.
% 
% You can now deduce the zero padding algorithm. 
% 1) take an image and compute its 2D-DFT.
% 2) place it in a larger image containing zeros.
% 3) compute the inverse 2D-DFT of the larger image.
% 4) adjust the normalization (multiply by M/N
% The only difficulty is, again, the storing order. 
% For color image do that for each color channel independently.

clear all;
close all;
zoom_factor=4; % need to be >= 1. % Doesn't need to be integer.
I= double(imread('lena.bmp'))/255; % you can try with color image
%I=mean(I,3);
I=I(1:256,1:256,:); % this is just to be sure we directly see something.
% On small screens matlab resizes down the image when its too big.
% Try without. you can also use the imwrite command to save the result and
% open it with an image viewer.
DFT2d_I=fft2(I);
[M , N, nb_color_channels]=size(I); 
 % Create large image that contains zeros.
 DFT2d_I_padded=zeros(floor(M*zoom_factor),floor(N*zoom_factor),nb_color_channels);
 % Replace zeroes by the values of the 2D-DFT of I.
 % 1) Upper left part.
 DFT2d_I_padded(1:floor(M/2),1:floor(N/2),:)=DFT2d_I(1:floor(M/2),1:floor(N/2),:);
 % 2) Upper right part
 %size(DFT2d_I(1:floor(M/2),N:-1:floor(N/2)+1,:))
 %size(DFT2d_I_padded(1:floor(M/2),floor(N*zoom_factor):-1:floor(N/2)+1,:))
 DFT2d_I_padded(1:floor(M/2),end:-1:end-floor(N/2)+1,:)=DFT2d_I(1:floor(M/2),end:-1:floor(N/2)+1,:);
 % 3) Bottom left part
 DFT2d_I_padded(end:-1:end-floor(M/2)+1,1:floor(N/2),:)=DFT2d_I(end:-1:floor(M/2)+1,1:floor(N/2),:);
 % 4) Bottom zight part.
 DFT2d_I_padded(end:-1:end-floor(M/2)+1,end:-1:end-floor(N/2)+1,:)=DFT2d_I(end:-1:floor(M/2)+1,end:-1:floor(N/2)+1,:);
 % Normalize (the \frac 1/N ) in the formula has changed
 DFT2d_I_padded=DFT2d_I_padded*floor(M*zoom_factor)*floor(N*zoom_factor)/(M*N);
I_zoomed=ifft2(DFT2d_I_padded);
I_zoomed=real(I_zoomed);
% Due to rounding errors I_with_modulus_of_J can have very small
% but non zero imaginary part. We want a real image for displaying.
figure; imshow(I);title('Original image I'); 
figure; imshow(I_zoomed); title('I zoomed');
% We can notive oscillations near the boundaries. This is, again, a
% consequence of the periodic boundary conditions.
% They can be more noticeable like on hibiscus. On hibiscus image you'll
% also notice these oscillations near the edges of the petal.


clear all;
close all;
zoom_factor=4; % need to be >= 1. % Doesn't need to be integer.
I= double(imread('hibiscus.bmp'))/255; % you can try with color image
%I=mean(I,3);
I=I(1:256,1:256,:);
[M , N, nb_color_channels]=size(I); 
J(:,:,:)=[I(1:end,1:end,:) I(1:end,end:-1:1,:); I(end:-1:1,1:end,:) I(end:-1:1,end:-1:1,:)];
imshow(J);title('forced periodization')
J=padding(J,zoom_factor); % This is a function that do exactly what is
% above. This is not a programming course but remember that separating the
% code using function is always better for e.g. bugs checking/readability/
% Remove now useless parts
I_zoomed=J(1:floor(M*zoom_factor),1:floor(N*zoom_factor),:);
 figure; imshow(I);title('Original image I'); 
figure; imshow(I_zoomed); title('I zoomed by implicit DCT');
J=padding(I,zoom_factor); % This is a function that do exactly what is
figure; imshow(J); title('I zoomed by DFT');
% Note the reduction of the artifacts, for hibiscus it is particularly
% visible at the boundaries.



%% ZOOMING OUT


% We've seen two algorithms to zoom in images. How to zoom out. 
% The first possibility is to keep say 1 pixel over 2. This kind of method
% is clearly not adapted to the vast majority of situations when resizing
% an image since only integer factors are possible. Let see how it perfoms.
clear all;
close all;
sub_sampling_factor=4; % need to be >= 1. % Doesn't need to be integer.
I= double(imread('lena.bmp'))/255; % you can try with color image
%I=mean(I,3);
I_zoomed=I(1:sub_sampling_factor:end,1:sub_sampling_factor:end,:);
 figure; imshow(I);title('Original image I'); 
figure; imshow(I_zoomed); title('I zoomed by direct decimation');
%% Another example
clear all;
close all;
sub_sampling_factor=4; % need to be >= 1. % Doesn't need to be integer.
I= double(imread('barbara.png'))/255; % you can try with color image
imagesc(I);colormap gray
%I=mean(I,3);
I_zoomed=I(1:sub_sampling_factor:end,1:sub_sampling_factor:end,:);
 figure; imshow(I);title('Original image I'); 
figure; imshow(I_zoomed); title('I zoomed by direct decimation');

%% last one
clear all;
close all;
sub_sampling_factor=4; % need to be >= 1. % Doesn't need to be integer.
I= double(imread('boat512.gif'))/255; % you can try with color image
%I=mean(I,3);


I_zoomed=I(1:sub_sampling_factor:end,1:sub_sampling_factor:end,:);
 figure; imshow(I);title('Original image I'); 
figure; imshow(I_zoomed); title('I zoomed by direct decimation');

% If you look for aliasing in an image you need to look for change of
% orientation in periodic patterns (see the barbara image), 
% lost of connexity of small structures
% (like a wire see the boat image), and aliasing (see lena).
% Aliasing in an image should be distinguished from temporal aliasing in 
% a movie where each individual image can be nice. In a movie temporal
% aliasing can be seen in e.g. wheels turning in the opposite direction (in
% western movies for instance) of the vehicule or can appear to be steady
% while the vehicle is moving. There exists a third kind of aliasing,
% called post aliasing, in this case the computer screen frequency is
% responsible.

%% At this point you should be pretty much convince that this is not a
% correct way to zoom out images. Given the way we zoomed in images (by
% zero padding) we can deduce a Mathematically correct way to zoom out
% images by setting to zero their high frequencies and inverse DFT.
% Yet, visually the result is of poor quality.
% Indeed, our eyes are used to see images with
% decreasing spectum that tends "smoothly" to zero. By cutting DFT we
% introduced a "discontinuity" that yield to ringing artifacts.

%% A correct way do zoom out an image you need first to convolve with a low
% pass filter. The gaussian kernel justifies itself in many ways. 
% See e.g. Witkin, A. P. "Scale-space filtering"
% T. Lindeberg (1994). "Scale-space theory: A basic tool for analysing 
% structures at different scales"
% It satisfies the semi group property (thanks to the
% Gaussian semi group). The standard deviation of the Gaussian can be
% though as distance to the photographed object. And therefore, doubling
% the distance twice should be equal to multiplying the distance by four.
% Choosing the std-dev.
% The amout of blur is (std-dev) (1) 0.8*sqrt(t^2-1) to t-subsample the 
% image, see e.g. Morel, J. M., & Yu, G. (2008). "On the consistency of the 
% sift method." equation (7). For t>>1 (1) is \approx 0.8 t.
clear all;
close all;
sub_sampling_factor=4; % need to be >= 1. % Integer
I= double(imread('flowers.bmp'))/255; % you can try with color image
% We use a gaussian_convolution function, that do exactly what our previous
% code is doing.
I_convolved=... complete the formula, hint use the gaussian_convolution formula
I_subsampled(:,:,:)=I_convolved(1:sub_sampling_factor:end,1:sub_sampling_factor:end,:);
figure; imshow(I);title('Original image I'); 
figure; imshow(I_subsampled); title('I zoomed out after Gaussian convolution');

% It does look much better. We can check that it looks much better
% We can zoom in of the same amount, just to check that
I_subsampled_zoomed=padding(I_subsampled,sub_sampling_factor);
figure;imshow(I_subsampled_zoomed);title('Zoom in of the image after zoom out');

