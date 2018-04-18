
% HOG vectors should be 1 x 3780

%% Get training HOGs
trainPosDir = 'Train/pos/';
trainNegDir = 'Train/neg/';
trainposfiles = dir(strcat(trainPosDir, '*.png'));
nposfiles = length(trainposfiles);
trainnegfiles = dir(strcat(trainNegDir, '*.png'));
nnegfiles = length(trainnegfiles);
trainHOGs = zeros(nposfiles+nnegfiles, 3780);
trainLabels = zeros(nposfiles+nnegfiles, 1);
for i = 1:nposfiles
    filename = strcat(trainPosDir, trainposfiles(i).name);
    img = imread(filename);
    img = img(17:144, 17:80,:);
    trainHOGs(i, :) = hog(img);
    trainLabels(i, :) = 1;
end
for i = 1:nnegfiles
    filename = strcat(trainNegDir, trainnegfiles(i).name);
    img = imread(filename);
    img = img(17:144, 17:80,:);
    trainHOGs(i+nposfiles, :) = hog(img);
    trainLabels(i+nposfiles, :) = -1;
end

%% Get testing HOGs

testPosDir = 'Test/pos/';
testNegDir = 'Test/neg/';
testposfiles = dir(strcat(testPosDir, '*.png'));
nposfiles = length(testposfiles);
testnegfiles = dir(strcat(testNegDir, '*.png'));
nnegfiles = length(testnegfiles);
testHOGs = zeros(nposfiles+nnegfiles, 3780);
testLabels = ones(nposfiles+nnegfiles, 1);
for i = 1:nposfiles
    filename = strcat(testPosDir, testposfiles(i).name);
    img = imread(filename);
    img = img(4:131, 4:67, :);
    testHOGs(i, :) = hog(img);
    testLabels(i, :) = 1;
end
for i = 1:nnegfiles
    filename = strcat(testNegDir, testnegfiles(i).name);
    img = imread(filename);
    img = img(4:131, 4:67, :);
    testHOGs(i+nposfiles, :) = hog(img);
    testLabels(i+nposfiles, :) = -1;
end

%% Save the things
train_features = trainHOGs;
train_labels = trainLabels;
save('../data/train_features.mat', 'train_features');
save('../data/train_labels.mat', 'train_labels');

test_features = testHOGs;
test_labels = testLabels;
save('../data/test_features.mat', 'test_features');
save('../data/test_labels.mat', 'test_labels');


features = testHOGs;
labels = testLabels;
save('../data/test_features.mat', 'features');
save('../data/test_labels.mat', 'labels');


















