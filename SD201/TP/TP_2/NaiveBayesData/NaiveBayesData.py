#!/usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np

from sklearn.naive_bayes import MultinomialNB
from sklearn.feature_extraction.text import CountVectorizer

k = 10
data_set = []

apple_company_data = []
with open('apple_the_company.txt', 'r', encoding='utf-8') as file:
    apple_company_data.extend(file.readlines())
print("Get instance about apple compnay: " + str(len(apple_company_data)))

apple_fruit_data = []
with open('apple_the_fruit.txt', 'r', encoding='utf-8') as file:
    apple_fruit_data.extend(file.readlines())
print("Get instance about apple fruit: " + str(len(apple_fruit_data)))

banana_data = []
with open('banana.txt', 'r', encoding='utf-8') as file:
    banana_data.extend(file.readlines())
print("Get instance about banana: " + str(len(apple_fruit_data)))

microsoft_data = []
with open('microsoft.txt', 'r', encoding='utf-8') as file:
    microsoft_data.extend(file.readlines())
print("Get instance about microsoft: " + str(len(apple_fruit_data)))


data_set.append(apple_company_data)
data_set.append(apple_fruit_data)
data_set.append(banana_data)
data_set.append(microsoft_data)

test_size = len(apple_fruit_data)//10
count_vect = CountVectorizer(stop_words='english')

accuracy = []

for i in range(0, k):

    # get test set
    test_data = []
    test_cl = []
    for j in range(0, len(data_set)):
        test_data += data_set[j][i*test_size:(i+1)*test_size]
        test_cl += [j] * test_size

    # get training set
    training_set = []
    training_cl = []
    for j in range(0, len(data_set)):
        training_set += data_set[j][0:i*test_size] + data_set[j][(i+1)*test_size:]
        training_cl += [j] * test_size * (k - 1)

    training = count_vect.fit_transform(training_set).toarray()
    test = count_vect.transform(test_data).toarray()

    clf = MultinomialNB()
    clf.fit(training, training_cl)

    resultat = clf.score(test, test_cl)
    print(resultat)
    accuracy.append(resultat)

print('accuracy:' + str(np.mean(accuracy)))
