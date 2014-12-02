#ifndef EDGE_H

#define EDGE_H

struct Edge {
	Edge(int u, int v, int w) : u(u), v(v), w(v) {}
	int u;
	int v;
	int w;
};

#endif
