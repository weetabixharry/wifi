function y = puncture(x, r)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% y = puncture(x, r)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

% Note: These puncturing 'masks' are derived from [Figure 17-9] Example of
% the bit-stealing and bit-insertion procedure (r = 3/4, 2/3).
if r == 1/2
    y = x;
    return;
elseif r == 2/3
    mask = [1,1,1,0];
elseif r == 3/4
    mask = [1,1,1,0,0,1];
else
    error('Invalid/unsupported coding rate');
end

im = 0;
iy = 0;
y = zeros(ceil(length(x) * r), 1);
for i = 1:length(x)
    % Loop over the puncturing mask
    if im < length(mask)
        im = im + 1;
    else
        im = 1;
    end
    
    if mask(im)
        iy = iy + 1;
        y(iy) = x(i);
    end
end
% Discard any excess
y = y(1:iy);
