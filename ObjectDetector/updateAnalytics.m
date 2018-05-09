function [updatedAnalytics] = updateAnalytics(positiveWindows, analytics)
% Store locations should be in [ [right, bottom, width, height]; [...] ; [...]]
storeLocations = analytics.stores;
updatedAnalytics = analytics;

numStores = size(storeLocations,1);
numPeople = size(positiveWindows,1);

% Update analytics variables 
updatedAnalytics.numFrames = updatedAnalytics.numFrames + 1;
updatedAnalytics.numPeople = updatedAnalytics.numPeople + numPeople;

% positiveWindows is [row col; row col;...] where that represents the top
% left corner
for i = 1:numStores
    right = storeLocations(i,1);
    bottom = storeLocations(i,2);
    width = storeLocations(i,3);
    height = storeLocations(i,4);
    top = bottom - height;
    left = right - width;
        
    for j = 1:numPeople
        xLoc = positiveWindows(j,2);
        yLoc = positiveWindows(j,1);
        if (xLoc <= right) && (xLoc >= left) && (yLoc >= top) && (yLoc <= bottom)
            updatedAnalytics.counts(i) = updatedAnalytics.counts(i) + 1;
        end
    end
end
end