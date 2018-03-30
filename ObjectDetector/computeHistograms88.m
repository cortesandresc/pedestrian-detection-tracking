function [histograms88] = computeHistograms88(imgGradient)

[imgRows, imgCols, ~] = size(imgGradient);
cellSize = [8, 8];    % [rows, cols]
nbins = 9;            % [0 180) in 20º intervals
angle_interval = 180 / nbins;

histogramRows = int32(imgRows / cellSize(1));
histogramCols = int32(imgCols / cellSize(2));
histograms88 = zeros(histogramRows, histogramCols, nbins);
for hRow = 1:histogramRows
    for hCol = 1:histogramCols
        startRow = (hRow-1)*cellSize(1) + 1;
        startCol = (hCol-1)*cellSize(2) + 1;
        endRow = startRow + cellSize(1) - 1;
        endCol = startCol + cellSize(2) - 1;
        histogram = zeros(1, nbins);
        for row = startRow:endRow
            for col = startCol:endCol
                angle = mod(imgGradient(row, col, 2)+90.0, 180); % [0 180)
                index1 = floor(angle / angle_interval)+1; % lower bucket
                index2 = mod(index1, nbins)+1;            % upper bucket
                remainder = mod(angle, angle_interval);
                ratio = remainder / angle_interval;
                magnitude = imgGradient(row, col, 1);
                histogram(index1) = histogram(index1) + magnitude*(1-ratio);
                histogram(index2) = histogram(index2) + magnitude*(ratio);
            end
        end
        histograms88(hRow, hCol, :) = histogram;
    end
end

end