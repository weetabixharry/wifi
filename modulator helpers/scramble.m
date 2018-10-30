function [y, pattern] = scramble(x, init)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
% [y, pattern] = scramble(x, init)
% . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

% Generate the repeating 127-bit pattern for this init state
pattern = get_scramble_pattern(init);

% Do scramble
y = zeros(size(x));
ip = 1;
for i = 1:length(x)
    y(i) = xor(x(i), pattern(ip));
    if ip < length(pattern)
        ip = ip + 1;
    else
        ip = 1;
    end
end
    
end


function pattern = get_scramble_pattern(init)

assert(length(init) == 7);

period = 127;
pattern = zeros(period,1);
reg = init;
for i = 1:period
    pattern(i) = xor(reg(7), reg(4));
    
    reg(2:7) = reg(1:6);
    reg(1) = pattern(i);
end

end