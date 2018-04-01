% This function shows how to find weights thetha using an svm (assumes
% linearly seperable

x = load('../data/features');
y = load('../data/labels');
x = x.features;
y = y.labels;

% Remove invalid data
p = zeros(size(x(1,:)));
er_x = x * p';
locs = (er_x ~= 0);
x(locs,:) = [];
y(locs,:) = [];

global x y;

% Include the last value as the bias
feature_length = size(x,2) + 1;

% Randomly initalize theta
theta = rand(1,feature_length) - 0.5;
options = optimset('MaxIter',1000);
theta = fminsearch(@svm_optimization_function, theta,options);

% To calculate test error:
% 1) predicted_labels = (test_data * theta') > 0
% 2) error = sum(test_labels - predicted labels)

x = load('../data/test_features');
y = load('../data/test_labels');
x = x.features;
y = y.labels;

% Remove invalid data
p = zeros(size(x(1,:)));
er_x = x * p';
locs = (er_x ~= 0);
x(locs,:) = [];
y(locs,:) = [];

% Extract weights and bias
bias = theta(end);
theta = theta(1:end-1);

% Calculate predictions and error
labels = y .* (x * theta' + bias);
sum(labels > 0 ) / size(y,1)