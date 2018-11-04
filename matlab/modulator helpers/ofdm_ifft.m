function time_sym = ofdm_ifft(ofdm_sym)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% function time_sym = ofdm_ifft(ofdm_sym)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% Converts 64-point OFDM symbol to 81-point time-domain representation,
% where the edge samples are weighted to form an overlap.
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

% FFT shift to put DC where it belongs and do OFDM IFFT
time_sym = ifft(fftshift(ofdm_sym));
% Extend periodically up to 81 samples...
time_sym = repmat(time_sym, ceil(81/64), 1);
% Align such that 64-point IFFT ends on penultimate sample
time_sym = circshift(time_sym, 80-64);
% Discard excess
time_sym = time_sym(1:81);
% Apply window function
time_sym(1) = 0.5 * time_sym(1);
time_sym(81) = 0.5 * time_sym(81);