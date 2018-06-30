import numpy as np
import sklearn.tree as tree
import graphviz
from sklearn.datasets import load_iris
import pydotplus
from IPython.display import  Image,display
from sklearn.model_selection import cross_val_score

training_data = np.load('training_data.npy')
training_class = np.load('training_class.npy')
test_data = np.load('test_data.npy')
test_class = np.load('test_class.npy')

clf = tree.DecisionTreeClassifier(criterion='gini', random_state=np.random.RandomState(130), min_samples_split= 4 )
clf.fit(training_data, training_class)

iris = load_iris()
dot_data = tree.export_graphviz(clf,out_file ='tree.dot',feature_names=iris['feature_names'], class_names=True)
graph = graphviz.Source(dot_data)
dot = pydotplus.graph_from_dot_file('tree.dot')


print(type(dot))
graph.render("tree")

training_accuracy = clf.score(training_data, training_class)
test_accuracy = clf.score(test_data,test_class)
print("Training Accuracy:" + str(training_accuracy))
print("Test Accuracy:" + str(test_accuracy))
