//
// Created by xiehao on 2024/9/24.
//

#ifndef MY_THREE_COLOR_OOPMAPSET_H
#define MY_THREE_COLOR_OOPMAPSET_H

class OopMap;

class OopMapSet {
private:
    int _om_count;
    int _om_size;
    OopMap** _om_data;

public:
    int om_count() const                { return _om_count;     }
    void set_om_count(int value)        { _om_count = value;    }
    void increment_count()              { _om_count++;          }
    int om_size() const                 { return _om_size;      }
    void set_om_size(int value)         { _om_size = value;     }
    OopMap** om_data() const            { return _om_data;      }
    void set_om_data(OopMap** value)    { _om_data = value;     }
    void grow_om_data();
    void set(int index, OopMap* value) {
        _om_data[index] = value;
    }

public:
    OopMapSet();

    // Collect OopMaps
    void add_gc_map(OopMap* map);
};

#endif //MY_THREE_COLOR_OOPMAPSET_H
