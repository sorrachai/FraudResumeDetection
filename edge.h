#ifndef EDGE_H

#define EDGE_H

struct pair_line {
	pair_line(int u, int v, int w) : u(u), v(v), w(v) {}
	int u;
	int v;
	int w;
};

#endif
