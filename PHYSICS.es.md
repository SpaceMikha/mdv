# La Física del Movimiento Orbital
## Tratado Teórico y Filosófico de Mecánica Celeste

**Mikhael da Silva** — *Investigador Independiente*
*Volumen complementario del proyecto Mission Design Visualizer*

**Idiomas:** [🇬🇧 English](PHYSICS.md) · [🇪🇸 Español](PHYSICS.es.md)

---

> *Una órbita es la geometría de una caída que jamás llega — el aplazamiento perpetuo de la colisión en curvatura.*

---

## Prefacio

Este documento es el complemento teórico y filosófico del *Mission Design Visualizer*. Se restringe deliberadamente a la física del movimiento orbital: no contiene software, ni detalles de implementación, ni heurísticas de ingeniería. Su objeto es la estructura de la mecánica celeste en tanto teoría matemática y en tanto objeto de reflexión filosófica.

La obra se articula a lo largo de un único arco dialéctico. Comenzamos con la imagen newtoniana, en la que los cuerpos se ven sometidos a fuerzas que se propagan instantáneamente a través de un espacio euclidiano inerte. Reformulamos esta imagen de modo variacional, después canónico, y al hacerlo descubrimos que la órbita no es primordialmente una trayectoria en el espacio físico, sino un flujo sobre una variedad simpléctica. Examinamos la integrabilidad del problema de Kepler, la quiebra de la integrabilidad bajo perturbación, y la emergencia del caos determinista. Concluimos con la reinterpretación relativista general de la gravitación como curvatura del espacio-tiempo, y con una meditación sobre el estatuto epistémico de la propagación numérica.

La exposición presupone fluidez en mecánica clásica, cálculo vectorial y ecuaciones diferenciales ordinarias. La familiaridad con la geometría diferencial y la dinámica hamiltoniana es deseable, pero no estrictamente imprescindible; las estructuras requeridas se introducen allí donde por primera vez se tornan indispensables.

---

## Índice

1. [Prolegómenos: La filosofía de la mecánica celeste](#1-prolegómenos-la-filosofía-de-la-mecánica-celeste)
2. [El paradigma newtoniano](#2-el-paradigma-newtoniano)
3. [Leyes de conservación y geometría del problema de Kepler](#3-leyes-de-conservación-y-geometría-del-problema-de-kepler)
4. [La formulación lagrangiana](#4-la-formulación-lagrangiana)
5. [La formulación hamiltoniana y el espacio fásico](#5-la-formulación-hamiltoniana-y-el-espacio-fásico)
6. [Variables de acción–ángulo e integrabilidad](#6-variables-de-acciónángulo-e-integrabilidad)
7. [Teoría de perturbaciones y el colapso de la integrabilidad](#7-teoría-de-perturbaciones-y-el-colapso-de-la-integrabilidad)
8. [Caos, teoría KAM y el problema de los tres cuerpos](#8-caos-teoría-kam-y-el-problema-de-los-tres-cuerpos)
9. [Mecánica geométrica y reducción por simetría](#9-mecánica-geométrica-y-reducción-por-simetría)
10. [Correcciones relativistas generales](#10-correcciones-relativistas-generales)
11. [La discretización como acto epistemológico](#11-la-discretización-como-acto-epistemológico)
12. [Meditaciones finales](#12-meditaciones-finales)
13. [Referencias y bibliografía complementaria](#13-referencias-y-bibliografía-complementaria)

---

## 1. Prolegómenos: La filosofía de la mecánica celeste

### 1.1 La peculiaridad epistémica del conocimiento orbital

La mecánica celeste ocupa una posición epistémica peculiar entre las ciencias físicas. Sus objetos — planetas, satélites, cometas — son inaccesibles a la intervención. No podemos perturbar a Júpiter para observar su recuperación, ni mantener fijo a Marte para aislar una variable. El laboratorio de la mecánica celeste es el único que existe; el experimento es el que está en curso. Todo nuestro conocimiento es, por tanto, observacional, y toda observación es conocimiento de un sistema ya en marcha.

Esta postura no intervencionista impuso, desde la Antigüedad, una disciplina inusualmente abstracta. Los epiciclos de Ptolomeo no eran teorías falsas a la espera de refutación; eran representaciones matemáticas, dispositivos geométricos que *salvaban los fenómenos* (σῴζειν τὰ φαινόμενα) sin comprometerse con afirmaciones causales. La presunción moderna de que una teoría debe ser además una afirmación sobre lo que *realmente existe* es una herencia post-newtoniana, y una herencia sobre la que merece la pena detenerse.

### 1.2 De la descripción a la causalidad

El tránsito de Kepler a Newton es el tránsito de una ciencia cinemática a una ciencia dinámica. Las tres leyes de Kepler — órbitas elípticas, áreas iguales en tiempos iguales, la relación armónica $T^2 \propto a^3$ — son descriptivas; codifican regularidades observadas. La ley newtoniana de la gravitación universal es *causal*; afirma una relación entre las masas de los cuerpos y las fuerzas que constriñen su movimiento, y deriva las leyes de Kepler como consecuencias de un principio más profundo.

Este tránsito no estuvo exento de coste metafísico. El propio Newton se sintió profundamente incómodo con la acción a distancia: que un cuerpo *aquí* ejerciera una fuerza instantánea sobre un cuerpo *allá*, a través del espacio vacío, sin medio interpuesto, le parecía

> *un absurdo tan grande que creo que ningún hombre que, en materias filosóficas, posea una facultad de pensar competente podría jamás caer en él.*
> — Carta a Bentley, 25 de febrero de 1693

La mecánica de Newton funcionaba. Pero los *Principia* no explicaban la gravedad; la cuantificaban. La explicación — la reinterpretación de la gravitación como curvatura del espacio-tiempo — habría de esperar dos siglos y medio, y volveremos a ella en el §10.

### 1.3 La órbita como objeto matemático

Una pregunta fundamental de la filosofía de la mecánica celeste es: *¿qué clase de objeto es una órbita?* Compiten tres respuestas:

1. **La órbita como trayectoria.** La órbita es la curva trazada en el espacio físico por un cuerpo sometido a influencia gravitatoria. Es la visión ingenua, suficiente para la navegación y la observación, pero tergiversa la estructura matemática de la teoría.

2. **La órbita como curva integral.** La órbita es una solución de una ecuación diferencial — una curva $\gamma(t)$ que satisface $\ddot{\mathbf{r}} = -\mu \mathbf{r}/r^3$ con condiciones iniciales prescritas. Es la visión newtoniana.

3. **La órbita como hoja simpléctica.** La órbita es una variedad integral unidimensional de un campo vectorial hamiltoniano sobre una variedad simpléctica de seis dimensiones (el espacio fásico), foliada por los conjuntos de nivel de las integrales del movimiento en toros invariantes. Es la visión geométrica moderna, y es la que este tratado acabará por sostener.

El ascenso de (1) a (3) no es mero formalismo. Cada reformulación revela estructuras invisibles al nivel precedente. La forma elíptica de una órbita kepleriana ligada es consecuencia de una simetría oculta $SO(4)$. La estabilidad del sistema solar es una afirmación sobre la medida de los toros invariantes bajo perturbación. Estos no son hechos visibles en las trayectorias; son hechos visibles en la geometría del espacio que las trayectorias habitan.

---

## 2. El paradigma newtoniano

### 2.1 La ley de la gravitación universal

La ley de Newton afirma que dos masas puntuales $m_1$ y $m_2$ separadas por $\mathbf{r}_{12} = \mathbf{r}_2 - \mathbf{r}_1$ se atraen mutuamente con una fuerza

$$\mathbf{F}_{12} = -G \frac{m_1 m_2}{r_{12}^3}\, \mathbf{r}_{12},$$

donde $G = 6{,}67430 \times 10^{-11}\,\mathrm{m^3\,kg^{-1}\,s^{-2}}$ es la constante de gravitación universal. Tres rasgos de esta ley merecen señalarse.

En primer lugar, la fuerza es *central*: está dirigida a lo largo de la recta que une las dos masas. Esta es la raíz geométrica de la conservación del momento angular.

En segundo lugar, la fuerza es *conservativa*: es el gradiente de un potencial escalar,

$$\mathbf{F}_{12} = -\nabla_1 U(r_{12}), \qquad U(r) = -\frac{G m_1 m_2}{r}.$$

Esta es la raíz geométrica de la conservación de la energía.

En tercer lugar, la dependencia respecto a la separación es exactamente $1/r^2$. Ninguna otra ley de potencia inversa produce órbitas cerradas y no precesantes para condiciones iniciales ligadas arbitrarias — un teorema debido a **Bertrand (1873)**. Que nuestro universo exhiba precisamente esta potencia es, en cierto sentido, lo que hace analíticamente tratable a la mecánica celeste; una fuerza $1/r^{2+\varepsilon}$ convertiría toda órbita en una roseta precesante, y la tradición clásica que va de Kepler a Newton no existiría.

### 2.2 Reducción al problema de un cuerpo

Consideremos dos cuerpos $m_1$ y $m_2$ bajo su gravitación mutua. Definimos el centro de masa $\mathbf{R} = (m_1\mathbf{r}_1 + m_2\mathbf{r}_2)/(m_1+m_2)$ y la posición relativa $\mathbf{r} = \mathbf{r}_2 - \mathbf{r}_1$. La tercera ley de Newton garantiza $\ddot{\mathbf{R}} = 0$: el centro de masa se mueve uniformemente, cual partícula libre.

La coordenada relativa satisface

$$\mu_r \ddot{\mathbf{r}} = -\frac{G m_1 m_2}{r^3}\,\mathbf{r}, \qquad \mu_r \equiv \frac{m_1 m_2}{m_1 + m_2},$$

donde $\mu_r$ es la *masa reducida*. El problema de los dos cuerpos se descompone así en una traslación libre del centro de masa y una única partícula efectiva de masa $\mu_r$ que orbita en torno a un centro de fuerza fijo. Para las naves espaciales, $m_1 \ll m_2 = M_\oplus$ y $\mu_r \approx m_1$; la reducción es exacta, aunque prácticamente indistinguible de la idealización de la partícula de prueba, en la cual

$$\ddot{\mathbf{r}} = -\frac{\mu}{r^3}\,\mathbf{r}, \qquad \mu \equiv G M_\oplus = 398\,600{,}4418\,\mathrm{km^3\,s^{-2}}.$$

La magnitud $\mu$, el *parámetro gravitatorio* de la Tierra, se conoce con una precisión muy superior a la de $G$ o $M_\oplus$ por separado. Este es un rasgo recurrente de la mecánica celeste: los productos de constantes fundamentales suelen ser medibles con mucha mayor precisión que las constantes mismas.

### 2.3 La metafísica del problema reducido

La reducción del problema de dos cuerpos al movimiento de una partícula ficticia en un potencial central fijo es conceptualmente radical. La «partícula de masa reducida» no es, en ningún sentido ordinario, una *cosa*. Es un constructo matemático que nos permite tratar una propiedad relacional de dos cuerpos — su vector de separación — como si fuese la posición de una única entidad. Este es un ejemplo temprano, y aún hoy poco apreciado, de cómo la estructura matemática de una teoría física puede apartarse de su ontología. Encontraremos ejemplos más dramáticos a medida que avancemos.

---

## 3. Leyes de conservación y geometría del problema de Kepler

### 3.1 Energía, momento angular y el vector de Laplace–Runge–Lenz

El problema de Kepler — movimiento en un potencial central $-\mu/r$ — admite tres integrales vectoriales del movimiento más allá de las triviales:

1. **Energía orbital específica** (un escalar):
$$\varepsilon = \tfrac{1}{2} v^2 - \frac{\mu}{r}.$$

2. **Momento angular específico** (un vector):
$$\mathbf{h} = \mathbf{r} \times \mathbf{v}.$$

3. **El vector de Laplace–Runge–Lenz** (un vector):
$$\mathbf{A} = \mathbf{v} \times \mathbf{h} - \mu\,\hat{\mathbf{r}}.$$

Que $\varepsilon$ y $\mathbf{h}$ se conserven se sigue del carácter conservativo y central de la fuerza. La conservación de $\mathbf{A}$, en cambio, es más sutil. Es una peculiaridad del potencial $1/r$ — manifestación, como veremos, de un grupo de simetría oculto mayor que el $SO(3)$ manifiesto de las fuerzas centrales.

### 3.2 La ecuación de la órbita

Multiplicando vectorialmente la ecuación del momento angular por $\mathbf{r}$ e integrando, se obtiene la órbita en forma polar:

$$r(\theta) = \frac{p}{1 + e \cos(\theta - \theta_0)}, \qquad p = \frac{h^2}{\mu}, \qquad e = \frac{|\mathbf{A}|}{\mu}.$$

Es una sección cónica. La excentricidad $e$ — o, equivalentemente, el módulo del vector de Laplace–Runge–Lenz — determina el tipo de órbita:

| $e$ | $\varepsilon$ | Cónica |
|-----|---------------|--------|
| $0$ | $-\mu/(2a)$ | Círculo |
| $0 < e < 1$ | $-\mu/(2a)$ | Elipse |
| $1$ | $0$ | Parábola |
| $> 1$ | $+\mu/(2a)$ | Hipérbola |

La dirección de $\mathbf{A}$ apunta del centro de atracción hacia el periapsis. Codifica, por tanto, la *orientación* de la órbita en su plano — un dato que, geométricamente hablando, es una integral del movimiento adicional más allá de la energía y del momento angular. Es esta integral «extra» la responsable de la naturaleza cerrada de las órbitas keplerianas ligadas.

### 3.3 La simetría oculta $SO(4)$

Un problema de fuerza central posee genéricamente sólo la simetría rotacional $SO(3)$, que da lugar a tres cantidades conservadas (las componentes de $\mathbf{h}$). Que el problema de Kepler tenga una cuarta integral vectorial — el vector de Laplace–Runge–Lenz — señala la presencia de un grupo de simetría mayor. El cálculo explícito revela que, para órbitas ligadas ($\varepsilon < 0$), los corchetes de Poisson de las componentes de $\mathbf{h}$ y del vector reescalado $\mathbf{A}/\sqrt{-2\varepsilon}$ cierran para formar el álgebra de Lie $\mathfrak{so}(4)$ — el álgebra de rotaciones infinitesimales en cuatro dimensiones.

El espacio fásico orbital del problema de Kepler ligado está, por tanto, sometido a la acción de $SO(4)$, y no meramente a la de $SO(3)$. Este es el **teorema de Fock (1935)**, y posee una notable contraparte física: la degeneración de los niveles de energía del átomo de hidrógeno no relativista, en la que estados de distinto momento angular comparten la misma energía, es la sombra cuántica de esta simetría clásica $SO(4)$.

Que la mecánica celeste y la física atómica compartan un grupo de simetría es un hecho profundo. No es coincidencia, sino consecuencia de su núcleo matemático común: ambos son problemas con potencial $1/r$. El universo, a ambas escalas, ha escogido el mismo potencial.

---

## 4. La formulación lagrangiana

### 4.1 El principio de acción estacionaria

La formulación newtoniana es vectorial: describe el movimiento en términos de fuerzas aplicadas a partículas. La formulación lagrangiana es variacional: describe el movimiento en términos de un criterio global, la *acción*,

$$S[\gamma] = \int_{t_1}^{t_2} L(q,\dot q,t)\, dt,$$

definida sobre caminos $\gamma: [t_1, t_2] \to \mathcal{Q}$ en la variedad de configuración $\mathcal{Q}$. La trayectoria física es aquella a lo largo de la cual $S$ es estacionaria bajo variaciones que se anulan en los extremos:

$$\delta S = 0 \quad \Longleftrightarrow \quad \frac{d}{dt}\frac{\partial L}{\partial \dot q^i} - \frac{\partial L}{\partial q^i} = 0.$$

Son las **ecuaciones de Euler–Lagrange**. Para el movimiento orbital, el lagrangiano es

$$L = T - U = \tfrac{1}{2}|\dot{\mathbf{r}}|^2 + \frac{\mu}{r}.$$

En coordenadas polares $(r, \theta)$, con $\dot{\mathbf{r}} \cdot \dot{\mathbf{r}} = \dot r^2 + r^2 \dot\theta^2$,

$$L(r,\theta,\dot r, \dot\theta) = \tfrac{1}{2}(\dot r^2 + r^2 \dot\theta^2) + \frac{\mu}{r}.$$

### 4.2 Coordenadas cíclicas y conservación

La coordenada $\theta$ no aparece explícitamente en $L$: es *cíclica*. El momento conjugado correspondiente

$$p_\theta = \frac{\partial L}{\partial \dot\theta} = r^2 \dot\theta$$

se conserva, por tanto. Esta cantidad es el momento angular específico $h$. La conservación del momento angular no es, bajo este análisis, un hecho físico independiente, sino consecuencia de la simetría rotacional del problema — una anticipación del teorema de Noether.

### 4.3 El potencial efectivo

Sustituyendo la ley de conservación $\dot\theta = h/r^2$ en la energía, se obtiene

$$\varepsilon = \tfrac{1}{2}\dot r^2 + \underbrace{\frac{h^2}{2 r^2} - \frac{\mu}{r}}_{V_{\text{ef}}(r)}.$$

El movimiento radial es el de una partícula unidimensional en el potencial efectivo $V_{\text{ef}}$. Este potencial presenta una barrera centrífuga $h^2/(2r^2)$ para $r$ pequeño y un pozo newtoniano $-\mu/r$ para $r$ grande. Su mínimo,

$$r_* = \frac{h^2}{\mu}, \qquad V_{\text{ef}}(r_*) = -\frac{\mu^2}{2 h^2},$$

corresponde al movimiento circular. Las órbitas ligadas oscilan radialmente entre los puntos de retroceso $r_p$ y $r_a$ en los que $\dot r = 0$; las órbitas no ligadas tienen un único perihelio y se alejan hasta el infinito.

El potencial efectivo no es una nueva fuerza física. Es una *ficción cinemática* que permite analizar el movimiento orbital bidimensional como un problema unidimensional — una vez más, un ejemplo de la subordinación de la ontología de la mecánica a su economía matemática.

### 4.4 El teorema de Noether

El teorema de Emmy Noether de 1918 afirma que toda simetría continua de la acción induce una cantidad conservada. Sus instancias específicas en mecánica celeste son:

| Simetría | Cantidad conservada |
|----------|---------------------|
| Traslación temporal | Energía $\varepsilon$ |
| Rotación (en torno a cualquier eje) | Componente del momento angular $h_i$ |
| Simetría oculta $SO(4)$ (sólo Kepler) | Vector de Laplace–Runge–Lenz $\mathbf{A}$ |

El teorema de Noether es una afirmación estructural: las leyes de conservación de la física no son hallazgos experimentales contingentes, sino consecuencias matemáticas del grupo de simetría de la acción. Entender la mecánica celeste es, a esta luz, entender sus simetrías.

---

## 5. La formulación hamiltoniana y el espacio fásico

### 5.1 La transformación de Legendre

El paso del lagrangiano $L(q,\dot q, t)$ al hamiltoniano $H(q,p,t)$ es una **transformación de Legendre** en las velocidades:

$$p_i = \frac{\partial L}{\partial \dot q^i}, \qquad H(q,p,t) = p_i \dot q^i - L(q,\dot q,t),$$

con $\dot q^i$ eliminada en favor de $p_i$. Para el movimiento orbital en coordenadas cartesianas,

$$H = \frac{|\mathbf{p}|^2}{2} - \frac{\mu}{r}.$$

El hamiltoniano es, genéricamente, la energía total; esta identificación requiere que el lagrangiano sea independiente del tiempo y que la energía cinética sea una forma cuadrática homogénea en las velocidades, condiciones ambas que se cumplen aquí.

### 5.2 Las ecuaciones canónicas

Las ecuaciones de Euler–Lagrange se convierten en las **ecuaciones canónicas** de Hamilton:

$$\dot q^i = \frac{\partial H}{\partial p_i}, \qquad \dot p_i = -\frac{\partial H}{\partial q^i}.$$

Son $2n$ ecuaciones de primer orden sobre el fibrado cotangente $T^*\mathcal{Q}$ — el *espacio fásico* — en lugar de $n$ ecuaciones de segundo orden sobre $\mathcal{Q}$. Esta duplicación de dimensión se compensa con un aumento notable de estructura geométrica, que pasamos a describir.

### 5.3 Geometría simpléctica del espacio fásico

El espacio fásico $M = T^*\mathcal{Q}$ está dotado de una dos-forma canónica

$$\omega = dp_i \wedge dq^i,$$

la **forma simpléctica**. Es cerrada ($d\omega = 0$) y no degenerada. El par $(M, \omega)$ es una *variedad simpléctica*, y las ecuaciones de Hamilton, en notación libre de coordenadas, rezan

$$\iota_{X_H}\,\omega = dH,$$

donde $X_H$ es el campo vectorial hamiltoniano asociado a $H$. El flujo $\phi^t$ generado por $X_H$ preserva $\omega$: para todo $t$,

$$(\phi^t)^* \omega = \omega.$$

Esta es la propiedad definitoria de un flujo hamiltoniano y la fuente de todo cuanto resulta distintivo de la mecánica hamiltoniana.

### 5.4 Corchetes de Poisson

Dadas dos funciones $f, g: M \to \mathbb{R}$, su **corchete de Poisson** es

$$\{f,g\} = \sum_i \left(\frac{\partial f}{\partial q^i}\frac{\partial g}{\partial p_i} - \frac{\partial f}{\partial p_i}\frac{\partial g}{\partial q^i}\right).$$

La evolución de cualquier observable $f$ a lo largo del flujo hamiltoniano es

$$\frac{df}{dt} = \{f, H\} + \frac{\partial f}{\partial t}.$$

Un observable es una constante del movimiento precisamente cuando $\{f, H\} = 0$ (para $f$ explícitamente independiente del tiempo). El álgebra de corchetes de Poisson de las cantidades conservadas es la expresión infinitesimal del grupo de simetría del sistema.

Para el problema de Kepler, los corchetes de Poisson de las componentes de $\mathbf{h}$ y $\mathbf{A}$ cierran sobre el álgebra de Lie $\mathfrak{so}(4)$ para órbitas ligadas, sobre $\mathfrak{so}(3,1)$ para las no ligadas y sobre el álgebra euclidiana $\mathfrak{e}(3)$ en el caso parabólico — tres tipos distintos de simetría para los tres regímenes cónicos.

### 5.5 El teorema de Liouville

El flujo hamiltoniano preserva no sólo $\omega$, sino su potencia exterior máxima $\omega^n / n!$, la **forma de volumen de Liouville**. Así pues:

> **Teorema de Liouville.** *El volumen en el espacio fásico de toda región medible es invariante bajo la evolución hamiltoniana.*

El flujo en el espacio fásico es *incompresible*. Ningún atractor, ninguna cuenca de contracción, puede existir en un sistema hamiltoniano. Este solo hecho separa la dinámica conservativa de la dinámica disipativa en el nivel más profundo, y gobierna cuanto va de la mecánica estadística a la estabilidad a largo plazo de las órbitas planetarias.

---

## 6. Variables de acción–ángulo e integrabilidad

### 6.1 La ecuación de Hamilton–Jacobi

Para un hamiltoniano independiente del tiempo, las ecuaciones de Hamilton pueden reducirse a una única ecuación en derivadas parciales de primer orden, la **ecuación de Hamilton–Jacobi**:

$$H\!\left(q^i, \frac{\partial W}{\partial q^i}\right) = E,$$

donde $W(q, \alpha)$ — la *función característica de Hamilton* — genera una transformación canónica a variables en las que el nuevo hamiltoniano es trivial. Cuando $W$ se separa aditivamente en algún sistema de coordenadas, $W = \sum_i W_i(q^i)$, se dice que el sistema es *separable*, y la ecuación de Hamilton–Jacobi se reduce a un conjunto de ecuaciones diferenciales ordinarias.

El problema de Kepler es separable en coordenadas esféricas (y, notablemente, también en coordenadas parabólicas — otro eco de su simetría ampliada).

### 6.2 Integrabilidad de Liouville–Arnold

Un sistema hamiltoniano con $n$ grados de libertad es *integrable en el sentido de Liouville* si posee $n$ cantidades conservadas independientes $F_1 = H, F_2, \ldots, F_n$ que están por pares en involución:

$$\{F_i, F_j\} = 0 \quad \text{para todos } i,j.$$

El **teorema de Liouville–Arnold** afirma que, bajo estas condiciones y supuestos de compacidad adecuados, los conjuntos de nivel conjuntos de las $F_i$ son difeomorfos a toros de dimensión $n$ $\mathbb{T}^n$, y existen coordenadas $(\mathbf{I}, \boldsymbol{\varphi})$ — las **variables de acción–ángulo** — en las que

$$H = H(\mathbf{I}), \qquad \dot{I}_i = 0, \qquad \dot{\varphi}_i = \omega_i(\mathbf{I}) \equiv \frac{\partial H}{\partial I_i}.$$

El movimiento es un flujo lineal sobre un toro: cuasi-periódico con frecuencias $\omega_i$.

### 6.3 El problema de Kepler en variables de Delaunay

Para el problema de Kepler, una elección canónica de variables de acción–ángulo es la de **Delaunay**:

| Acción | Ángulo conjugado | Interpretación |
|--------|------------------|----------------|
| $L = \sqrt{\mu a}$ | $\ell$ (anomalía media) | Tamaño orbital |
| $G = L\sqrt{1 - e^2}$ | $g = \omega$ (argumento del periapsis) | Momento angular |
| $H = G\cos i$ | $h = \Omega$ (longitud del nodo ascendente) | Componente de $\mathbf{h}$ a lo largo de un eje fijo |

El hamiltoniano, expresado en estas variables, depende únicamente de $L$:

$$H = -\frac{\mu^2}{2 L^2}.$$

Dado que $H$ es independiente de $G$ y $H$, las frecuencias $\omega_G = \omega_H = 0$: dos de los tres ángulos son constantes del movimiento. Sólo $\ell$ avanza linealmente en el tiempo. Esta es la formulación dinámica del carácter cerrado de las órbitas keplerianas. El toro fásico tridimensional completo colapsa en una curva cerrada unidimensional — de nuevo, consecuencia de la simetría oculta $SO(4)$.

### 6.4 La integrabilidad como fragilidad estructural

La clase de los sistemas hamiltonianos integrables es extraordinariamente pequeña. Las perturbaciones genéricas de un sistema integrable *no* son integrables. La integrabilidad es una propiedad estructuralmente frágil — una condición de medida nula en cualquier sentido razonable — y su prevalencia en los ejemplos que pueblan los libros de texto refleja la selección histórica de problemas tratables, más que el comportamiento genérico de los sistemas dinámicos. El problema de Kepler es excepcional, no típico.

Esta constatación — que la mayoría de los sistemas hamiltonianos son no integrables y exhiben comportamiento caótico — es la revolución conceptual operada por Poincaré a finales del siglo XIX. Es el objeto de las dos secciones siguientes.

---

## 7. Teoría de perturbaciones y el colapso de la integrabilidad

### 7.1 El hamiltoniano perturbado

Un sistema casi kepleriano posee un hamiltoniano de la forma

$$H(\mathbf{I}, \boldsymbol{\varphi}; \varepsilon) = H_0(\mathbf{I}) + \varepsilon H_1(\mathbf{I}, \boldsymbol{\varphi}),$$

donde $H_0$ es integrable (el hamiltoniano kepleriano puro), $H_1$ es una perturbación pequeña (achatamiento terrestre, arrastre atmosférico, gravedad lunisolar, correcciones relativistas), y $\varepsilon \ll 1$ es un parámetro adimensional de contabilidad. La teoría clásica de perturbaciones busca una transformación canónica $(\mathbf{I}, \boldsymbol{\varphi}) \to (\mathbf{I}', \boldsymbol{\varphi}')$ bajo la cual, orden a orden en $\varepsilon$, el nuevo hamiltoniano dependa sólo de las nuevas acciones:

$$H'(\mathbf{I}') = H_0(\mathbf{I}') + \varepsilon \langle H_1 \rangle(\mathbf{I}') + \varepsilon^2 H_2'(\mathbf{I}') + \cdots,$$

donde $\langle \cdot \rangle$ denota el promedio sobre los ángulos no perturbados.

### 7.2 Divisores pequeños y la serie de Lindstedt

La función generatriz de la transformación canónica, desarrollada en modos de Fourier de los ángulos no perturbados, contiene denominadores de la forma

$$\mathbf{k} \cdot \boldsymbol{\omega}_0(\mathbf{I}), \qquad \mathbf{k} \in \mathbb{Z}^n \setminus \{0\}.$$

Estos denominadores, los **divisores pequeños**, se anulan siempre que las frecuencias no perturbadas $\boldsymbol{\omega}_0$ son *conmensurables* — esto es, satisfacen una condición de resonancia $\mathbf{k} \cdot \boldsymbol{\omega}_0 = 0$ para algún vector entero no nulo $\mathbf{k}$. Para frecuencias arbitrariamente próximas a la resonancia, los divisores se hacen arbitrariamente pequeños, y la serie perturbativa no tiene por qué converger.

Esta observación, debida a Poincaré, socavó el programa del siglo XIX de demostrar la estabilidad del sistema solar mediante métodos de series de potencias. Poincaré demostró, más enérgicamente, que *en general las series perturbativas para un sistema no integrable no convergen*, aun cuando cada término individual sea finito.

### 7.3 Perturbaciones seculares y el método del promedio

Aunque la serie perturbativa completa pueda no converger, las ecuaciones promediadas de primer orden

$$\dot{\mathbf{I}}' = 0, \qquad \dot{\boldsymbol{\varphi}}' = \boldsymbol{\omega}_0(\mathbf{I}') + \varepsilon\,\frac{\partial \langle H_1 \rangle}{\partial \mathbf{I}'}$$

proporcionan una imagen cualitativa valiosa. Las acciones se conservan a primer orden; sólo los ángulos derivan. Expresado en elementos orbitales, esto es la **teoría secular de Laplace–Lagrange**, que predice la lenta precesión de las longitudes del nodo ascendente y de los argumentos del periapsis, dejando $a$ y $e$ invariantes a primer orden.

Para satélites terrestres perturbados por $J_2$, las tasas seculares

$$\dot\Omega = -\frac{3}{2}\, n J_2 \left(\frac{R_\oplus}{p}\right)^{\!2}\cos i,\qquad
\dot\omega = \frac{3}{4}\, n J_2 \left(\frac{R_\oplus}{p}\right)^{\!2}(5\cos^2 i - 1)$$

dan, por ejemplo, la inclinación $i_{\text{helsínc}} \approx 97°\text{–}99°$ para la que $\dot\Omega$ iguala el movimiento medio de la Tierra en torno al Sol — el régimen *heliosincrónico*. La inclinación crítica $\cos^2 i_c = 1/5$ (es decir, $i_c \approx 63{,}4°$) en la que $\dot\omega = 0$ es la inclinación Molniya — un artefacto de simetría matemática aprovechado operacionalmente para comunicaciones de altas latitudes.

### 7.4 El teorema de Poincaré sobre la inexistencia de integrales

El resultado negativo culminante de la teoría clásica de perturbaciones es el teorema de Poincaré (1892):

> *Una perturbación analítica genérica de un hamiltoniano analítico integrable no admite ninguna integral analítica adicional del movimiento, más allá del propio hamiltoniano.*

Las integrales, cuando existen, son excepcionales. Para el problema de los $n$ cuerpos con $n \geq 3$, las diez integrales clásicas (energía, momento lineal, momento angular, movimiento del centro de masa) agotan las cantidades analíticas conservadas, y el movimiento es genéricamente no integrable. El problema de los tres cuerpos, al que Poincaré dedicó su memoria premiada de 1890, se convirtió en el ejemplo paradigmático de un sistema bastante simple de enunciar y bastante intratable como para resistir un análisis completo.

---

## 8. Caos, teoría KAM y el problema de los tres cuerpos

### 8.1 Dependencia sensible de las condiciones iniciales

Un sistema dinámico exhibe **dependencia sensible de las condiciones iniciales** cuando trayectorias próximas divergen exponencialmente en el tiempo:

$$|\delta \mathbf{z}(t)| \sim |\delta \mathbf{z}(0)|\, e^{\lambda t}, \qquad \lambda > 0,$$

donde $\lambda$ es el mayor exponente de Lyapunov. La dependencia sensible es el núcleo técnico de lo que popularmente se denomina *caos*. No contradice el determinismo: la evolución está enteramente fijada por la condición inicial. Lo que limita es la predictibilidad: el conocimiento de precisión finita de la condición inicial se degrada, a la tasa de Lyapunov, hasta la vacuidad.

El problema de los tres cuerpos exhibe dependencia sensible en amplias regiones de su espacio fásico. Una incertidumbre de un metro en la posición inicial de un cometa de la familia de Júpiter crece, en $10^5$ años, hasta una incertidumbre de posición comparable al propio radio orbital. La predicción, en cualquier sentido estricto, se vuelve imposible. El universo es determinista; no es, en general, predecible.

### 8.2 Secciones de Poincaré y la maraña homoclínica

Para visualizar el flujo de un sistema hamiltoniano con dos grados de libertad, se fija una superficie de energía (tridimensional) y se cortan las trayectorias con una superficie transversal (bidimensional). El mapa discreto resultante es una **sección de Poincaré**. Para sistemas integrables, la sección consiste en curvas cerradas — las trazas de los toros invariantes. Para sistemas no integrables, emerge una imagen mucho más rica: algunos toros persisten, otros se descomponen en cadenas de islas, y, en el entorno de órbitas periódicas hiperbólicas, las variedades estable e inestable se intersecan *transversalmente*, creando la estructura infinitamente autointersecante que Poincaré llamó la **maraña homoclínica**.

De esta estructura escribió Poincaré:

> *Uno queda sobrecogido ante la complejidad de esta figura, que ni siquiera intentaré dibujar. Nada resulta más idóneo para darnos una idea de la complejidad del problema de los tres cuerpos, y en general de todos los problemas de dinámica en los que no existe integral uniforme.*
> — *Méthodes Nouvelles de la Mécanique Céleste*, Vol. III, §397 (1899)

La maraña homoclínica es el primer objeto matemático en la historia de la física que *se resistió a ser dibujado*. Es el sustrato geométrico del caos.

### 8.3 El teorema KAM

Pese a la no integrabilidad genérica de los sistemas hamiltonianos perturbados, no toda estructura queda destruida por la perturbación. El **teorema de Kolmogórov–Arnold–Moser** (Kolmogórov 1954, Arnold 1963, Moser 1962) afirma, bajo hipótesis adecuadas de no degeneración y suavidad:

> *Para perturbaciones suficientemente pequeñas de un hamiltoniano integrable no degenerado, sobrevive un conjunto de toros invariantes de medida de Lebesgue positiva, en los que el flujo perturbado es conjugado a un flujo lineal con vector de frecuencias diofántico.*

Más precisamente: los toros cuyas frecuencias $\boldsymbol{\omega}$ satisfacen una condición diofántica

$$|\mathbf{k} \cdot \boldsymbol{\omega}| \geq \frac{\gamma}{|\mathbf{k}|^\tau}, \qquad \mathbf{k} \in \mathbb{Z}^n \setminus \{0\}$$

para ciertos $\gamma, \tau > 0$, persisten bajo perturbaciones cuyo tamaño esté acotado por una función de $\gamma, \tau$ y de la regularidad del sistema. La medida de los toros que no persisten se anula como $\sqrt{\varepsilon}$ cuando $\varepsilon \to 0$.

El teorema KAM restituye una forma de estabilidad en el seno del caos. El espacio fásico de un sistema integrable perturbado es un entrelazamiento de tipo Cantor de toros supervivientes (sobre los que el movimiento es cuasi-periódico) y zonas de resonancia (en las que el movimiento es caótico). Ninguna de las dos estructuras por sí sola describe el sistema; su coexistencia es su carácter esencial.

### 8.4 La difusión de Arnold y la estabilidad de Nekhoróshev

Para $n \geq 3$ grados de libertad, los toros KAM supervivientes no separan el espacio fásico en regiones disjuntas; en principio, las trayectorias entre los toros pueden deambular globalmente por la superficie de energía. Este fenómeno es la **difusión de Arnold**, demostrada por Arnold en 1964 para un ejemplo específico y que se presume genérica. Sus escalas temporales en sistemas realistas son, no obstante, astronómicamente largas.

El **teorema de Nekhoróshev** (1977) complementa la teoría KAM con un enunciado cuantitativo válido para *cualquier* condición inicial (no sólo para las situadas sobre toros supervivientes): para perturbaciones analíticas de un hamiltoniano integrable pronunciado, las acciones son constantes salvo una pequeña desviación,

$$|\mathbf{I}(t) - \mathbf{I}(0)| \leq C \varepsilon^{b}, \qquad |t| \leq T_* \equiv T_0 \exp\!\left(\varepsilon_0/\varepsilon\right)^{a},$$

para exponentes $a, b > 0$ dependientes del número de grados de libertad. La inestabilidad es posible, pero exponencialmente lenta. El sistema solar, cuyo tiempo de Lyapunov es $\sim 10^7$ años y cuya vida esperada frente a la expulsión planetaria es $\sim 10^{10}$ años, vive dentro del régimen de Nekhoróshev: localmente caótico, globalmente estable en escalas cosmológicas.

### 8.5 El residuo filosófico

El descubrimiento del caos determinista disolvió una presuposición metafísica que, antes de Poincaré, era casi invisible: que el determinismo implica la predictibilidad. El demonio de Laplace, capaz de deducir del estado presente del universo su pasado y su futuro enteros, era imaginado computando; el descubrimiento del caos revela que, aun concediendo conocimiento perfecto de las leyes, una precisión computacional finita basta para derrotar al demonio más allá del horizonte de Lyapunov. El determinismo es metafísico; la predictibilidad es epistemológica; desde Poincaré, no son ya la misma cuestión.

---

## 9. Mecánica geométrica y reducción por simetría

### 9.1 La variedad de configuración

La mecánica clásica adopta su forma geométrica más plena cuando el espacio de configuración no se supone un espacio vectorial euclidiano, sino una variedad lisa $\mathcal{Q}$. Para una única partícula en tres dimensiones, $\mathcal{Q} = \mathbb{R}^3$; para una partícula confinada a una esfera, $\mathcal{Q} = S^2$; para un cuerpo rígido, $\mathcal{Q} = SO(3)$; para el problema de Kepler restringido a órbitas ligadas, la variedad de configuración es $\mathbb{R}^3 \setminus \{0\}$.

El lagrangiano es una función $L: T\mathcal{Q} \to \mathbb{R}$ sobre el *fibrado tangente*; el hamiltoniano es una función $H: T^*\mathcal{Q} \to \mathbb{R}$ sobre el *fibrado cotangente*. La transformación de Legendre es una aplicación entre ambos. Nada en esta formulación privilegia sistema alguno de coordenadas; la geometría es intrínseca.

### 9.2 Aplicaciones momento y reducción por simetría

Sea un grupo de Lie $G$ que actúa sobre $\mathcal{Q}$ por difeomorfismos. La acción elevada sobre $T^*\mathcal{Q}$ preserva la forma simpléctica, y, mediante una construcción debida a Souriau y Kostant, existe una **aplicación momento** $\mathbf{J}: T^*\mathcal{Q} \to \mathfrak{g}^*$ hacia el dual del álgebra de Lie, cuyas componentes se conservan a lo largo del flujo de cualquier hamiltoniano $G$-invariante.

Para las rotaciones espaciales $G = SO(3)$, la aplicación momento es el familiar vector momento angular. El teorema de Noether es la versión infinitesimal de esta construcción.

La **reducción de Marsden–Weinstein** (1974) utiliza la aplicación momento para eliminar la simetría. Fijando el momento en un valor $\mu \in \mathfrak{g}^*$ y pasando al cociente por el subgrupo de isotropía, se obtiene un *espacio fásico reducido*

$$M_\mu = \mathbf{J}^{-1}(\mu) / G_\mu,$$

él mismo simpléctico, cuya dimensión es $\dim M - \dim G - \dim G_\mu$. Para el problema de Kepler, la reducción por rotaciones produce un espacio fásico reducido bidimensional en el que tiene lugar el movimiento radial: el análisis del potencial efectivo de §4.3 es la expresión en coordenadas de esta reducción.

### 9.3 La fase geométrica

Una trayectoria orbital que retorna a su punto inicial en el espacio reducido no tiene por qué retornar a su punto inicial en el espacio completo. El desplazamiento residual — la **fase geométrica** o **ángulo de Hannay** — es una cantidad puramente geométrica, independiente de la velocidad a la que se recorre el circuito. Para ciclos adiabáticos de un sistema hamiltoniano, el ángulo de Hannay es el análogo clásico de la fase geométrica cuántica de Berry.

En la mecánica celeste, la fase geométrica se manifiesta, por ejemplo, en la lenta rotación del plano orbital de un satélite a medida que la Tierra recorre su trayecto anual, incluso en ausencia de torque gravitatorio solar directo. Es un recordatorio de que *la composición de movimientos reversibles no es, en general, reversible* — una fuente estructural, más que disipativa, de irreversibilidad.

---

## 10. Correcciones relativistas generales

### 10.1 De la fuerza a la geometría

En relatividad general, la gravitación no es una fuerza. Un cuerpo en caída libre sigue una *geodésica* de una variedad lorentziana curva, el espacio-tiempo $(M, g)$, cuya métrica $g_{\mu\nu}$ se determina por la distribución de materia y energía a través de las ecuaciones de campo de Einstein,

$$R_{\mu\nu} - \tfrac{1}{2} R\, g_{\mu\nu} + \Lambda\, g_{\mu\nu} = \frac{8\pi G}{c^4}\, T_{\mu\nu}.$$

La imagen newtoniana — cuerpos moviéndose a través de un espacio plano bajo la influencia de una fuerza instantánea — se recupera como el límite de campo débil y movimiento lento de esta teoría. Las órbitas no son las trayectorias excavadas por una atracción misteriosa; son las líneas más rectas que pueden trazarse en una geometría que está, ella misma, curvada por la masa.

### 10.2 La métrica de Schwarzschild y sus geodésicas

Fuera de un cuerpo estático y esféricamente simétrico de masa $M$, la métrica del espacio-tiempo es la **solución de Schwarzschild**:

$$ds^2 = -\left(1 - \frac{r_s}{r}\right)c^2\,dt^2 + \left(1 - \frac{r_s}{r}\right)^{-1}dr^2 + r^2(d\theta^2 + \sin^2\theta\, d\phi^2),$$

donde $r_s = 2GM/c^2$ es el **radio de Schwarzschild** — $\sim 3$ km para el Sol, $\sim 9$ mm para la Tierra.

Las geodésicas en esta métrica se apartan de las órbitas keplerianas en correcciones de orden $r_s/r$. Al orden post-newtoniano dominante, el observable principal es la precesión anómala del periapsis:

$$\Delta\omega_{\text{RG}} = \frac{6\pi G M}{c^2\, a(1 - e^2)} \quad \text{por órbita}.$$

Para Mercurio, esto rinde $\sim 43$ segundos de arco por siglo — el célebre residuo que la teoría clásica de perturbaciones no podía explicar, y cuya explicación por Einstein en 1915 constituyó el primer triunfo empírico de la relatividad general.

### 10.3 La inversión conceptual

La reinterpretación relativista general de la gravedad no es una corrección cuantitativa de una teoría preexistente, sino una reconceptualización radical. Ni la fuerza ni el potencial aparecen en las ecuaciones fundamentales; sus papeles son asumidos por las componentes del tensor métrico. Lo que habíamos pensado previamente como la *causa* del movimiento orbital — la fuerza gravitatoria — ha sido reemplazado por su *lugar* — la geometría del espacio-tiempo. La órbita siempre fue geometría; Newton se había limitado a traducir esa geometría al vocabulario de su tiempo.

Esta es la lección más profunda del arco que va de Kepler a Einstein: que la estructura matemática de una teoría física no es una formalización de una intuición física preexistente, sino una fuente de intuición por derecho propio, que la historia del tema descubre gradualmente.

---

## 11. La discretización como acto epistemológico

### 11.1 La brecha entre continuo y computación

La órbita, en tanto objeto matemático, es una curva lisa — solución de una ecuación diferencial en tiempo continuo. Toda simulación computacional, por refinada que sea, es una secuencia de muestras discretas $(\mathbf{r}_n, \mathbf{v}_n)$ producidas por un integrador que aproxima el flujo sobre pasos temporales finitos. Surge así la pregunta: ¿en qué sentido, y bajo qué condiciones, representa una secuencia discreta de estados computados a una trayectoria orbital continua?

### 11.2 Integradores simplécticos

Un integrador numérico genérico aplicado a las ecuaciones de Hamilton produce un mapa discreto $\Phi_h: M \to M$ que aproxima el flujo verdadero $\phi^h$. Para la mayoría de los integradores, incluidos los métodos clásicos de Runge–Kutta, $\Phi_h$ *no* preserva la forma simpléctica $\omega$: el flujo numérico posee una disipación o excitación artificial que se manifiesta, en mecánica celeste, como una deriva secular espuria en la energía o en el momento angular.

Un **integrador simpléctico** es una discretización $\Phi_h$ que preserva $\omega$ exactamente:

$$\Phi_h^* \omega = \omega.$$

Tales integradores — leapfrog, Störmer–Verlet, variantes de orden superior debidas a Yoshida — poseen un **hamiltoniano modificado** $\tilde H = H + h^p H_1 + h^{p+1} H_2 + \cdots$ que conservan exactamente (en el sentido de una serie formal). Sobre escalas temporales astronómicamente largas, los integradores simplécticos preservan la estructura cualitativa del flujo — la existencia de toros invariantes, la forma de las secciones de Poincaré — de un modo en que los métodos no simplécticos no lo hacen. No producen la *verdadera* trayectoria, pero producen el verdadero *tipo* de trayectoria.

### 11.3 Sombreado (*shadowing*)

Un sentido más profundo en el que una trayectoria numérica representa a una verdadera es la **propiedad de sombreado** (*shadowing*). Se dice que una secuencia discreta $(\mathbf{z}_n)$ es *sombreada* por una órbita verdadera $\gamma(t)$ si, para todo $n$,

$$|\mathbf{z}_n - \gamma(n h)| < \delta$$

para algún $\delta$ pequeño. Los teoremas de sombreado — para sistemas hiperbólicos (Anósov, 1967; Bowen, 1975) y, en formas más débiles, para sistemas dinámicos más generales — garantizan que toda trayectoria numérica suficientemente precisa se halla próxima a *alguna* trayectoria verdadera del sistema, aun cuando no siga de cerca aquella que emana de la condición inicial nominal.

Es un enunciado epistemológicamente notable. La trayectoria numérica no tiene por qué ser el futuro verdadero del sistema; sólo ha de ser *un* futuro verdadero de *algún* sistema próximo. En un régimen caótico, esto suele ser lo mejor que cabe decir — y es, en un sentido preciso, suficiente.

### 11.4 ¿Qué es la órbita de una simulación?

Podemos retornar ahora a la pregunta planteada al comienzo de esta sección. La órbita de un cuerpo simulado no es ni una trayectoria en el espacio físico (no es física), ni una solución de una ecuación diferencial (no se resolvió ecuación diferencial alguna). Es una secuencia discreta de números de coma flotante relacionada con la órbita matemática continua mediante una cadena de aproximaciones: el modelo físico es, él mismo, aproximado; el integrador es una aproximación del modelo; la aritmética de coma flotante es una aproximación del integrador.

Que esta cadena de aproximaciones produzca, con todo, predicciones fiables del comportamiento de las naves reales no es evidente por sí mismo. Se hace posible gracias a tres hechos conspirantes: la suavidad de las ecuaciones diferenciales en cuestión, la existencia de integradores simplécticos que preservan los invariantes estructurales, y la propiedad de sombreado que suministra una órbita verdadera en el entorno de la computada. El éxito de la computación en mecánica celeste es, en último análisis, un teorema de la teoría de sistemas dinámicos.

---

## 12. Meditaciones finales

### 12.1 La órbita como objeto matemático y físico

Comenzamos con tres ontologías candidatas para la órbita: trayectoria, curva integral, hoja simpléctica. El curso de este tratado ha sido el argumento de que cada una es correcta en su nivel de descripción apropiado, y que el progreso de la mecánica celeste puede leerse como un ascenso gradual por estos niveles. La órbita, como fenómeno físico, es la misma en cada siglo; la órbita, como objeto matemático, ha cambiado tantas veces como nuestra propia comprensión de las matemáticas.

### 12.2 La jerarquía de reformulaciones

Cada reformulación que hemos encontrado — de lagrangiana a hamiltoniana, de simpléctica a geométrica, de newtoniana a relativista — posee dos aspectos simultáneos. Técnicamente, revela estructura invisible al nivel anterior: la forma simpléctica, la aplicación momento, el tensor métrico. Filosóficamente, disuelve un concepto que previamente parecía indispensable: la noción de fuerza, la distinción entre configuración y fase, la separación entre gravedad y geometría. Cada generación de la mecánica disuelve la ontología de su predecesora en una unidad matemática más profunda.

Es razonable suponer que la presente formulación será, a su vez, disuelta por lo que haya de venir.

### 12.3 El problema de dos cuerpos como unidad en un mundo plural

El potencial $1/r$ aparece en la mecánica celeste y en la física atómica, en el problema clásico de Kepler y en el átomo cuántico de hidrógeno, con el mismo grupo de simetría $SO(4)$ en ambos. Un cosmos que permite esta coincidencia es un cosmos estructurado por la necesidad matemática tanto como por la contingencia física. El problema de los dos cuerpos no es meramente una aproximación útil; es un lugar en el que la unidad de la estructura matemática se afirma a través de dominios, por lo demás, separados de la física.

### 12.4 Sobre los límites de la disciplina

La mecánica celeste, como teoría clásica, es completa. Sus axiomas — la gravedad newtoniana, o su generalización relativista general; el formalismo hamiltoniano; la geometría de las variedades simplécticas — están fijados. Lo que resta no es la revisión fundacional, sino la exploración continuada: la catalogación de soluciones especiales (órbitas periódicas, puntos de libración, conexiones homoclínicas), la caracterización de regímenes caóticos, la aplicación de nuevas técnicas matemáticas a viejos problemas.

Y, sin embargo, la disciplina continúa produciendo sorpresas. El descubrimiento, en 1993, de la órbita en forma de ocho del problema de tres cuerpos (Moore; Chenciner y Montgomery); la identificación, en las dos últimas décadas, de trayectorias de transferencia de baja energía que aprovechan las variedades invariantes de las órbitas en torno a los puntos de libración; el estudio en curso de la difusión de Arnold en modelos realistas del sistema solar — cada uno de ellos demuestra que una teoría puede estar fundacionalmente completa y ser matemáticamente inagotable a la vez.

Es en esta condición paradójica — completa en sus fundamentos, ilimitada en sus consecuencias — donde la mecánica celeste imparte una lección que se generaliza mucho más allá de sí misma.

---

## 13. Referencias y bibliografía complementaria

**Mecánica clásica y métodos matemáticos**

- Arnold, V. I. *Mathematical Methods of Classical Mechanics* (2ª ed.). Springer, 1989.
- Abraham, R. y Marsden, J. E. *Foundations of Mechanics* (2ª ed.). Benjamin/Cummings, 1978.
- Marsden, J. E. y Ratiu, T. S. *Introduction to Mechanics and Symmetry* (2ª ed.). Springer, 1999.
- Goldstein, H., Poole, C. y Safko, J. *Classical Mechanics* (3ª ed.). Addison-Wesley, 2001.
- Landau, L. D. y Lifshitz, E. M. *Mecánica* (3ª ed.). Pergamon / Reverté, 1976.

**Mecánica celeste**

- Poincaré, H. *Les Méthodes Nouvelles de la Mécanique Céleste*, vols. I–III. Gauthier-Villars, 1892–1899.
- Brouwer, D. y Clemence, G. M. *Methods of Celestial Mechanics*. Academic Press, 1961.
- Morbidelli, A. *Modern Celestial Mechanics: Aspects of Solar System Dynamics*. Taylor & Francis, 2002.
- Szebehely, V. *Theory of Orbits: The Restricted Problem of Three Bodies*. Academic Press, 1967.

**Integrabilidad, teoría KAM y caos**

- Arnold, V. I., Kozlov, V. V. y Neishtadt, A. I. *Mathematical Aspects of Classical and Celestial Mechanics* (3ª ed.). Springer, 2006.
- Moser, J. *Stable and Random Motions in Dynamical Systems*. Princeton University Press, 1973.
- Chierchia, L. y Mather, J. N. «Kolmogorov–Arnold–Moser Theory». *Scholarpedia*, 5(9):2123, 2010.
- Lichtenberg, A. J. y Lieberman, M. A. *Regular and Chaotic Dynamics* (2ª ed.). Springer, 1992.

**Mecánica geométrica y geometría simpléctica**

- Souriau, J.-M. *Structure des Systèmes Dynamiques*. Dunod, 1970. (Ed. inglesa: *Structure of Dynamical Systems*, Birkhäuser, 1997.)
- McDuff, D. y Salamon, D. *Introduction to Symplectic Topology* (3ª ed.). Oxford University Press, 2017.

**Relatividad general**

- Misner, C. W., Thorne, K. S. y Wheeler, J. A. *Gravitation*. W. H. Freeman, 1973.
- Wald, R. M. *General Relativity*. University of Chicago Press, 1984.

**Integración geométrica y simpléctica**

- Hairer, E., Lubich, C. y Wanner, G. *Geometric Numerical Integration* (2ª ed.). Springer, 2006.
- Leimkuhler, B. y Reich, S. *Simulating Hamiltonian Dynamics*. Cambridge University Press, 2004.

**Filosofía de la física**

- Earman, J. *A Primer on Determinism*. Reidel, 1986.
- Sklar, L. *Philosophy of Physics*. Oxford University Press, 1992.
- Smith, G. E. «The Methodology of the *Principia*». En *The Cambridge Companion to Newton*, Cambridge University Press, 2002.

---

*© 2026 Mikhael da Silva. Licenciado bajo Creative Commons Attribution-NoDerivatives 4.0 International (CC BY-ND 4.0).*
