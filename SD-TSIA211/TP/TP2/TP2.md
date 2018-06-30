En posant $ \widetilde{X} = (\tilde{x_i}, \tilde{x_2},\dots, \tilde{x_{n-1}},\tilde{x_n} )  \in \mathbb R^{(p+1) \times n }​$ ,où $ \tilde{x_i} =\begin{pmatrix} 1 \\ x_i \end{pmatrix}  \in R^{p+1}​$  ,  $\tilde{\omega} = \begin{pmatrix} \omega_0 \\ \omega_1 \end{pmatrix}  \in \mathbb R^{p+1}​$ et la matrice diagonale
$$
A = diag(0,1,1,\dots, 1) = 
\begin{pmatrix} 
0& 0& 0& \dots &\dots &0 \\
0& 1& 0& \dots &\dots &0 \\
\vdots &\vdots& \vdots&  \ddots & & \vdots\\
\vdots &\vdots &\vdots& &\ddots & \vdots \\
0& 0& 0& \dots  &\dots &1 
\end{pmatrix}
$$


  Alors la fonction peut être ecrits sous format ci-dessous:

$$ f_1: (\tilde{\omega}) \mapsto \frac{1}{n} \sum_{i=1}^{n} \log(1+exp(-y_i \tilde{x}_i^T\tilde{\omega})) + \frac{\rho}{2}\tilde{\omega}^ TA \tilde{\omega} $$

En posant 

$g(\tilde x_i, y_i, \tilde \omega) = $ 

Alors on obtiens le gradient 

$$ \nabla f =  \frac{1}{n} \sum_{i=1}^{n} \log(1+exp(-y_i \tilde{x}_i^T\tilde{\omega})) + \frac{\rho}{2}\tilde{\omega}^ TA \tilde{\omega} $$

