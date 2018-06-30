

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