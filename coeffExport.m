%%export filter coefficients
% create a 128 order (128 Taps) Bandpass filter that filter between 2kHz and
% 5kHz. This is the frequency band where the bird sound is found.
bpFilt = designfilt('bandpassfir','FilterOrder',128, ...
         'CutoffFrequency1',2000,'CutoffFrequency2',5000, ...
         'SampleRate',48000);
% Test that your filter work by filtering using matlab
% Ex: output = filter(bpFilt, mono); 
%     sound(output,fs);


% export coefficiensts into a text file so you can write it into the Teensy
% code     
coeff = bpFilt.Coefficients;

fid = fopen('coeff.txt', 'w');
fprintf(fid,'%.7f,\n',coeff);
fclose(fid);
