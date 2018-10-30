function y = map_qam(x)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% function y = map_qam(x)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% N.B. Input must be columns, each of length Nbpsc.
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Nbpsc = size(x, 1);

% [Figure 17-10] BPSK, QPSK, 16-QAM, and 64-QAM constellation bit encoding.
% See also Tables 17-7 to 17-10.
s = max(Nbpsc/2,1);
offset = -2^s + 1;
re = offset + 2*bin2gray(bi2de(x(1:s,:).', 'left-msb'), 'qam', 2^Nbpsc);
if Nbpsc > 1
    im = offset + 2*bin2gray(bi2de(x(s+1:end,:).', 'left-msb'), 'qam', 2^Nbpsc);
else
    im = zeros(size(re)); % (BPSK)
end
% [Table 17-6] Modulation-dependent normalization factor Kmod
if Nbpsc == 1
    % BPSK
    Kmod = 1;
elseif Nbpsc == 2
    % QPSK
    Kmod = 1 / sqrt(2);
elseif Nbpsc == 4
    % 16-QAM
    Kmod = 1 / sqrt(10);
elseif Nbpsc == 6
    % 64-QAM
    Kmod = 1 / sqrt(42);
else
    error('Invalid or unsupported Nbpsc');
end

% Apply scale
y = Kmod * complex(re,im);
