function y = interleave(x, Nbpsc)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% function y = interleave(x, Nbpsc)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% N.B. Input must be columns, each of length Ncbps.
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

% Equation (17-17)
s = max(Nbpsc/2,1);

Ncbps = size(x, 1);
Nsyms = size(x, 2);

y = zeros(Ncbps, Nsyms);
for k = 0:Ncbps-1
    % First permuation
    i = (Ncbps/16) * mod(k,16) + floor(k/16);

    % Second permutation
    j = s * floor(i/s) + mod(i + Ncbps - floor(16 * i/Ncbps), s);
        
    for sym = 1:Nsyms
        y(1+j, sym) = x(1+k, sym);
    end
end
