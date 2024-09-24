//
// Created by xiehao on 2024/9/24.
//

#include "OopMapSet.h"
#include "../../../include/common.h"

OopMapSet::OopMapSet() {
    set_om_size(MinOopMapAllocation);

    set_om_count(0);

    OopMap **temp = (OopMap **) calloc(om_count(), ALIGN_SIZE);

    set_om_data(temp);
};

void OopMapSet::add_gc_map(OopMap *map) {
    if (om_count() >= om_size()) {
        grow_om_data();
    }

    set(om_count(), map);
    increment_count();
}

void OopMapSet::grow_om_data() {
    int new_size = om_size() * 2;
    OopMap **new_data = (OopMap **) calloc(new_size, ALIGN_SIZE);
    memcpy(new_data, om_data(), om_size() * sizeof(OopMap *));
    set_om_size(new_size);
    set_om_data(new_data);
}