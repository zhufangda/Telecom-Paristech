import numpy as np

def load_cervical_cancer(filename):
    """
    Cette fonction lit le fichier filename, par exemple
    filename = 'riskfactorscervicalcancer.csv'
    Elle retourne 
    X : une matrice de caracteristiques
    y : un vecteur des classes tel que si y[i] = 1, il y a un fort risque de cancer du col de l'uterus, et si y[i] = -1, les tests sont negatifs

    Pour plus d'infos sur la base de donnees,
    http://archive.ics.uci.edu/ml/datasets/Cervical+cancer+%28Risk+Factors%29#
    """

    data = np.loadtxt(filename, delimiter=',', skiprows=1)

    # predire si un des tests est positif
    y = np.max(data[:, -4:], axis=1) * 2 - 1
    # reequilibrer la base de donnees 
    pos_indices = np.where(y > 0)[0]
    neg_indices = np.where(y < 0)[0]
    indices = pos_indices.tolist() + neg_indices[1::7].tolist()

    y = y[indices]
    X = data[indices, :-4]

    # Standardisation de la matrice
    X = X - np.mean(X, axis=0)
    std_ = np.std(X, axis=0)
    X = X[:, std_ > 0]
    std_ = std_[std_ > 0]
    X = X / std_

    return X, y


