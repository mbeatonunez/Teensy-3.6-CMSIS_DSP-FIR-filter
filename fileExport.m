%code for FIR filter. reads audio file, converts it into mono and exports it
%into a file to later be read by the teensy.
[Sound,fs] = audioread('background_noise_city.wav');
%we go from stereo to mono so that we deal with a one dimensional array
mono = (Sound(:,1)+Sound(:,2))/2;
mono = mono';
N = length(mono); % number of samples
fid = fopen('input.txt', 'w'); 
fwrite(fid,mono,'float');      % outputs data as binary rather than ASCII
fclose(fid);
