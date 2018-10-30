function y = bcc_encode(x)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% function y = bcc_encode(x)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% Implements the R=1/2 BCC encoder as described in 17.3.5.5 of the 802.11
% standard.
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

% Industry-standard generator polynomials
g0 = oct2poly(133);
g1 = oct2poly(171);

% Output is twice length of input
Nx = length(x);
Ny = 2*Nx;
y = zeros(Ny,1);

% Initialise to zero state? Can't see where this is explicitly specified...
reg = zeros(7,1);
for i = 1:Nx
    % Shift in new value
    reg(2:7) = reg(1:6);
    reg(1) = x(i);
    
    % Implement xor connections as per Figure 17.8 of 802.11 standard
    s0 = 0;
    s1 = 0;
    for k = 1:7
        if g0(k)
            s0 = xor(s0, reg(k));
        end
        if g1(k)
            s1 = xor(s1, reg(k));
        end
    end
    y(2*(i-1)+1) = s0;
    y(2*(i-1)+2) = s1;
end
