#ifndef POLYNOM_H
#define POLYNOM_H

#include <iostream>
#include "../libs/bigfloat.h"
#include "../../common_libs/t_list.h"

typedef struct poly_node 
{
	bigfloat ratio;
	unsigned int pow;

	poly_node() : ratio(0), pow(0) {}

	poly_node(const bigfloat& r, unsigned int p) : ratio(r), pow(p) {}

} p_node;

class polynom
{
private:
	t_list<poly_node> poly_list;

public:
	polynom();
	polynom(const char*);
	~polynom();

	polynom& operator+= (polynom const& arg);
	polynom& operator-= (polynom const& arg);
	polynom& operator*= (polynom const& arg);
	polynom& operator/= (polynom const& arg);

	polynom operator+ (polynom const& arg) const;
	polynom operator- (polynom const& arg) const;
	polynom operator* (polynom const& arg) const;
	polynom operator/ (polynom const& arg) const;

	polynom diff() const;
	polynom intgr() const;

	bool operator== (polynom const& arg) const;
	bool operator!= (polynom const& arg) const;

	friend std::ostream& operator<<(
		std::ostream& stream,
		polynom const& value);

	friend std::istream& operator>>(
		std::istream& stream,
		polynom& value);

	friend void standart_appearance(polynom& arg);

};

#endif //POLYNOM_H