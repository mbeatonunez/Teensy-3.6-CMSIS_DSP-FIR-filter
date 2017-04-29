%reading the filtered data back into matlab
filt = textread('FILT.TXT','%f');
filt = filt';
fs = 48000; 
spectrogram(filt, [],[],[],fs),title('Teensy Filtered Data');
audiowrite('filtered_birds.wav',filt,fs);
%uncomment this to listen to the filtered data
%sound(2*filt,fs);