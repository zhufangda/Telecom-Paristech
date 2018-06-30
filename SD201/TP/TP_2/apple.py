#coding=utf-8
import sklearn as sk
import sys
import os
import matplotlib.pyplot as plt
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.neighbors import KNeighborsClassifier


company_path ='Documents/Company/'
fruit_path = 'Documents/Fruit/'

company_list = os.listdir(company_path)
fruit_list = os.listdir(fruit_path)

trainingCL = []
trainingFiles = []
testFiles = []
testCL = []

length = len(fruit_list)
for i in range(len(fruit_list)):
    with open(fruit_path + fruit_list[i], 'r', encoding='utf-8') as file:
        if(i< length*0.6):
            inputLines = [line for line in file if line.rstrip()]
            trainingFiles.extend(inputLines);
            trainingCL.extend(['fruit']* len(inputLines));
        else:
            testFiles.append(file.read());
            testCL.append('fruit');

length = len(company_list)
for i in range(len(company_list)):
    with open(company_path + company_list[i], 'r', encoding='utf-8') as file:
        if(i< length * 0.6):
            inputLines = [line for line in file if line.rstrip()]
            trainingFiles.extend(inputLines);
            trainingCL.extend(['company']*len(inputLines));
        else:
            testFiles.append(file.read());
            testCL.append('company');

count_vect = CountVectorizer(stop_words='english')

X_train_counts = count_vect.fit_transform(trainingFiles)
training = X_train_counts.toarray()

test = count_vect.transform(testFiles).toarray()
x = list(range(1,200))
y=[]
for i in range(1,200):
    neigh = KNeighborsClassifier(n_neighbors = i)
    neigh.fit(training,trainingCL)
    print(neigh.score(test,testCL));
    y.append(neigh.score(test,testCL))

plt.plot(x,y)
plt.show()







