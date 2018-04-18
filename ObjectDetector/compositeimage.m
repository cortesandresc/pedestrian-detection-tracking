function [] = compositeimage()

rows = 10;
cols = 12;
n = rows*cols;
imgRows = rows * 128;
imgCols = cols * 64;
composite_image = uint8(zeros(imgRows, imgCols, 3));

posdir = 'Train/pos/';
posfiles = dir(strcat(posdir, '*.png'));
nposfiles = length(posfiles);
negdir = 'Train/neg/';
negfiles = dir(strcat(negdir, '*.png'));
nnegfiles = length(negfiles);

for i = 0:n-1
    row = floor(i / cols);
    col = mod(i, cols);
    startRow = (row)*128 + 1;
    endRow = startRow + 128-1;
    startCol = (col)*64 + 1;
    endCol = startCol + 64-1;
    pos = rand > 1.1;
    %pos = round(rand);
    if (pos)
        idx = int16(rand * nposfiles);
        filename = strcat(posdir, posfiles(idx).name);
        img = imread(filename);     
        selectedImage = img(17:144, 17:80, :);
    else
        idx = int16(rand * nnegfiles);
        filename = strcat(negdir, negfiles(idx).name);
        img = imread(filename);
        selectedImage = img(17:144, 17:80,:);
    end
    composite_image(startRow:endRow,startCol:endCol,:) = selectedImage;
end

imwrite(composite_image, 'composite_img1.jpg');

end