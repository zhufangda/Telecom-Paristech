# Chapitre 1 First theoretical results on OLS(Ordinary Least Squares) 

## I. Définition of th OLS

We consider a regression model with n observations and p covariates. Aim is to predict an output $y$ with a lineair $n$ combination of covariates.

For $i=1,2,3,\dots,n$, 

$y_i$ are the observed output, 

$x_i = (x_{i,0},x_{i,1},\dots,x_{i,p})^T\in \mathbb R$ are the observed covariates with the convention that $x_{i,0} = 1$ 

Prediction(lineair) is given by $X_i^TQ = Q_0+\sum_{i=0}^{p}x_{i,k}Q_k$  where $Q \in \mathbb R^{p+1}$. The OLS estimator is given by 

$$\hat{Q_P} = \underset{Q \in \mathbb R^{p+1}}{\operatorname{argmax}} \sum_{i=1}^n(y_i-x_i^TQ)^2$$.

To tackle the existance unicity of such a défition, we introduce the notation 

$$ X = \begin{pmatrix} x_i^T \\ x_2^T \\ \vdots \\ x_n^T \end{pmatrix}  = \underbrace{ \begin{pmatrix} x_{1,0}  &\dots &x_{1,p} \\ \vdots & \ddots & \vdots \\ x_{n,0} & \dots & x_{n,p}\end{pmatrix}}_{P\, covariates \, with \,(p+1) \,columns} \in \mathbb R^{n \times (p+1)}​$$

$$Y = \begin{pmatrix} y_1 \\ \vdots \\ \vdots \\ y_n \end{pmatrix}  \in \mathbb R^n$$

We have $\sum_{i=0}^{n} (y_i-x_i^TQ)^2= || Y - XQ||^2$, where $|| \cdot || $is the Euclidean norme in $\mathbb R^n$.

The OLS has a nice geometric formulation $\hat{Y} = X\hat{Q_n} $ is the cloest point to $Y$ in the lineairspace $Span(X)$.

$Span(X) = $ lineair space generated bu the column of $X$.Remember: $(R^n, <\cdot>)$ is a Hilbert Space.  By the Hilbert projection, $\hat{Y} $ is unique and caracterized by the "norme equation". $\forall u \in Vect(x), <u,Y-\hat{Y}> = 0 $.  Equality, $\hat{Y}$ such that $X^T(Y-\hat{Y}) = 0$.  

*This does not mean that $\hat{Q_n}$ is unique but only that is existance satisfies*

<u>Proposition</u> The OLS estimator exists, we have two probabilities.

(i) $\hat{Q_n}$ is unique $\iff $ $X^TX$ is invertible $\iff$ $Ker(x) = {0}$

(ii) The OLS equation has an infinity of solutions $\iff$ $Ker(x) \neq {0}$ 

$Ker(X) = Ker(X^TX)$.

If $u \in ker(X) \implies u \in ker(X^TX)$.

if $u \in ker(X^TX), u^TX^TX = 0 \iff ||Xu|| = 0 \iff Xu = 0$

