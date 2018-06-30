%% After this: you need to be able to compute the histogram of an image,
% detect clues of over/under exposition, change the contrast of an image,
% equalize the histogram, give an image the histogram of another,


clear all;
close all;
I=double(imread('pentagon.png')); 
I=I/255; %(this command will be explained later on)
I=mean(I,3);
figure;
imshow(I)
figure;
hist(I(:),256); title('pentagon'); % 256 stands for 256 possible pixel values


J=double(imread('ruins.jpg')); 
J=J/255; %(this command will be explained later on)
J=mean(J,3);
figure;
imshow(J)
figure;
hist(J(:),256); title('ruins'); % 256 stands for 256 pixel values

K=double(imread('castle.jpg')); 
K=K/255; %(this command will be explained later on)
K=mean(K,3);
figure;
imshow(K)
figure;
hist(K(:),256); title('castle'); % 256 stands for 256 pixel values

%just by looking at the histograms we can easily see that
% ruins image was clearly overexposed (saturation of "white").
% (Too long exposure time)


%%
% The simplest way is to adjust the caontrast is the affine contrast change
% sot that the pixel values cover the entire [0,1] interval
K_aff= K - min(K(:));
K_aff= K_aff / max(K(:));
figure;
imshow(K_aff);title('After affine contrast stretch')
figure;
hist(K_aff(:),256); title('castle_after affine constrst stretch');
figure;
imshow(K);title('Original image')
figure;
hist(K(:),256); title('castle-original');

% Here we implicitly constructed an increasing function \Phi(x)=ax+b;
% so that K_aff(i,j)=\Phi(K(i,j)) has minimal pixel value zero
% and maximal pixel value 1.

%% Recall that : When changing pixel values of an image: contrast changes
%% are increasing functions (strictly). A decreasing function
%% You can have an idea that this is a reasonable operation because
%% strictly monotonous functions are invertible. This means that there is
%% as much information before and after the contrast change operation.

% One of the most common contrast function is called "Gamma correction"
% it is nothing but a power function like x=>x^2. All of them satisfies
% 1) are increasing; -so that they are "admissible"- f(0)=0 and f(1)=1
% so that an image with valued in [0,1] also have values in [0,1] after
% the contrast change.
% The "gamma" correction comes from a technological reason. (see wikipedia,
% if you want to know more)
clear all;
close all;
I=double(imread('ceiling.png')); 
I=I/255; %(this command will be explained later on)
I=mean(I,3);

gamma= 0.2;
figure;
plot(linspace(0,1,1000),    linspace(0,1,1000).^gamma); title('Contrast change')
figure; imshow(I);title('original')
i= 1;
for gamma = 0.55:0.01:0.64
    subplot(3,3,i)
    imshow(I.^gamma); title(['Gamma = ' , num2str(gamma)]);
    i = i+1;
end
% Details in darker regions appears (e.g. top left).
% Try different values of gamma

%% another example: some images have both very dark and very bright regions
% we can enhance darker parts with a gamma<1, and brighter parts with
% gamma >1. (This is classic in outdoors sunny scenes with shades. It is
%  impossible to tune the exposure time to get a "correct" image having
% e.g. 10 000 photons in dark and bright areas simultaneously. 
% It has lead to the high dynamic range functionality in nowadays cameras. 
% These images usuallyrequires a more sophisticated treatment than a global
% contrast adjusment...)
clear all;
close all;
I=double(imread('bright_dark.png')); 
I=I/255;
%imshow(I);title('original')
gamma=.6 % >= 0
figure;
plot(linspace(0,1,1000),linspace(0,1,1000).^gamma); title('Contrast change')
figure; imshow(I);title('original')
figure
imshow(I.^gamma); title('Detils are more visible in dark regions');

% and for brighter regions:

figure
imshow(I.^2); title('Details are more visible in bright regions');


%% Contrast inversion:
clear all;
close all;
I=double(imread('images.jpeg')); 
I=I/255; %(this command will be explained later on)
I=mean(I,3);
I=I-min(I(:));
I=I/max(I(:));
I_inv=1-I;
figure;
imshow(I_inv);title('Can you tell who this is?')






% It is usually more difficult than looking the original image:
figure;imshow(I);
% The reason is that our eyes/brain is very well trained to see the same
% object under very different lighting but not in negative.




%% Contrast Equalization v.s. direct specification v.s. Midway algorithm
%% for pixel-wise image comparison. To compare images, one may need to 
%% adjust the contrast so that the pixel values can be easily compared.
%% (Different cameras have different sensitivity to lighting, so that
%% the pixel values can differ.) 
clear all;
close all;
I1 = double(imread('NotreDame2.tif'))/255;
I2= double(imread('NotreDame1.tif'))/255;
J1 = histeq(I1);
imshow(I1);title('image1');
figure; imshow(J1);title('Image1 after histogram equalization');
J2 = histeq(I2);
figure; imshow(I2);title('image2');
figure; imshow(J2);title('Image2 after histogram equalization');
%just to verify
figure; hist(I1(:),256);title('histogram of image1')
figure; hist(I2(:),256);title('histogram of image2')
figure; hist(J1(:),256);title('histogram of image1 after equalization')
figure; hist(J2(:),256);title('histogram of image2 after equalization')
figure;hist(I1(:),256);title('Original histogram of I1');
figure;hist(I2(:),256);title('Original histogram of I2');



%% Contrast specification  
% Anoter possibillity is to change the histogram of image1 so that it
% matches the histogram of image 2. (This is the simplest way
% implementation when both images have 
% We're going to give I1 the histogram of I2 and vice versa
clear all;
close all;
I1 = double(imread('NotreDame2.tif'))/255;
I2= double(imread('NotreDame1.tif'))/255;
[sizeI1x, sizeI1y]=size(I1);
I1vect=I1(:); % We transform the matrix into a vector (just a change of
% representation)
[sizeI2x, sizeI2y]=size(I2);
I2vect=I2(:); % We transform the matrix into a vector (just a change of
% representation)
[I1_sorted, I1_index]=sort(I1vect); % we sort the pixel values, by
% ascending order, I1_index gives the place of the sorted values bofore
% reordeing.
[I2_sorted, I2_index]=sort(I2vect);

%and I2 by Imid(1), etc.
I1vect(I1_index)=I2_sorted;
I2vect(I2_index)=I1_sorted;
I1_spect_onto_I2=reshape(I1vect,sizeI1x, sizeI1y); % rearrange: from a vector to
% a matrix
I2_specified_onto_I1=reshape(I2vect,sizeI2x, sizeI2y);% rearrange: from a vector to
% a matrix
figure;imshow(I1);title('I1 original')
figure;imshow(I1_spect_onto_I2);title('I1 with the histogram of I2');
figure;imshow(I2);title('I2 original')
figure;imshow(I2_specified_onto_I1);title('I2 with the histogram of I1');
figure;hist(I1(:),256);title('Original histogram of I1');
figure;hist(I2(:),256);title('Original histogram of I2');
figure;hist(I1_spect_onto_I2(:),256);title('Histogram of I1 after specification onto I2');
figure;hist(I2_specified_onto_I1(:),256);title('Histogram of I2 after specification onto I1');