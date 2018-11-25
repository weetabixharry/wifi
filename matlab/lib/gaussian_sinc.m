function [h, g] = gaussian_sinc(Ntaps, centre, bw, alpha)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
% function [h, g] = gaussian_sinc(Ntaps, centre, bw, alpha)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
% Note: "centre" denotes 1-indexed centre tap. Can be non-integer (polyphase).
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 

assert(centre>0 && centre<=Ntaps);
assert(bw>=0 && bw <=1);

% Make the gaussian window
t = (1:Ntaps).' - centre; % Note: indexed from 1
x = t * 2 / Ntaps;
g = exp(-alpha*x.^2);

% Make sinc and apply window
h = bw*sinc(bw*t) .* g;
