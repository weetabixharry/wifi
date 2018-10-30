function lut = get_rate_info()
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% function lut = get_rate_info()
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% Fields are: RATE, r, Nbpsc, Ncbps, Ndbps, mbps
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

% References:
% [Table 17-3] Modulation-dependent parameters
% [Table 17-5] Contents of the SIGNAL field

lut = [
    make_struct('1101', 1/2, 1,  48,  24, 6 );
    make_struct('1111', 3/4, 1,  48,  36, 9 );
    make_struct('0101', 1/2, 2,  96,  48, 12);
    make_struct('0111', 3/4, 2,  96,  72, 18);
    make_struct('1001', 1/2, 4, 192,  96, 24);
    make_struct('1011', 3/4, 4, 192, 144, 36);
    make_struct('0001', 2/3, 6, 288, 192, 48);
    make_struct('0011', 3/4, 6, 288, 216, 54);
];

end

% HELPER function
function s = make_struct(RATE, r, Nbpsc, Ncbps, Ndbps, mbps)
    s = struct('RATE', RATE, 'r', r, 'Nbpsc', Nbpsc, 'Ncbps', Ncbps, 'Ndbps', Ndbps, 'mbps', mbps);
end
