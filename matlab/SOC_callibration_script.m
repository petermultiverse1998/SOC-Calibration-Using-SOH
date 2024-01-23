%% Parameters
T = 1;
initialSOC = 80;
initialCurrent = 40*5;
period = 60*60;


%% Supporting Functions 
% Remainder
function r = rem(num,den)
    r = num-den*floor(num/den);
end