function I_zoomed=padding(I,zoom_factor)

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
end