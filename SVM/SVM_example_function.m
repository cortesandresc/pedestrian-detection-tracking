% This function shows how to find weights thetha using an svm (assumes
% linearly seperable

% The Iris dataset has 4 features per datum
feature_length = 4;
% Randomly initalize theta
theta = rand(1,feature_length);
theta = fminsearch(@svm_optimization_function, theta);

% To calculate test error:
% 1) predicted_labels = (test_data * theta') > 0
% 2) error = sum(test_labels - predicted labels)