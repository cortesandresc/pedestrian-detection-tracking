function nmsPositiveWindows = NMS(positiveWindows, threshold)
% positiveWindows is the windows identified as having people
% threshold is the threshold distance between two 

numBoxes = size(positiveWindows,1);
nmsCounts = zeros(numBoxes,1);

for i = 1:numBoxes
    % 1x2 
    posA = positiveWindows(i,:);
    for j = (i+1) : numBoxes
        % 1x2
        posB = positiveWindows(j,:);
        distance = norm(posA-posB);
        if (distance <= threshold)
            nmsCounts(i) = nmsCounts(i) + 1;
            nmsCounts(j) = nmsCounts(j) + 1;
        end
    end 
end

keepMask = logical(ones(numBoxes,1));
for i = 1:numBoxes
    % 1x2 
    posA = positiveWindows(i,:);
    countA = nmsCounts(i);
    for j = 1 : numBoxes
        % 1x2
        posB = positiveWindows(j,:);
        countB = nmsCounts(j);
        distance = floor(norm(posA-posB));
        % todo: check this part | countA < countB will fail when just 2
        if ((distance <= threshold) && (countA < countB))
            keepMask(i) = 0;
            break;
        end
    end 
end
%keepMask' % Uncomment to get an idea of how many its removing per frame
nmsPositiveWindows = positiveWindows(keepMask,:);
