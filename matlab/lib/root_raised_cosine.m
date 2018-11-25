function h = root_raised_cosine(tap_count, Ts, beta)
% . . . . . . . . . . . . . . . . . . . . . . . . . . .
% function h = root_raised_cosine(tap_count, Ts, beta)
% . . . . . . . . . . . . . . . . . . . . . . . . . . .
% Ts = oversamp_ratio
% . . . . . . . . . . . . . . . . . . . . . . . . . . .
% Note: Normalisation is slightly less clear than in
% the raised cosine case. Here, normalisation is such
% that symbol points would be unchanged if we were to
% apply hh = resample(h,1,1,h). However, in practice
% each half is applied separately, so resulting
% amplitudes are more difficult to predict. As an
% alternative, we could use h/sum(h) for 0dB DC gain.
% . . . . . . . . . . . . . . . . . . . . . . . . . . .
% Easy reference: Wikipedia article.
% Reference: "Theory of Root-Raised Cosine Filter",
%            Michael Joost, December 2010.
% Reference notes: (i)   r = beta
%                  (ii)  t needs dividing by Ts
%                  (iii) Factor of sqrt(Ts) neglected.
% . . . . . . . . . . . . . . . . . . . . . . . . . . .

TOL = 1e-15;

h = zeros(tap_count, 1);
for idx = 1:tap_count
    % Make sure t=0 always occurs, even if Ntaps is even.
    % For Ntaps odd:  t = -(N-1)/2 : (N-1)/2 [symmetric]
    % For Ntaps even: t = -N/2 : N/2-1		 [assymetric]
    t = -floor(tap_count/2) + (idx-1);
    
    if abs(t) < TOL
        h(idx) = (1 - beta + 4*beta/pi)/sqrt(Ts);
    elseif beta ~= 0 && abs(abs(t) - Ts/(4*beta)) < TOL
        h(idx) = beta*((1+2/pi)*sin(pi/(4*beta)) + (1-2/pi)*cos(pi/(4*beta)))/sqrt(2*Ts);
    else
        num = sin(pi*t*(1-beta)/Ts) + 4*beta*t*cos(pi*t*(1+beta)/Ts)/Ts;
        den = pi*t*(1 - (4*beta*t/Ts)^2)/sqrt(Ts);
        h(idx) = num / den;
    end
    
end