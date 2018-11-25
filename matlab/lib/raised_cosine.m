function h = raised_cosine(tap_count, Ts, beta)
% . . . . . . . . . . . . . . . . . . . . . . . .
% function h = raised_cosine(tap_count, Ts, beta)
% . . . . . . . . . . . . . . . . . . . . . . . .
% Ts = oversamp_ratio
% . . . . . . . . . . . . . . . . . . . . . . . .
% Note: Normalisation is such that symbol points
% remain unchanged. To instead achieve unity DC
% gain, use h/sum(h).
% . . . . . . . . . . . . . . . . . . . . . . . .

h = zeros(tap_count, 1);
for idx = 1:tap_count
    % Make sure t=0 always occurs, even if Ntaps is even.
    % For Ntaps odd:  t = -(N-1)/2 : (N-1)/2 [symmetric]
    % For Ntaps even: t = -N/2 : N/2-1		 [assymetric]
    t = -floor(tap_count/2) + (idx-1);
	
    h(idx) = sinc(t/Ts);
    % Fix 0 / 0 using L'Hopital's rule
    if 2.0 * beta * abs(t) == Ts
        h(idx) = h(idx) * pi / 4.0;
    else
        h(idx) = h(idx) * cos(pi * beta * t / Ts);
        h(idx) = h(idx) / (1 - (2*beta*t / Ts)^2 );
    end
end


% % Frequency domain
% for i = 0:Ntaps-1
	% f = -1/2 + i/(Ntaps-1);
	% if abs(f) <= (1-beta) / (2*Ts)
		% H(i+1) = Ts;
	% elseif (1-beta)/(2*Ts) < abs(f) && abs(f) <= (1+beta)/(2*Ts)
		% H(i+1) = Ts*(1 + cos(pi*Ts(abs(f) - (1-beta)/(2*Ts))/beta))/2;
	% else
		% H(i+1) = 0;
	% end
% end
