#include <vector>

#include "point.h"

enum class obj_type {
	POLYGON,
	POLYMESH,
	CURVE,
	PATH,
	TEXT
};

class Object {
public:
	Object(Point o)
		: m_origin{ o },
		m_scale{ 1.0, 1.0 },
		m_rotation{ 0 },
		m_type{ obj_type::POLYGON }
	{ };
	Point get_origin() const { return m_origin; };
	void set_origin(Point o) { m_origin = o; };

	Point get_scale() const { return m_scale; };
	void set_scale(Point s) { m_scale = s; };


	double get_rotation() const { return m_rotation; };
	void set_rotation(double radians) { m_rotation = radians; };

private:
	Point m_origin;
	Point m_scale;
	double m_rotation;	// radians
	obj_type m_type;
};