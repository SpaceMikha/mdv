# La Mecánica Analítica del Movimiento Orbital
## Tratado de Física Matemática sobre Dinámica Celeste

**Mikhael da Silva** — *Investigador Independiente*
*Volumen complementario del proyecto Mission Design Visualizer*

**Idiomas:** [🇬🇧 English](PHYSICS.md) · [🇪🇸 Español](PHYSICS.es.md)

---

## Prefacio

Este tratado es un complemento de física matemática del *Mission Design Visualizer*. Su objeto es la estructura analítica de la mecánica celeste: la geometría del espacio fásico, los fundamentos variacionales de las ecuaciones del movimiento, la integrabilidad y regularización del problema de Kepler, los enunciados rigurosos de las teorías KAM y Nekhoróshev, las propiedades ergódicas de los flujos hamiltonianos, la jerarquía post-newtoniana y el análisis de error regresivo de los integradores preservadores de estructura. No contiene software, ni detalles de implementación, ni heurísticas de ingeniería.

La exposición presupone fluidez en geometría diferencial sobre variedades lisas (fibrados tangente y cotangente, formas diferenciales, derivadas de Lie), análisis funcional al nivel de variedades de Banach de caminos, y ecuaciones diferenciales ordinarias. Se utilizan libremente la teoría ergódica de la medida y la teoría elemental de Lie. Cada sección se articula en torno a los enunciados formales de sus teoremas principales; el comentario filosófico se ha restringido a lo necesario para motivar la matemática.

---

## Índice

1. [Preliminares matemáticos y notación](#1-preliminares-matemáticos-y-notación)
2. [Mecánica newtoniana sobre variedades de configuración](#2-mecánica-newtoniana-sobre-variedades-de-configuración)
3. [El problema de Kepler: estructura analítica](#3-el-problema-de-kepler-estructura-analítica)
4. [Regularización del problema de Kepler](#4-regularización-del-problema-de-kepler)
5. [Mecánica lagrangiana y cálculo de variaciones](#5-mecánica-lagrangiana-y-cálculo-de-variaciones)
6. [Mecánica hamiltoniana y geometría simpléctica](#6-mecánica-hamiltoniana-y-geometría-simpléctica)
7. [La ecuación de Hamilton–Jacobi y las variables de acción–ángulo](#7-la-ecuación-de-hamiltonjacobi-y-las-variables-de-acciónángulo)
8. [Integrabilidad de Liouville–Arnold](#8-integrabilidad-de-liouvillearnold)
9. [Teoría clásica de perturbaciones](#9-teoría-clásica-de-perturbaciones)
10. [Teoría KAM](#10-teoría-kam)
11. [Estimaciones de Nekhoróshev y difusión de Arnold](#11-estimaciones-de-nekhoróshev-y-difusión-de-arnold)
12. [Mecánica geométrica y reducción simpléctica](#12-mecánica-geométrica-y-reducción-simpléctica)
13. [Teoría ergódica de los flujos hamiltonianos](#13-teoría-ergódica-de-los-flujos-hamiltonianos)
14. [Mecánica celeste relativista](#14-mecánica-celeste-relativista)
15. [Integración numérica preservadora de estructura](#15-integración-numérica-preservadora-de-estructura)
16. [Referencias](#16-referencias)

---

## 1. Preliminares matemáticos y notación

Trabajamos sobre una variedad lisa $n$-dimensional $\mathcal{Q}$ (la *variedad de configuración*), típicamente riemanniana $(\mathcal{Q}, g)$. Los fibrados tangente y cotangente son $T\mathcal{Q}$ y $T^*\mathcal{Q}$; las coordenadas locales son $(q^i)$, $(q^i, \dot q^i)$, y $(q^i, p_i)$, con convención de suma de Einstein. $\mathcal{L}_X$ denota la derivada de Lie a lo largo de $X$; $\iota_X$ la contracción; $d$ la derivada exterior. El espacio de funciones lisas es $C^\infty(M)$; el espacio de $k$-formas es $\Omega^k(M)$. Para las variedades de Banach de caminos escribimos $C^r([a,b], \mathcal{Q})$.

A lo largo del texto, $G = 6{,}67430 \times 10^{-11}\,\mathrm{m^3\,kg^{-1}\,s^{-2}}$, $\mu = GM$, $\mu_\oplus = 398\,600{,}4418\,\mathrm{km^3\,s^{-2}}$ y $R_\oplus = 6378{,}137\,\mathrm{km}$. Trabajamos en unidades en las que, salvo indicación en contrario, $c = 1$ en las secciones relativistas.

**Hipótesis de regularidad.** Salvo mención expresa, se supone que los hamiltonianos y los lagrangianos son $C^\omega$ (analíticos reales) en sus dominios de definición. Las estimaciones en la teoría KAM y la de Nekhoróshev dependen finamente de los radios de analiticidad; denotamos por $\|H\|_{r}$ la norma de $H$ sobre una franja compleja de anchura $r$.

---

## 2. Mecánica newtoniana sobre variedades de configuración

### 2.1 Las ecuaciones del movimiento

Para un sistema mecánico con lagrangiano $L = \tfrac{1}{2} g_{ij}(q)\dot q^i \dot q^j - U(q)$, las ecuaciones del movimiento son la **ecuación geodésica con fuerza**:

$$\nabla_{\dot\gamma}\dot\gamma = -\,\mathrm{grad}\,U,$$

donde $\nabla$ es la conexión de Levi-Civita de $g$. Sobre $\mathcal{Q} = \mathbb{R}^3$ con métrica euclidiana, esto se reduce a la segunda ley de Newton

$$\ddot{\mathbf{r}} = -\nabla U(\mathbf{r}).$$

**Teorema (existencia y unicidad).** *Sea $U \in C^{1,1}_{\mathrm{loc}}(\mathcal{Q})$. Para cada $(q_0, v_0) \in T\mathcal{Q}$, existe una única solución maximal $\gamma \in C^2((-T_-, T_+), \mathcal{Q})$ con $\gamma(0) = q_0$, $\dot\gamma(0) = v_0$. Si $U$ está acotado inferiormente y $T_+ < \infty$, entonces $|\dot\gamma(t)| \to \infty$ cuando $t \to T_+$.*

Para el potencial de Kepler $U(r) = -\mu/r$, el conjunto singular $\{r = 0\}$ admite soluciones que lo alcanzan en tiempo finito (órbitas de colisión). Son las órbitas para las que la regularización resulta necesaria; cf. §4.

### 2.2 Reducción del problema de dos cuerpos

Sean $\mathbf{r}_1, \mathbf{r}_2 \in \mathbb{R}^3$ dos cuerpos de masas $m_1, m_2$. Definimos

$$\mathbf{R} = \frac{m_1 \mathbf{r}_1 + m_2 \mathbf{r}_2}{m_1 + m_2}, \qquad \mathbf{r} = \mathbf{r}_2 - \mathbf{r}_1.$$

La aplicación $(\mathbf{r}_1, \mathbf{r}_2) \mapsto (\mathbf{R}, \mathbf{r})$ es un difeomorfismo lineal $\mathbb{R}^6 \to \mathbb{R}^6$ de jacobiano $1$ que desacopla las ecuaciones del movimiento:

$$(m_1 + m_2)\ddot{\mathbf{R}} = 0, \qquad \mu_r\,\ddot{\mathbf{r}} = -G m_1 m_2 \frac{\mathbf{r}}{r^3}, \qquad \mu_r = \frac{m_1 m_2}{m_1 + m_2}.$$

El reescalamiento por $\mu_r$ produce la ecuación del movimiento en el marco del centro de masa:

$$\ddot{\mathbf{r}} = -\frac{\mu}{r^3}\mathbf{r}, \qquad \mu = G(m_1 + m_2).$$

### 2.3 Teorema de Bertrand

**Teorema (Bertrand, 1873).** *Sea $U: (0,\infty) \to \mathbb{R}$ lisa, y supongamos que toda órbita ligada del problema de fuerza central con potencial $U(r)$ es cerrada. Entonces $U(r) = -\mu/r$ (Kepler) o $U(r) = \tfrac12\omega^2 r^2$ (oscilador armónico), salvo constantes aditivas y multiplicativas.*

Una demostración por perturbación de órbitas circulares se halla en Arnold (1989), Apéndice 2C. El teorema aísla el problema de Kepler y el oscilador armónico como los únicos problemas de fuerza central en los que la frecuencia de oscilación radial y la frecuencia de revolución angular son conmensurables para toda órbita ligada — razón $1:1$ para Kepler, razón $1:2$ para el oscilador armónico.

---

## 3. El problema de Kepler: estructura analítica

### 3.1 Cantidades conservadas

El hamiltoniano de Kepler sobre $T^*(\mathbb{R}^3 \setminus \{0\})$,

$$H(\mathbf{r}, \mathbf{p}) = \frac{|\mathbf{p}|^2}{2} - \frac{\mu}{r},$$

admite tres integrales vectoriales independientes:

$$\mathbf{L} = \mathbf{r} \times \mathbf{p}, \qquad \mathbf{A} = \mathbf{p} \times \mathbf{L} - \mu \hat{\mathbf{r}},$$

el momento angular y el **vector de Laplace–Runge–Lenz**, junto con el escalar $H$. De estas siete componentes sólo son independientes cinco, en virtud de las identidades $\mathbf{A} \cdot \mathbf{L} = 0$ y $|\mathbf{A}|^2 = \mu^2 + 2 H |\mathbf{L}|^2$.

### 3.2 La ecuación de la órbita

Multiplicando vectorialmente $\ddot{\mathbf{r}} = -\mu \mathbf{r}/r^3$ por $\mathbf{L}$ e integrando una vez se obtiene

$$\mathbf{p} \times \mathbf{L} = \mu\hat{\mathbf{r}} + \mathbf{A}.$$

Haciendo producto escalar con $\mathbf{r}$ y denotando por $\nu$ el ángulo desde $\mathbf{A}$ hasta $\mathbf{r}$,

$$r(\nu) = \frac{p}{1 + e\cos\nu}, \qquad p = \frac{|\mathbf{L}|^2}{\mu}, \qquad e = \frac{|\mathbf{A}|}{\mu}.$$

La órbita es una cónica con foco en el origen. La relación $e^2 - 1 = 2 H |\mathbf{L}|^2 / \mu^2$ clasifica las órbitas ligadas por $H < 0$.

### 3.3 Teorema de Fock y la simetría dinámica $SO(4)$

**Teorema (Fock, 1935).** *Sobre el subespacio $\{H < 0\} \subset T^*(\mathbb{R}^3\setminus\{0\})$, los corchetes de Poisson de las componentes de $\mathbf{L}$ y $\tilde{\mathbf{A}} = \mathbf{A}/\sqrt{-2 H}$ satisfacen*

$$\{L_i, L_j\} = \varepsilon_{ijk} L_k, \quad \{L_i, \tilde A_j\} = \varepsilon_{ijk}\tilde A_k, \quad \{\tilde A_i, \tilde A_j\} = \varepsilon_{ijk} L_k,$$

*que es el álgebra de Lie $\mathfrak{so}(4) \cong \mathfrak{so}(3) \oplus \mathfrak{so}(3)$ bajo el cambio de base $\mathbf{J}^\pm = \tfrac12(\mathbf{L} \pm \tilde{\mathbf{A}})$.*

*Esbozo.* Los corchetes se comprueban por cálculo directo; la identidad $|\mathbf{A}|^2 - \mu^2 = 2 H |\mathbf{L}|^2$ garantiza que los casimires $|\mathbf{J}^\pm|^2$ coinciden y fijan la energía. $\square$

El teorema posee dos consecuencias de primer orden.

- *Contraparte cuántica.* En el átomo de hidrógeno, el análogo de $\mathbf{J}^\pm$ genera una representación unitaria de $SO(4)$; el número cuántico principal $n$ etiqueta las representaciones irreducibles, lo que produce la degeneración de energía $n^2$ — el espectro de Bohr es la estructura de multipletes de $SO(4)$.
- *Simetría oculta.* El problema de Kepler posee una simetría mayor que el $SO(3)$ geométrico. Como veremos en §8.3, esto es justamente lo que vuelve sus variables de acción–ángulo *degeneradas*: dos de las tres frecuencias fundamentales coinciden.

### 3.4 La ecuación de Kepler

Sean $M = n(t - t_0)$ con movimiento medio $n = \sqrt{\mu/a^3}$ la anomalía media, y $E$ la anomalía excéntrica. Entonces

$$M = E - e \sin E \qquad \text{(ecuación de Kepler)}.$$

Es una ecuación transcendente en $E$. La iteración de Newton

$$E_{k+1} = E_k - \frac{E_k - e\sin E_k - M}{1 - e\cos E_k}$$

converge cuadráticamente para $e < 1$, pues $|E_{k+1} - E_*| \leq \frac{e}{2(1-e)}|E_k - E_*|^2$. Para la semilla $E_0 = M + e\sin M$ y $e < 0{,}6627\ldots$ (el límite de Laplace), la convergencia está garantizada desde el primer paso.

---

## 4. Regularización del problema de Kepler

### 4.1 La singularidad de colisión

Las soluciones de $\ddot{\mathbf{r}} = -\mu\mathbf{r}/r^3$ pueden alcanzar $r = 0$ en tiempo finito; el campo vectorial es incompleto. Dos regularizaciones estándar convierten el flujo de Kepler en un flujo geodésico sobre una variedad compacta.

### 4.2 La regularización de Moser ($H < 0$)

**Teorema (Moser, 1970).** *El flujo del hamiltoniano de Kepler sobre la superficie de energía $\{H = -k^2/2\}$, tras la reparametrización temporal $ds/dt = 1/r$, es conjugado al flujo geodésico sobre el fibrado tangente unitario de $S^3$ perforado en el polo norte.*

La aplicación viene dada por la proyección estereográfica $\phi: \mathbb{R}^3 \to S^3$, $\mathbf{p} \mapsto \xi(\mathbf{p})$, elevada canónicamente a $T^*\mathbb{R}^3$. Bajo esta aplicación,

$$2 r (H + k^2/2) = |\xi|^2 - 1 + (\text{términos de ligadura}),$$

de modo que la superficie de energía se convierte en $|\xi|^2 = 1$, y el flujo resulta geodésico sobre $S^3$. Esto realiza geométricamente la simetría oculta $SO(4)$: $SO(4)$ es el grupo de isometrías de $S^3$.

### 4.3 Regularización de Kustaanheimo–Stiefel

Para el movimiento tridimensional, la **transformación de Kustaanheimo–Stiefel (KS)** eleva $\mathbb{R}^3$ a $\mathbb{R}^4$ mediante la aplicación cuaterniónica

$$\mathbf{r} = L(\mathbf{u})\mathbf{u}, \qquad L(\mathbf{u}) = \begin{pmatrix} u_1 & -u_2 & -u_3 & u_4 \\ u_2 & u_1 & -u_4 & -u_3 \\ u_3 & u_4 & u_1 & u_2 \\ u_4 & -u_3 & u_2 & -u_1 \end{pmatrix},$$

restringida a la ligadura bilineal $u_1 u_4 - u_2 u_3 + u_3 u_2 - u_4 u_1 = 0$. Bajo el tiempo de Sundman $ds = dt/r$, el hamiltoniano regularizado

$$\mathcal{H}(\mathbf{u}, \mathbf{v}) = \tfrac{1}{8}|\mathbf{v}|^2 - \tfrac{1}{2}E_0|\mathbf{u}|^2 - \mu$$

es el de un oscilador armónico cuatridimensional con energía fija $-\mu$. El flujo de Kepler sobre una superficie de energía fija $\{H = E_0\}$ se vuelve así *lineal* en variables KS — la reducción analítica más fuerte posible del problema.

---

## 5. Mecánica lagrangiana y cálculo de variaciones

### 5.1 El funcional de acción

Fijemos extremos $q_0, q_1 \in \mathcal{Q}$ y un intervalo $[t_0, t_1]$. Sea $\Omega = \{\gamma \in C^1([t_0,t_1], \mathcal{Q}) : \gamma(t_0) = q_0, \gamma(t_1) = q_1\}$. La **acción**

$$S: \Omega \to \mathbb{R}, \qquad S[\gamma] = \int_{t_0}^{t_1} L(\gamma, \dot\gamma, t)\, dt,$$

es un funcional $C^1$ sobre la variedad de Banach $\Omega$.

**Teorema (Euler–Lagrange).** *Una curva $\gamma \in \Omega \cap C^2$ es punto crítico de $S$ si y sólo si*

$$\frac{d}{dt}\frac{\partial L}{\partial \dot q^i} - \frac{\partial L}{\partial q^i} = 0.$$

### 5.2 La segunda variación y los campos de Jacobi

La segunda variación de $S$ en un punto crítico $\gamma$ es, para variaciones $\eta \in T_\gamma\Omega$,

$$\delta^2 S[\gamma](\eta, \eta) = \int_{t_0}^{t_1}\left[\eta^T\!\Big(\frac{d^2 L}{d\dot q\, d\dot q}\Big)\ddot\eta + 2 \eta^T\!\Big(\frac{d^2 L}{d\dot q\, dq}\Big)\dot\eta + \eta^T\!\Big(\frac{d^2 L}{dq\, dq}\Big)\eta\right] dt.$$

Una variación $\eta$ para la que $\delta^2 S[\gamma](\eta, \cdot) = 0$ es un **campo de Jacobi**; satisface una EDO lineal de segundo orden, la ecuación de Jacobi. Un punto $\gamma(t_*) \in \mathcal{Q}$ es **conjugado** de $\gamma(t_0)$ a lo largo de $\gamma$ si existe un campo de Jacobi no nulo que se anula en ambos $t_0$ y $t_*$.

**Teorema (índice de Morse).** *El índice de Morse de $\gamma$ — la dimensión del subespacio maximal sobre el que $\delta^2 S[\gamma]$ es definido negativo — es igual al número de puntos de $(t_0, t_1)$ conjugados a $\gamma(t_0)$, contados con multiplicidad.*

Para el problema de Kepler, el semiperíodo de oscilación radial separa puntos conjugados; las órbitas ligadas tienen índice de Morse $2k$ tras $k$ semi-revoluciones.

### 5.3 Teorema de Noether

Sea un grupo monoparamétrico $\phi_\varepsilon$ de difeomorfismos de $\mathcal{Q}$ que se eleva a $T\mathcal{Q}$ de modo que $L \circ \phi_\varepsilon^{T\mathcal{Q}} = L$. Sea $X = \partial_\varepsilon|_{\varepsilon=0}\phi_\varepsilon$ su generador infinitesimal.

**Teorema (Noether, 1918).** *La función*

$$I = \frac{\partial L}{\partial \dot q^i} X^i(q)$$

*es constante a lo largo de las soluciones de las ecuaciones de Euler–Lagrange.*

La construcción de la aplicación momento del §12 es la formulación global libre de coordenadas de este teorema.

---

## 6. Mecánica hamiltoniana y geometría simpléctica

### 6.1 La forma simpléctica canónica

Sobre $T^*\mathcal{Q}$ con coordenadas locales $(q^i, p_i)$, la **uno-forma tautológica** es $\theta = p_i\, dq^i$; la **forma simpléctica canónica** es $\omega = -d\theta = dq^i \wedge dp_i$.

**Teorema (Darboux).** *Sea $(M, \omega)$ una variedad simpléctica de dimensión $2n$. Para todo $x \in M$ existen coordenadas $(q^i, p_i)$ en un entorno de $x$ en las que $\omega = dq^i \wedge dp_i$.*

Todas las variedades simplécticas son, por tanto, localmente equivalentes; la geometría simpléctica no tiene invariantes locales más allá de la dimensión. Los invariantes globales — anchura de Gromov, capacidades simplécticas, homología de Floer — pertenecen a la topología simpléctica y quedan fuera de nuestro alcance actual.

### 6.2 Campos hamiltonianos y corchetes de Poisson

Para $H \in C^\infty(M)$, el **campo vectorial hamiltoniano** $X_H$ se define por $\iota_{X_H}\omega = dH$. El flujo $\phi^t_H$ satisface $\mathcal{L}_{X_H}\omega = 0$ (fórmula de Cartan), por lo que todo flujo hamiltoniano es un simplectomorfismo.

El **corchete de Poisson** es

$$\{f, g\} = \omega(X_f, X_g) = X_f(g) = -X_g(f).$$

Satisface la identidad de Jacobi $\{f, \{g, h\}\} + \{g, \{h, f\}\} + \{h, \{f, g\}\} = 0$, haciendo de $(C^\infty(M), \{\cdot, \cdot\})$ un álgebra de Lie; esta álgebra de Lie es una extensión del álgebra de campos hamiltonianos por las constantes.

### 6.3 Teorema de Liouville

**Teorema (Liouville).** *El flujo hamiltoniano preserva la forma de volumen de Liouville $\Omega_L = \omega^n / n!$.*

*Demostración.* $\mathcal{L}_{X_H}\Omega_L = \mathcal{L}_{X_H}(\omega^n/n!) = \omega^{n-1} \wedge \mathcal{L}_{X_H}\omega / (n-1)! = 0$, ya que $\mathcal{L}_{X_H}\omega = 0$. $\square$

### 6.4 Teorema de recurrencia de Poincaré

**Teorema (Poincaré, 1890).** *Sea $\phi: M \to M$ que preserva una medida finita $\mu$, y sea $A \subset M$ medible con $\mu(A) > 0$. Entonces $\mu$-casi todo $x \in A$ retorna a $A$ infinitas veces bajo iteración de $\phi$.*

En una superficie de energía acotada de un sistema hamiltoniano, la medida de volumen inducida es finita; por tanto, casi toda trayectoria es recurrente. Este enunciado, rigurosamente demostrado, es lo bastante fuerte como para que su combinación con la segunda ley de la termodinámica requiera cautela: ambos no son contradictorios, pero los tiempos de recurrencia son doblemente exponenciales en el número de grados de libertad y astronómicamente mayores que las escalas cosmológicas.

---

## 7. La ecuación de Hamilton–Jacobi y las variables de acción–ángulo

### 7.1 La ecuación de Hamilton–Jacobi

Buscamos una transformación canónica $(q, p) \mapsto (Q, P)$ generada por $W(q, P)$ mediante $p_i = \partial W/\partial q^i$, $Q^i = \partial W/\partial P_i$, de modo que el nuevo hamiltoniano sea $K = E(P)$. La condición es la **ecuación de Hamilton–Jacobi**:

$$H\!\left(q, \frac{\partial W}{\partial q}\right) = E.$$

Una *integral completa* $W(q, \alpha_1, \ldots, \alpha_n)$ con $\det(\partial^2 W/\partial q \partial \alpha) \neq 0$ proporciona $n$ constantes del movimiento independientes $P_i = \alpha_i$, y sus conjugadas $Q^i = \partial W/\partial \alpha_i - t\,\partial E/\partial \alpha_i$ son constantes.

### 7.2 Separabilidad y teorema de Stäckel

La ecuación de Hamilton–Jacobi es *separable en coordenadas ortogonales* $(q^1, \ldots, q^n)$ si $W = \sum_i W_i(q^i, \alpha)$. El teorema de Stäckel da condiciones necesarias y suficientes sobre $g_{ij}$ y $U$ para la separabilidad. Para el problema de Kepler, la separabilidad se verifica en coordenadas esféricas, parabólicas y esferoidales — un sistema superintegrable, reflejo de la simetría $SO(4)$ del §3.3.

### 7.3 Variables de acción–ángulo

Sobre un sistema integrable, las **variables de acción** se definen por

$$I_i = \frac{1}{2\pi}\oint_{\gamma_i} p\,dq,$$

siendo $\gamma_i$ el $i$-ésimo ciclo fundamental del toro invariante. Las **variables de ángulo** conjugadas $\varphi_i$ se definen módulo $2\pi$ y evolucionan linealmente:

$$\dot I_i = 0, \qquad \dot\varphi_i = \omega_i(I) = \frac{\partial H}{\partial I_i}.$$

### 7.4 Variables de Delaunay para el problema de Kepler

Las variables canónicas de acción–ángulo del problema de Kepler son

$$\begin{array}{lll}
L = \sqrt{\mu a}, & \ell = M, & \text{(anomalía media)}\\
G = L\sqrt{1-e^2}, & g = \omega, & \text{(argumento del periapsis)}\\
H = G\cos i, & h = \Omega, & \text{(longitud del nodo ascendente)}
\end{array}$$

con hamiltoniano $H_{\text{Kep}} = -\mu^2/(2 L^2)$. Puesto que $H_{\text{Kep}}$ depende sólo de $L$, las frecuencias son $(\omega_L, \omega_G, \omega_H) = (n, 0, 0)$: el sistema es **degenerado** en el sentido de Kolmogórov, al poseer menos frecuencias independientes que grados de libertad. Esta degeneración es la expresión hamiltoniana del carácter cerrado de las órbitas keplerianas ligadas (§3.3) y constituye el principal obstáculo técnico para una aplicación ingenua de la teoría KAM a la dinámica de satélites terrestres.

---

## 8. Integrabilidad de Liouville–Arnold

### 8.1 Enunciado

**Teorema (Liouville–Arnold).** *Sea $(M^{2n}, \omega)$ una variedad simpléctica, y sean $F_1 = H, F_2, \ldots, F_n \in C^\infty(M)$ tales que*

- *$\{F_i, F_j\} = 0$ para todos $i, j$ (involución);*
- *$dF_1, \ldots, dF_n$ son linealmente independientes sobre $M_f = \{F_i = f_i\}$;*
- *$M_f$ es compacta y conexa.*

*Entonces $M_f$ es difeomorfa al toro $\mathbb{T}^n$, y existen coordenadas de acción–ángulo $(I, \varphi) \in U \times \mathbb{T}^n$ en un entorno de $M_f$, en las que $\omega = dI \wedge d\varphi$ y $H = H(I)$.*

La demostración procede mostrando que los flujos hamiltonianos conmutantes $\phi^{t_1}_{F_1}, \ldots, \phi^{t_n}_{F_n}$ definen una acción localmente libre de $\mathbb{R}^n$ sobre $M_f$; la compacidad obliga a que la isotropía sea un retículo de rango $n$, de modo que $M_f = \mathbb{R}^n / \Lambda \cong \mathbb{T}^n$.

### 8.2 Resonancia y no degeneración

La aplicación $I \mapsto \omega(I) = \partial H/\partial I$ es la **aplicación de frecuencias**. Un valor $I_*$ es **resonante** si $\mathbf{k} \cdot \omega(I_*) = 0$ para algún $\mathbf{k} \in \mathbb{Z}^n \setminus \{0\}$; en caso contrario, **no resonante**. El sistema es **no degenerado** (Kolmogórov) en $I_*$ si $\det(\partial \omega/\partial I)(I_*) \neq 0$, y **no degenerado isoenergéticamente** (Arnold) si el hessiano orlado de $H$ respecto de $I$ es no singular sobre la superficie de energía.

### 8.3 El problema de Kepler es degenerado

De $H_{\text{Kep}} = -\mu^2/(2L^2)$, $\partial^2 H/\partial I_j \partial I_k = 0$ para $(j,k) \neq (1,1)$; la aplicación de frecuencias tiene rango $1$. El problema de Kepler es *completamente* degenerado — la degeneración más fuerte posible. La teoría perturbativa para sistemas cercanos a kepleriano procede vía el **hamiltoniano secular** obtenido por promediado sobre $\ell$, el cual restituye la no degeneración genérica en las variables $(G, H, g, h)$ tras eliminar el ángulo rápido.

---

## 9. Teoría clásica de perturbaciones

### 9.1 Planteamiento

Sea $H = H_0(I) + \varepsilon H_1(I, \varphi)$, con $H_1$ $2\pi$-periódica en $\varphi$, analítica sobre una franja compleja

$$D_{r,s} = \{|\Im \varphi_i| < s, \ \mathrm{dist}(I, D) < r\}.$$

Desarrollemos

$$H_1(I, \varphi) = \sum_{\mathbf{k} \in \mathbb{Z}^n} \hat H_1^\mathbf{k}(I)\, e^{i\mathbf{k}\cdot\varphi}.$$

Buscamos una transformación canónica, generada por $W(I', \varphi) = I'\cdot\varphi + \varepsilon S(I', \varphi) + O(\varepsilon^2)$, que elimine la dependencia angular a primer orden en $\varepsilon$.

### 9.2 La ecuación homológica

La condición de primer orden es

$$\omega_0(I') \cdot \partial_\varphi S = \langle H_1 \rangle(I') - H_1(I', \varphi),$$

donde $\langle H_1 \rangle = \hat H_1^{\mathbf{0}}$ es el promedio. Desarrollando en serie de Fourier,

$$S(I', \varphi) = \sum_{\mathbf{k} \neq 0} \frac{\hat H_1^\mathbf{k}(I')}{i\, \mathbf{k}\cdot\omega_0(I')}\, e^{i\mathbf{k}\cdot\varphi}.$$

Los denominadores $\mathbf{k}\cdot\omega_0$ son los **divisores pequeños**. Su acumulación en la resonancia es el principal obstáculo para la convergencia.

### 9.3 Dinámica secular de órbitas perturbadas por $J_2$

Promediando el hamiltoniano de $J_2$

$$H_{J_2} = -\frac{\mu J_2 R_\oplus^2}{2 r^3}\left(3\sin^2\Phi - 1\right), \qquad \Phi = \text{latitud},$$

sobre la anomalía media se obtiene $\langle H_{J_2}\rangle$ dependiente sólo de $(L, G, H)$. Las ecuaciones de Hamilton dan entonces las tasas seculares de Brouwer:

$$\begin{aligned}
\dot\Omega &= -\frac{3}{2}\, n\, J_2\left(\frac{R_\oplus}{p}\right)^{\!2}\cos i, \\
\dot\omega &= \phantom{-}\frac{3}{4}\, n\, J_2\left(\frac{R_\oplus}{p}\right)^{\!2}(5\cos^2 i - 1), \\
\dot M_0 &= n + \frac{3}{4}\, n\, J_2\left(\frac{R_\oplus}{p}\right)^{\!2}\sqrt{1-e^2}\,(3\cos^2 i - 1),
\end{aligned}$$

donde $p = a(1-e^2)$. La inclinación crítica $\cos^2 i_c = 1/5$, $i_c \approx 63{,}435°$, anula $\dot\omega$; la condición heliosincrónica $\dot\Omega = 360°/\mathrm{año}$ determina $i_{\text{SSO}}(a, e)$ implícitamente.

### 9.4 Divergencia en general

**Teorema (Poincaré, 1892).** *Para una perturbación analítica real genérica de un hamiltoniano integrable no degenerado, la serie formal de Lindstedt*

$$H_0 + \varepsilon \langle H_1\rangle + \varepsilon^2 H_2' + \cdots$$

*diverge. Genéricamente, no existe ninguna integral analítica adicional del movimiento aparte del propio $H$.*

La convergencia se restablece sobre un conjunto de Cantor de condiciones iniciales en virtud de la teoría KAM (§10); la estabilidad exponencialmente larga de las acciones sobre un conjunto abierto la suministra la teoría de Nekhoróshev (§11).

---

## 10. Teoría KAM

### 10.1 Frecuencias diofánticas

Un vector de frecuencias $\omega \in \mathbb{R}^n$ es **diofántico** con exponentes $(\gamma, \tau)$, $\gamma > 0$, $\tau \geq n-1$, si

$$|\mathbf{k} \cdot \omega| \geq \frac{\gamma}{|\mathbf{k}|^\tau} \quad \text{para todo } \mathbf{k} \in \mathbb{Z}^n \setminus \{0\}.$$

Denotemos el conjunto de tales $\omega$ por $\Omega_{\gamma, \tau}$. Para $\tau > n - 1$ fijo, la medida de $\Omega_{\gamma, \tau} \cap B_R$ satisface

$$|\mathbb{R}^n \setminus \Omega_{\gamma, \tau}| \cap B_R \leq C_\tau\, \gamma\, R^{n-1},$$

de modo que casi todo $\omega$ es diofántico para algún $\gamma$.

### 10.2 El teorema KAM

**Teorema (Kolmogórov 1954, Arnold 1963, Moser 1962).** *Sea $H_0(I)$ analítico real sobre $D \subset \mathbb{R}^n$ con $\det(\partial^2 H_0/\partial I^2) \neq 0$. Sea $H_\varepsilon = H_0 + \varepsilon H_1$ con $H_1$ analítico real sobre $D \times \mathbb{T}^n$. Fijemos $\gamma, \tau$ con $\tau > n-1$. Existen $\varepsilon_0 > 0$ y $C > 0$ dependientes de $H_0$, $\|H_1\|$, $\gamma$, $\tau$ tales que para todo $|\varepsilon| < \varepsilon_0$:*

*(i) Existe un conjunto de tipo Cantor $K_\varepsilon \subset D$ de medida de Lebesgue positiva, $|D \setminus K_\varepsilon| \leq C\sqrt{\varepsilon}$, con $K_\varepsilon \subset \omega_0^{-1}(\Omega_{\gamma,\tau})$;*

*(ii) Para cada $I_0 \in K_\varepsilon$, existe un embebido $\iota_{I_0}: \mathbb{T}^n \hookrightarrow D \times \mathbb{T}^n$, analítico real en $\varphi$ y $C^\infty$-Whitney en $I_0$, cuya imagen es invariante bajo el flujo hamiltoniano de $H_\varepsilon$ y sobre la cual el flujo es conjugado a $\varphi \mapsto \varphi + \omega_0(I_0)\, t$.*

### 10.3 Idea de la demostración: superconvergencia

La iteración clásica de Lindstedt converge como una serie geométrica de razón $\varepsilon/\gamma^2$, que fracasa siempre que $\gamma$ deba reducirse. La innovación de Kolmogórov es la **iteración tipo Newton**: en el paso $k$, habiendo eliminado la dependencia angular hasta $O(\varepsilon_k)$, el paso siguiente la elimina hasta $O(\varepsilon_k^2)$. Incluso con pérdidas por divisores pequeños en cada paso, la convergencia cuadrática domina:

$$\varepsilon_{k+1} \leq C_k \varepsilon_k^2, \qquad \varepsilon_k \leq \varepsilon_0^{2^k} \cdot \text{(factores de crecimiento lento)}.$$

La estimación funciona con tal de que la franja de analiticidad se contraiga geométricamente en cada paso y la condición diofántica se preserve. Las demostraciones completas se hallan en Arnold–Kozlov–Neishtadt (2006), §4, y en Chierchia–Mather (2010).

### 10.4 Toros de dimensión inferior y Graff–Eliasson

En los sistemas degenerados — incluido el problema de Kepler — se requieren teoremas KAM para toros invariantes de *dimensión inferior* (no maximales). Graff (1974), Eliasson (1988) y Pöschel (1989) demostraron que, bajo *condiciones de no resonancia de Melnikov* que involucran tanto frecuencias normales como tangenciales, las familias de toros de dimensión inferior persisten bajo perturbación. Estos resultados subyacen en la teoría moderna del movimiento planetario: el sistema solar posee un conjunto de medida positiva de condiciones iniciales cuasi-periódicas, aun cuando no se conozca ningún toro KAM aislado de dimensión maximal que lo contenga.

---

## 11. Estimaciones de Nekhoróshev y difusión de Arnold

### 11.1 Pendiente y teorema de Nekhoróshev

Una función $h: \mathbb{R}^n \to \mathbb{R}$ es **pronunciada** (*steep*) sobre $D$ si, grosso modo, su restricción a cualquier subespacio afín carece de puntos críticos en el interior. Las funciones convexas son pronunciadas; las cuasiconvexas también lo son; el hamiltoniano de Kepler, aun siendo degenerado, se torna pronunciado tras el promediado secular en las variables $(G, H)$.

**Teorema (Nekhoróshev, 1977).** *Sea $H_0$ analítico real y pronunciado sobre $D \subset \mathbb{R}^n$. Sea $H_\varepsilon = H_0 + \varepsilon H_1$ con $\|H_1\| \leq 1$. Existen constantes $\varepsilon_0, T_0, C > 0$ y exponentes $a, b > 0$ dependientes sólo de $n$ y de los índices de pendiente, tales que, para todo $|\varepsilon| < \varepsilon_0$ y toda acción inicial $I_0$:*

$$|I(t) - I_0| \leq \varepsilon^b \qquad \text{para todo}\quad |t| \leq T_0 \exp\!\left((\varepsilon_0/\varepsilon)^a\right).$$

Para $H_0$ convexo, los exponentes óptimos son $a = b = 1/(2n)$ (Lochak, 1992). El teorema se aplica sobre *toda* condición inicial — no sobre un conjunto de Cantor — pero sólo durante tiempos exponencialmente largos, no infinitos.

### 11.2 Difusión de Arnold

Para $n \geq 3$ grados de libertad, los toros KAM supervivientes de codimensión uno *no* folian la superficie de energía; las regiones complementarias son conexas. Arnold (1964) construyó un ejemplo explícito de sistema hamiltoniano en el que las órbitas derivan a través de regiones resonantes, recorriendo un rango finito en el espacio de acciones — la **difusión de Arnold**. La teoría moderna (Mather, Bernard, Cheng, Xia, Kaloshin, Marco, Sabbagh) establece la genericidad de la difusión en las categorías $C^r$ y analítica, con velocidades acotadas por la estimación de Nekhoróshev.

Relevancia astrodinámica: el caos observado a largo plazo en el sistema solar interior (Laskar, 1989–2013) se cree manifestación de una difusión lenta a lo largo de redes resonantes. Las integraciones numéricas sobre $10^9$ años exhiben un tiempo de Lyapunov de $\sim 5\,$Ma y una probabilidad de expulsión de Mercurio del orden del $1\%$ en los próximos $5\,$Ga — tasa consistente con las escalas de Nekhoróshev para los parámetros observados del sistema.

---

## 12. Mecánica geométrica y reducción simpléctica

### 12.1 Acciones de grupo y la aplicación momento

Sea $G$ que actúa sobre $(M, \omega)$ mediante simplectomorfismos, con generador infinitesimal $\xi_M$ para $\xi \in \mathfrak{g}$. Una **aplicación momento** es una aplicación lisa $\mathbf{J}: M \to \mathfrak{g}^*$ que satisface

$$\iota_{\xi_M}\omega = d\langle \mathbf{J}, \xi\rangle \quad \text{para todo } \xi \in \mathfrak{g}.$$

Cuando $\mathbf{J}$ es $\mathrm{Ad}^*$-equivariante, entrelaza la acción de $G$ sobre $M$ con la acción coadjunta sobre $\mathfrak{g}^*$.

**Teorema (Noether, forma hamiltoniana).** *Si $H$ es $G$-invariante y $\mathbf{J}$ es una aplicación momento, entonces $\mathbf{J}$ es constante a lo largo del flujo de $X_H$.*

### 12.2 Reducción de Marsden–Weinstein

**Teorema (Marsden–Weinstein, 1974).** *Sea $\mathbf{J}: M \to \mathfrak{g}^*$ una aplicación momento $\mathrm{Ad}^*$-equivariante para una acción libre y propia de $G$, y sea $\mu \in \mathfrak{g}^*$ un valor regular. Sea $G_\mu$ el subgrupo de isotropía de $\mu$ bajo la acción coadjunta. Entonces*

$$M_\mu := \mathbf{J}^{-1}(\mu) / G_\mu$$

*es una variedad simpléctica lisa de dimensión $\dim M - \dim G - \dim G_\mu$. Todo hamiltoniano $G$-invariante $H$ sobre $M$ desciende a un hamiltoniano $H_\mu$ sobre $M_\mu$, y las soluciones de las ecuaciones de Hamilton de $H_\mu$ se elevan (de manera no unívoca) a soluciones de las de $H$.*

### 12.3 Reducción del problema de Kepler

El problema de Kepler sobre $T^*\mathbb{R}^3$ es $SO(3)$-invariante, con aplicación momento $\mathbf{J} = \mathbf{L}$. Para $\mu = \mathbf{L}_0 \neq 0$, $G_\mu = SO(2)$, y la reducción produce

$$\dim M_\mu = 6 - 3 - 1 = 2,$$

una superficie simpléctica sobre la cual la dinámica reducida es el movimiento radial en el potencial efectivo $V_{\text{ef}}(r) = L_0^2/(2r^2) - \mu/r$. La invariancia $SO(4)$ adicional permite una segunda etapa de reducción, tras la cual el problema de Kepler colapsa en un punto: toda órbita con $(H, L, A)$ fijos es un único punto del espacio doblemente reducido.

### 12.4 Reconstrucción y fases geométricas

Dada una solución del sistema reducido, reconstruir la dinámica completa requiere integrar la **conexión mecánica** a lo largo de la trayectoria reducida. La holonomía de esta conexión a lo largo de una órbita reducida cerrada es la **fase geométrica** (ángulo de Hannay), dada por

$$\Delta\varphi_{\text{geom}} = \oint_{\gamma_{\text{red}}} \mathcal{A},$$

con $\mathcal{A}$ la uno-forma de conexión mecánica. Para un satélite en un plano orbital que precesa, el ángulo de Hannay contribuye un desplazamiento medible del argumento del periapsis adicional a la deriva secular — un efecto puramente cinemático de reconstrucción rotacional.

---

## 13. Teoría ergódica de los flujos hamiltonianos

### 13.1 Ergodicidad

Sea $(\phi^t)$ que preserva una medida de probabilidad $\mu$ sobre $M$. El flujo es **ergódico** si todo conjunto medible $\phi^t$-invariante tiene medida $0$ o $1$.

**Teorema (Birkhoff, 1931).** *Si $(\phi^t, \mu)$ es ergódico y $f \in L^1(\mu)$, entonces*

$$\lim_{T\to\infty} \frac{1}{T}\int_0^T f(\phi^t x)\, dt = \int_M f\, d\mu \quad \text{para } \mu\text{-c.t. } x.$$

Los flujos hamiltonianos son genéricamente *no* ergódicos: el teorema KAM construye un conjunto de medida positiva de órbitas cuasi-periódicas (no ergódicas). La ergodicidad, cuando se verifica, lo hace sobre superficies de energía individuales, no sobre el espacio fásico completo — y, aun así, sólo tras eliminar los toros KAM.

### 13.2 Exponentes de Lyapunov y Oseledets

**Teorema (Oseledets, 1968).** *Sea $(\phi^t, \mu)$ un flujo preservador de medida sobre una variedad riemanniana, con $\log\|D\phi^t\| \in L^1(\mu)$. Para $\mu$-c.t. $x$, existen números reales $\lambda_1(x) > \cdots > \lambda_k(x)$ (los exponentes de Lyapunov) y una descomposición $T_x M = \bigoplus_j E_j(x)$ tales que*

$$\lim_{t\to\infty}\frac{1}{t}\log\|D\phi^t(x) v\| = \lambda_j(x) \quad \text{para todo } v \in E_j(x) \setminus \{0\}.$$

Para flujos hamiltonianos sobre una variedad simpléctica de dimensión $2n$, el espectro de Lyapunov es simétrico: $\lambda_{2n+1-j} = -\lambda_j$. Dos exponentes de Lyapunov se anulan idénticamente (la dirección del flujo y el gradiente de energía).

### 13.3 El tiempo de Lyapunov del sistema solar

La evidencia numérica (Sussman y Wisdom 1988, 1992; Laskar 1989, 2009) da el tiempo de Lyapunov $T_\lambda = 1/\lambda_{\max}$ del sistema solar interior como aproximadamente $5\,\mathrm{Ma}$. La incertidumbre en las condiciones iniciales a escala métrica crece, por tanto, hasta la escala de la unidad astronómica en $\approx 150\,$Ma; la predicción determinista de las posiciones planetarias en escalas geológicas es matemáticamente imposible, incluso bajo gravitación clásica.

### 13.4 Mezcla y decaimiento de correlaciones

La ergodicidad es un enunciado débil; la *mezcla* es más fuerte. Un flujo es **mezclante** si $\mu(A \cap \phi^{-t}B) \to \mu(A)\mu(B)$ cuando $t \to \infty$. Para los sistemas hiperbólicos (Anósov, flujos geodésicos sobre variedades de curvatura negativa, billares en mesas dispersantes), el decaimiento exponencial de correlaciones para observables de Hölder es conocido (Dolgopyat, Liverani). Para los sistemas hamiltonianos con espacio fásico dividido — la situación genérica en mecánica celeste — el decaimiento es como mucho polinómico y está gobernado por la pegajosidad de los toros KAM.

---

## 14. Mecánica celeste relativista

### 14.1 La ecuación geodésica

En relatividad general, un cuerpo en caída libre de masa despreciable sigue una geodésica temporal de una métrica lorentziana $g$. En coordenadas locales,

$$\ddot x^\mu + \Gamma^\mu_{\nu\rho}\dot x^\nu \dot x^\rho = 0, \qquad \Gamma^\mu_{\nu\rho} = \tfrac12 g^{\mu\sigma}(\partial_\nu g_{\sigma\rho} + \partial_\rho g_{\sigma\nu} - \partial_\sigma g_{\nu\rho}).$$

La métrica queda determinada por las ecuaciones de campo de Einstein,

$$R_{\mu\nu} - \tfrac12 R g_{\mu\nu} + \Lambda g_{\mu\nu} = \frac{8\pi G}{c^4} T_{\mu\nu}.$$

### 14.2 La solución de Schwarzschild

La única solución vacua estática, esféricamente simétrica y asintóticamente plana es

$$ds^2 = -\left(1 - \frac{r_s}{r}\right)c^2\, dt^2 + \left(1 - \frac{r_s}{r}\right)^{-1} dr^2 + r^2(d\theta^2 + \sin^2\theta\, d\phi^2), \qquad r_s = \frac{2 G M}{c^2}.$$

### 14.3 Ecuación geodésica en el plano ecuatorial

Haciendo $\theta = \pi/2$ y utilizando los vectores de Killing $\partial_t, \partial_\phi$ (energía $\tilde E$, momento angular $\tilde L$), la ecuación radial para partículas masivas es

$$\left(\frac{dr}{d\lambda}\right)^2 = \tilde E^2 - \left(1 - \frac{r_s}{r}\right)\left(1 + \frac{\tilde L^2}{r^2}\right).$$

El potencial efectivo contiene, además de los términos newtonianos, una contribución $-r_s \tilde L^2 / r^3$ responsable de la precesión relativista del perihelio y de la existencia de órbitas circulares estables más internas.

### 14.4 Desarrollo post-newtoniano

El desarrollo en $v/c$ y $r_s/r$ produce las **ecuaciones de movimiento post-newtonianas parametrizadas (PPN)**,

$$\ddot{\mathbf{r}} = -\frac{\mu}{r^3}\mathbf{r} + \frac{\mu}{c^2 r^3}\left[\left((2\beta + 2\gamma)\frac{\mu}{r} - \gamma v^2\right)\mathbf{r} + 2(1 + \gamma)(\mathbf{r}\cdot\dot{\mathbf{r}})\dot{\mathbf{r}}\right] + O(c^{-4}),$$

con $\beta = \gamma = 1$ en relatividad general. El efecto orbital dominante es una precesión secular del periapsis,

$$\Delta\omega_{\text{RG}} = \frac{6\pi G M}{c^2 a(1 - e^2)} = \frac{3\pi r_s}{a(1-e^2)} \quad \text{por órbita}.$$

Para Mercurio ($a \approx 0{,}387\,\mathrm{UA}$, $e \approx 0{,}2056$), esto da $\Delta\omega_{\text{RG}} \approx 0{,}1035''$ por órbita, es decir $42{,}98''$/siglo — en acuerdo con la observación hasta la precisión con que ambos miembros se conocen.

### 14.5 Arrastre de marcos y efecto Lense–Thirring

Para un cuerpo central en rotación con momento angular $\mathbf{S}$, la métrica de Kerr introduce un término gravitomagnético que produce la **precesión de Lense–Thirring** del momento angular orbital,

$$\dot{\mathbf{L}}_{\text{LT}} = \frac{2 G}{c^2 a^3 (1 - e^2)^{3/2}}\left[\mathbf{S} - 3(\mathbf{S}\cdot\hat{\mathbf{n}})\hat{\mathbf{n}}\right] \times \mathbf{L},$$

con $\hat{\mathbf{n}}$ la normal unitaria al plano orbital. Medida por Gravity Probe B (Everitt et al., 2011) en $37{,}2 \pm 7{,}2\,\mathrm{mas/año}$, en acuerdo con la predicción de la RG $39{,}2\,\mathrm{mas/año}$.

---

## 15. Integración numérica preservadora de estructura

### 15.1 Integradores simplécticos

Una aplicación de un paso $\Phi_h: M \to M$ es **simpléctica** si $\Phi_h^* \omega = \omega$. La composición de aplicaciones simplécticas es simpléctica; escindiendo un hamiltoniano $H = T(p) + V(q)$ y componiendo $\phi^{h/2}_T \circ \phi^h_V \circ \phi^{h/2}_T$ se obtiene el esquema **Störmer–Verlet (leapfrog)**, un integrador simpléctico simétrico de segundo orden. Los compuestos de orden superior (Yoshida, 1990) proveen orden par arbitrario mediante composiciones anidadas con coeficientes cuidadosamente escogidos.

### 15.2 Análisis de error regresivo

**Teorema (error regresivo, Benettin–Giorgilli, Hairer–Lubich–Wanner).** *Sea $\Phi_h$ un integrador simpléctico analítico real de orden $p$ para un hamiltoniano analítico real $H$, sobre un dominio compacto. Existen un hamiltoniano formal*

$$\tilde H(h) = H + h^p H_p + h^{p+1} H_{p+1} + \cdots$$

*y constantes $C, h_0 > 0$ tales que, truncando la serie en el índice óptimo $N(h) \sim h_0 / h$,*

$$\|\Phi_h(z) - \phi^h_{\tilde H_N}(z)\| \leq C h\, \exp(-h_0/h).$$

En términos llanos: un integrador simpléctico no resuelve $H$, sino que *resuelve exactamente* (salvo errores exponencialmente pequeños) un hamiltoniano cercano $\tilde H$. La trayectoria numérica es una trayectoria exacta de un problema ligeramente perturbado — una garantía extraordinariamente fuerte.

### 15.3 Consecuencias para la integración orbital

- **Energía.** $H(\Phi_h^n z_0) - H(z_0) = O(h^p)$ y *no deriva* en escalas exponencialmente largas $n \leq \exp(h_0/h)$.
- **KAM.** El hamiltoniano perturbado $\tilde H$ admite sus propios toros KAM; con tal de que $h$ sea suficientemente pequeño para que $\tilde H$ yazca dentro del radio KAM de $H$, las trayectorias numéricas capturan correctamente el movimiento cuasi-periódico en tiempos astronómicamente largos.
- **Primeras integrales.** Los invariantes cuadráticos (momento angular de acciones hamiltonianas lineales) se preservan *exactamente* por los métodos de Runge–Kutta de Gauss–Legendre; los invariantes genéricos (vector LRL) se preservan sólo aproximadamente.

### 15.4 Métodos no simplécticos

Los métodos clásicos de Runge–Kutta (RK4, RK45, DOPRI5) no son simplécticos: no poseen hamiltoniano modificado, y los errores de energía se acumulan *secularmente*,

$$|H(\Phi_h^n z_0) - H(z_0)| \leq C n h^{p+1},$$

en lugar de permanecer acotados. Para propagaciones más allá de unos pocos tiempos de Lyapunov, esta deriva secular domina, y se pierden rasgos cualitativos del flujo (estructura del periapsis, captura resonante). Para los cálculos de análisis de misión de corta duración — el régimen del Mission Design Visualizer — el error permanece numéricamente aceptable; para las integraciones del sistema solar a largo plazo, los métodos simplécticos son indispensables.

### 15.5 Sombreado

**Teorema (Anósov, Bowen).** *Sea $\phi^t$ el flujo de un sistema hamiltoniano uniformemente hiperbólico sobre un conjunto invariante compacto $\Lambda$. Para cada $\varepsilon > 0$ existe $\delta > 0$ tal que toda $\delta$-seudoórbita $(z_n)_{n \in \mathbb{Z}}$ es $\varepsilon$-sombreada por una órbita verdadera: existe $z$ con $\|\phi^{nh}(z) - z_n\| < \varepsilon$ para todo $n$.*

Una trayectoria computada numéricamente es, genéricamente, una seudoórbita. El teorema de sombreado afirma que, en régimen hiperbólico, alguna órbita verdadera del sistema se halla uniformemente próxima a ella — no la que emana de la condición inicial nominal, sino alguna órbita del mismo sistema. Es la garantía más fuerte disponible de la fidelidad del cálculo numérico en regímenes caóticos.

---

## 16. Referencias

**Mecánica analítica y geometría simpléctica**

- Arnold, V. I. *Mathematical Methods of Classical Mechanics* (2ª ed.). Springer, 1989.
- Abraham, R. y Marsden, J. E. *Foundations of Mechanics* (2ª ed.). Benjamin/Cummings, 1978.
- Marsden, J. E. y Ratiu, T. S. *Introduction to Mechanics and Symmetry* (2ª ed.). Springer, 1999.
- McDuff, D. y Salamon, D. *Introduction to Symplectic Topology* (3ª ed.). Oxford University Press, 2017.
- Souriau, J.-M. *Structure des Systèmes Dynamiques*. Dunod, 1970.

**Mecánica celeste**

- Poincaré, H. *Les Méthodes Nouvelles de la Mécanique Céleste*, vols. I–III. Gauthier-Villars, 1892–1899.
- Brouwer, D. y Clemence, G. M. *Methods of Celestial Mechanics*. Academic Press, 1961.
- Morbidelli, A. *Modern Celestial Mechanics*. Taylor & Francis, 2002.
- Szebehely, V. *Theory of Orbits: The Restricted Problem of Three Bodies*. Academic Press, 1967.
- Stiefel, E. L. y Scheifele, G. *Linear and Regular Celestial Mechanics*. Springer, 1971.
- Moser, J. «Regularization of Kepler's problem and the averaging method on a manifold». *Comm. Pure Appl. Math.* 23 (1970), 609–636.

**KAM, Nekhoróshev y difusión de Arnold**

- Arnold, V. I., Kozlov, V. V. y Neishtadt, A. I. *Mathematical Aspects of Classical and Celestial Mechanics* (3ª ed.). Springer, 2006.
- Moser, J. *Stable and Random Motions in Dynamical Systems*. Princeton University Press, 1973.
- Chierchia, L. y Mather, J. N. «Kolmogorov–Arnold–Moser Theory». *Scholarpedia* 5(9):2123, 2010.
- Nekhoroshev, N. N. «An exponential estimate of the time of stability of nearly-integrable Hamiltonian systems». *Russian Math. Surveys* 32 (1977), 1–65.
- Lochak, P. «Canonical perturbation theory via simultaneous approximation». *Russian Math. Surveys* 47 (1992), 57–133.
- Arnold, V. I. «Instability of dynamical systems with several degrees of freedom». *Soviet Math. Dokl.* 5 (1964), 581–585.
- Laskar, J. «A numerical experiment on the chaotic behaviour of the solar system». *Nature* 338 (1989), 237–238.

**Teoría ergódica**

- Katok, A. y Hasselblatt, B. *Introduction to the Modern Theory of Dynamical Systems*. Cambridge University Press, 1995.
- Cornfeld, I. P., Fomin, S. V. y Sinai, Ya. G. *Ergodic Theory*. Springer, 1982.

**Relatividad general**

- Misner, C. W., Thorne, K. S. y Wheeler, J. A. *Gravitation*. W. H. Freeman, 1973.
- Wald, R. M. *General Relativity*. University of Chicago Press, 1984.
- Weinberg, S. *Gravitation and Cosmology*. Wiley, 1972.
- Will, C. M. *Theory and Experiment in Gravitational Physics* (2ª ed.). Cambridge University Press, 2018.

**Integración geométrica y simpléctica**

- Hairer, E., Lubich, C. y Wanner, G. *Geometric Numerical Integration* (2ª ed.). Springer, 2006.
- Leimkuhler, B. y Reich, S. *Simulating Hamiltonian Dynamics*. Cambridge University Press, 2004.
- Benettin, G. y Giorgilli, A. «On the Hamiltonian interpolation of near-to-the-identity symplectic mappings». *J. Stat. Phys.* 74 (1994), 1117–1143.
- Yoshida, H. «Construction of higher order symplectic integrators». *Phys. Lett. A* 150 (1990), 262–268.

---

*© 2026 Mikhael da Silva. Licenciado bajo Creative Commons Attribution-NoDerivatives 4.0 International (CC BY-ND 4.0).*
