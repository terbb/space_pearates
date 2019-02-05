#ifndef COLOR_COMPONENT_H
#define COLOR_COMPONENT_H

#include "utility.hpp"
#include "base_component.hpp"

// Change to RGBA after we get everything running.
class ColorComponent : public BaseComponent
{
public:
	ColorComponent(glm::vec4 _RGBA);
	~ColorComponent();
	glm::vec4 RGBA;
	static const int typeID = ComponentType::color;
	inline virtual int getTypeID() const { return typeID; };
};

#endif
