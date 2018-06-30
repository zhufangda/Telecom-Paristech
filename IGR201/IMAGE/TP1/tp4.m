I= double(imread('house.png'))/255;
I=mean(I,3); %grayscale conversion
DFT2d_I=fft2(I); % compute DTF
[M , N]=size(I); % image size to get frequency grid.


%%% WITH LOOPs %%%%
% X translation
for k=1:floor(N/2)
  for l=1:M
      phase(l,k)=exp(-2*1i*pi*(...*(k-1)/N));
  end;
end;
for k=floor(N/2)+1:N
  for l=1:M
      phase(l,k)=exp(-2*1i*pi*(...*(k-1-N)/N));
  end;
end;
DFT2d_I_translated1=DFT2d_I.*phase;

% here DFT2d_I_translated1 contains the 2D-DFT of the image translated
% in x, we do the same for y
for k=1:N
  for l=1:floor(M/2)
      phase(l,k)=exp(-2*1i*pi*(...*(l-1)/M));
  end;
end;
for k=1:N
  for l=floor(M/2)+1:M
      phase(l,k)=exp(-2*1i*pi*(..*(l-1-M)/M));
  end;
end;
DFT2d_I_translated=DFT2d_I_translated1.*phase;
% more or less done here.
I_translated=ifft2(DFT2d_I_translated);
figure; imshow(I);title('Original image I'); 
figure; imshow(I_translated); title('I translated');
