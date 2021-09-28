#include <stdio.h>
#include <stdlib.h>

// functia pentru media aritmetica
double medie(int **harta, int *m, int n)
{   int bytes = 0;
	double m_a = 0;
	for (int i = 0 ; i < n ; ++i) {
		for (int j = 0 ; j < m[i] ; ++j) {
			char *p = (char *)&harta[i][j];
// cu ajutorul unui pointer de tip char adun bitii de pe chenar
			if (!i || i == n - 1) {
				m_a += p[0] + p[1] + p[2] + p[3];
				bytes += 4;
			}
			if (i && i != n - 1) {
				if (!j) {
					m_a += p[0];
					bytes += 1;
				}
				if (j == m[i] - 1) {
					m_a += p[3];
					bytes += 1;
				}
			}
		}
	}
// efectuez media aritmetica si o returnez
	m_a /= bytes;
	return m_a;
}

// functia de stergere
void stergere(int i, int dimensiune, int x, int **harta)
{   if (dimensiune == 'C') {
// modific cu 0 byte-ul
	char *p = (char *)&harta[i][0];
	p[x - 1] = 0;
} else if (dimensiune == 'S') {
// modific cu 0 byte-ul
	short *p = (short *)&harta[i][0];
	p[x - 1] = 0;
} else {
// modific cu 0 byte-ul in caz contrar
	harta[i][x - 1] = 0;
	}
}

// functia de interschimbare
void interschimbare(int i, int dimensiune, int x, int **harta)
{	int tmp;
	if (dimensiune == 'I') {
		char *p = (char *)&harta[i][x];
// fac interschimbare utilizand o variabila tmp
		tmp = p[0];
		p[0] = p[3];
		p[3] = tmp;
		tmp = p[1];
		p[1] = p[2];
		p[2] = tmp;
	} else if (dimensiune == 'S') {
		short *p = (short *)&harta[i][0];
		char *q = (char *)&p[x];
// fac interschimbare utilizand o variabila tmp
		tmp = q[0];
		q[0] = q[1];
		q[1] = tmp;
	}
}

// functia de modificare
void modificare(int i, int dimensiune, int x, int y, int *m, int **harta)
{	if (dimensiune == 'C') {
// verific dimensiunea
	if (4 * m[i] < x) {
		int dif = 0;
		if ((x - 4 * m[i]) % 4 == 0)
			dif = (x - 4 * m[i]) / 4;
		else
			dif = (x - 4 * m[i]) / 4 + 1;
		int prim = m[i];
		m[i] += dif;
		for (int j = prim ; j < m[i] ; ++j)
			harta[i][j] = 0;
	}
// modific byte-ul respectiv
	char *p = (char *)&harta[i][0];
	p[x - 1] = y;
} else if (dimensiune == 'S') {
// verific dimensiunea
	if (2 * m[i] < x) {
		int dif = 0;
		if ((x - 2 * m[i]) % 2 == 0)
			dif = (x - 2 * m[i]) / 2;
		else
			dif = (x - 2 * m[i]) / 2 + 1;
		int prim = m[i];
		m[i] += dif;
		for (int j = prim ; j < m[i] ; ++j)
			harta[i][j] = 0;
	}
// modific byte-ul respectiv
	short *p = (short *)&harta[i][0];
	p[x - 1] = y;
} else {
// verificam dimensiunea si realocam memorie
	if (m[i] < x) {
		int dif = 0;
		dif = x - m[i];
		int prim = m[i];
		m[i] += dif;
		harta[i] = realloc(harta[i], m[i] * sizeof(int));
		for (int j = prim ; j < m[i] ; ++j)
			harta[i][j] = 0;
		}
// modifc byte-ul
	harta[i][x - 1] = y;
	}
}

// functie fill in care cautam portiunea maxima de zerouri
void fill(int x, int y, int n, int *m, int **harta, int *count)
{   // verificam ca inca suntem in matrice
	if (x < 0 || x >= n || y < 0 || y >= 4 * m[x])
		return;
	char *p = (char *)&harta[x][0];
	if (p[y])
		return;
// marcam ca trecem pe aici si numaram 0
	p[y] = -1;
	++(*count);
// apelam functia fill pentru vecini
	fill(x + 1, y, n, m, harta, count);
	fill(x - 1, y, n, m, harta, count);
	fill(x, y + 1, n, m, harta, count);
	fill(x, y - 1, n, m, harta, count);
}

int main(void)
{   int n, k, ok, l, x, y;
	int x_min, y_min;
	char intrare, dimensiune;
	int **harta, *m;

	scanf("%d", &n);
	harta = malloc(n * sizeof(int *));
	m = malloc(n * sizeof(int));
// aloc memorie pentru matrice si citesc
	for (int i = 0 ; i < n ; ++i) {
		scanf("%d", &k);
		m[i] = k;
		harta[i] = malloc(m[i] * sizeof(int));
		for (int j = 0 ; j < m[i] ; ++j)
			scanf("%X", &harta[i][j]);
	}

// calculez media aritmetica
	double m_a;
	m_a = medie(harta, m, n);

	scanf("%d", &ok);
// efectuam modificarile cerute
	for (int i = 0 ; i < ok ; ++i) {
		scanf(" %c ", &intrare);
		scanf("%c%d%d", &dimensiune, &l, &x);
		if (intrare == 'M') {
			scanf("%X", &y);
			modificare(l, dimensiune, x, y, m, harta);
		} else if (intrare == 'D') {
			stergere(l, dimensiune, x, harta);
		} else {
			interschimbare(l, dimensiune, x, harta);
		}
	}
// afisarea task-urilor
	printf("task 1\n%.7f\n", m_a);
	printf("task 2\n");

	for (int i = 0 ; i < n ; ++i) {
		for (int j = 0 ; j < m[i] ; ++j)
			printf("%08X ", harta[i][j]);
		printf("\n");
	}
	printf("task 3\n");

	int amax = -1, count;
	for (int i = 0 ; i < n ; ++i) {
		char *p = (char *)&harta[i][0];
		for (int j = 0; j < 4 * m[i] ; ++j) {
			if (p[j] == 0) {
				count = 0;
				fill(i, j, n, m, harta, &count);
				if (count > amax) {
					amax = count;
					y_min = j;
					x_min = i;
				}
			}
		}
	}

	printf("%d %d %d\n", x_min, y_min, amax);

	for (int i = 0 ; i < n ; ++i)
		free(harta[i]);

	free(harta);
	free(m);

	return 0;
}
