function fcs = generate_crc(x)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% fcs = generate_crc(x)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% Computes CRC-32 checksum. Assumes input x is a vector of BYTES.
%
% N.B. This is more or less copied and pasted from:
% mathworks.com/matlabcentral/fileexchange/49518
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

% Initialize variables
fcs  = uint32(hex2dec('FFFFFFFF'));
poly = uint32(hex2dec('EDB88320'));
x = uint8(x);
% Compute CRC-32 value
imax = intmax('uint32');
for i = 1:length(x)
    fcs = bitxor(fcs,uint32(x(i)));
    for j = 1:8
        mask = bitcmp(bitand(fcs,uint32(1)));
        if mask == imax
            mask = 0;
        else
            mask = mask+1;
        end
        fcs = bitxor(bitshift(fcs,-1), bitand(poly,mask));
    end
end
fcs = bitcmp(fcs);
