#ifndef TRAILSETTINGS_H
#define TRAILSETTINGS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class TrailSettings : public Resource {
    GDEXTENSION_CLASS(TrailSettings, Resource)

public:
    int num_points;
	float noise_scale;
	float size;
	double update_interval;
	double uv_shift;
	bool billboard;
	bool emitting = true;

};

}

#endif