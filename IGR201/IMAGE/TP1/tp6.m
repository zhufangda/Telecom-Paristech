clear all;
close all
angle=45.98; % in degrees different in (-90,-90). This is enough to treat the 
% general case. Indeed, -90, 90° and 180 rotations are trivials and doesn't
% require to interpolate. Rotations in (-180,-90) or in (90, 180) can be 
% obtained as a rotation in (-90,90) and a 180° rotation
% We recall that the method is based on the following identity
%
%  | cos(a) -sin(a) |   | 1  -tan(a/2) || 1       0 || 1  -tan(a/2) |
%  |                | = |              ||           ||              |
%  | sin(a)  cos(a) |   | 0     1      || sin(a)  1 || 0     1      |
% valid for a \neq +/- \frac{\pi}2
%  each the three matrices are just translation among row/columns of e.g.
%  tan(a/2), 2*tan(a/2), 3*tan(a/2)
%
% The follwing implementation uses loops, anoter version (faster on Matlab)
% avoid loops. The implementation is straightforward.
% We shall use the Fourier/translation formula. We set the rotation center
% to the center of the image.

angle=angle*pi/180; % degrees to radians
I= double(imread('hibiscus.bmp'))/255;
I=mean(I,3);
[M , N]=size(I);
%%%%%%% Remark %%%%%%%%%
% To prevent one border of the image to re-enter the other side (due to the
% periodic boundary conditions of the DFT we padd the image.
%%%%%%%%%%%%%%%%%%%%%%%%
temp=mean(I(:))*ones(2*M,2*N);
temp(floor(M/2):floor(M/2)+M-1,floor(N/2):floor(N/2)+N-1)=I(:,:);
I=temp; clear temp;
[M , N]=size(I);
I_rot=mean(I(:))*ones(M,N);
%%%%%%%% compute constants %%%%%%%%%%%%%%%
tan_angle_2=tan(angle/2);
sin_angle=sin(angle);
Nr = ifftshift((-fix(M/2):ceil(M/2)-1)); % Nyquist row and colums freq.
Nc = ifftshift((-fix(N/2):ceil(N/2)-1));
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Each of the following loops implements
% one of the above translations.
for k=1:M
    I_rot(k,:)=(ifft(fft(I(k,:)).*exp(-1i*2*pi*(k-floor(M/2))*Nc*..../N))); 
end;
% The constant term in the above, -floor(M/2) is the displacement of the
% rotation center.
figure; imshow(I_rot); title('I translated'); 

for k=1:N
    I_rot(:,k)=(ifft(fft(I_rot(:,k)').*exp(+1i*2*pi*((k-floor(N/2))*Nr*..../M))))'; 
end;
figure; imshow(I_rot); title('I translated'); 

for k=1:M
    I_rot(k,:)=real(ifft(fft(I_rot(k,:)).*exp(-1i*2*pi*(k-floor(M/2))*Nc*..../N))); 
end;

figure; imshow(I);title('Original image I'); 
figure; imshow(I_rot); title('I rotated'); 
