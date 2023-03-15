struct GCRoot {
    GCRoot* next;
    void* obj_ptr;
};
