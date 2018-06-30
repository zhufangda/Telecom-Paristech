clear all;
close all;

%% After this you need to be able to: read, print images, manupulate
%% understand pixel values and the effects of pixel quantization.

%% read image
I=double(imread('house.png')); 
I=I/255; %(this command will be explained later on)

size(I) % note that there's no ; at the end. We want Matlab to print the
% result
% The output is [nb_row n_columns nb_of_color_chanels (1 for grayscale
% images, here 3 (most common) for color images, can be more "hyperspectral
% images. Indeed, any color in a natural image can be made by mixing
% different proportions of RED, GREEN, BLUE.
%=> I is stored as the concatenation of three matrices. 
% I(x_coord,y_coord,1) contains the RED value at (x_coord_y_coord)
% I(x_coord,y_coord,2) contains the BLUE value at (x_coord_y_coord)
%
% We can access/ visulalize any
% pixel values:

%% display 5x5 pixels on the top left pixels values for every color channel
I(1:5,1:5,1)

%% display the image
imshow(I)

%We can change the RED channel for these pixels:
I(1:5,1:5,1)=1;
figure; %otherwise the new image erase the previous one on the screen
imshow(I)
%% or change another channel
I=double(imread('house.png')); 
I=I/255;
I(1:5,1:5,2)=1;
figure;
imshow(I)


%% convert to grayscale (simplest method)
I=double(imread('house.png')); 
I=I/255;
I=mean(I,3); % perform the mean among color channel to get a grayscale image
figure;
imshow(I)

%% we can generate simple images
I=zeros(512,512); %create a 512x512 matrix, containing zeroes
I(200:250,200:250)=1; %change the pixel values for the rectangle
figure;
imshow(I); 
% => we see that imshow print black where I(x,y) is zero and white where
% I(x,y) is 1. Values in-between are "gray"
I=zeros(512,512); %create a 512x512 matrix, containing zeroes
I(200:250,200:250)=0.7; %try with different values in [0,10]
% => a value bigger than 1 is white.
figure;
imshow(I); 
% Now 8 bits images (most images) have 2^8 gray levels {0,1,2,...,255}
% so the command on e.g. line 8 I=I/255 change the pixels values
% so that it becomes compatible with the imhsow command.

%% Requantification
I=double(imread('house.png')); 
I=I/255;
% each color channel are in {0,1/255,2/255,...,255/255=1} and this set
% therefore has cardinality 256
% We may want to dimish this cardinality to 128 (to e.g. save some space)
% the simplest method is
N=10 %eg.
J=...% use the round command to requantify I so that its values are 0,0.1,0.2,etc
figure;
imshow(J);title('Requantified') %give titles to easily distiniguish 
%different plots.
figure;
imshow(I);title('Original')
%=> in line 72: complete the formula so we can requantize the image to have N
% levels. Try differents values for N. When do you begin to distinguish 
% the re-quantized version from the original one ?
% It is easy to get why quantification is important: 1) it changes the
% "visual quality" of an image. A smaller quantification step allows to 
% distinguish between small light intensity variations. However, storing
% very precise number costs more in terms of "space".
