# toleft测试
对于向量$\vec{pq}$如果点S在向量$\vec{pq}$的左边，则向量积$\vec{pq}\times\vec{qs} > 0$,也就是
$$
\begin{vmatrix}
x_p & y_p & 1 \\
x_q & y_q & 1 \\
x_s & y_s & 1 \\
\end{vmatrix}>0
$$

# 判断点是否在三角形内部
对于三角形pqs，如果：
toleft(p, q, r),
toleft(q, s, r),
toleft(s, p, r)的结果一样，说明r在pqs内部。