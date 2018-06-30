%% Generate observed image
clear all;
close all;
I=double(imread('flowers.bmp'))/255;
I=mean(I,3);
sizeI=size(I);
u=I+0.1*randn(sizeI);
figure; imshow(u);title('Noisy observed image');

% Denoise
w=3;
sigma_s=1000000;
sigma_i=0;
size_I_noisy=size(u);


%% Bilateral filtering.
% Pre-compute Gaussian distance weights.
% Step1. 
spatial_weights=zeros(2*w+1,2*w+1);
for x_1=1:2*w+1
    for x_2=1:2*w+1
       puissance = -((x_1 - w - 1)^2 + (x_2 - w - 1)^2)/(2*sigma_s);
       spatial_weights(x_1,x_2) = exp(puissance);
    end;
end;
% Step2. 
I_denoised = u;
for p_1 = w+1 : size_I_noisy(1)-w
   for p_2 = w+1 : size_I_noisy(2)-w
       square = u(p_1-w :p_1+w, p_2-w:p_2 + w);
       puissance = -(square - u(p_1,p_2) * ones(2*w+1, 2*w+1)).^2/(2*sigma_i);
       u_denoise = square .* spatial_weights .* exp(puissance);
       C = spatial_weights .* exp(puissance);
       I_denoised(p_1,p_2) = sum( u_denoise(:)) / sum(C(:));
   end;
end;

figure;imshow(I_denoised);title('Denoised image');

