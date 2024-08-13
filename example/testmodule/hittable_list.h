#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct {
    hittable base;
    hittable** objects;
    size_t size;
} hittable_list;

hittable_list* hittable_list_create() {
    hittable_list* list = (hittable_list*)malloc(sizeof(hittable_list));
    if (list == NULL) {
        return NULL;
    }

    list->base.hit = NULL;
    list->objects = NULL;
    list->size = 0;

    return list;
}

void hittable_list_clear(hittable_list* list) {
    if (list->objects != NULL) {
        for (size_t i = 0; i < list->size; i++) {
            free(list->objects[i]);
        }
        free(list->objects);
        list->objects = NULL;
        list->size = 0;
    }
}

void hittable_list_add(hittable_list* list, hittable* object) {
    hittable** new_objects = (hittable**)realloc(list->objects, (list->size + 1) * sizeof(hittable*));
    if (new_objects == NULL) {
        return;
    }

    list->objects = new_objects;
    list->objects[list->size] = object;
    list->size++;
}

int hittable_list_hit(const hittable_list* list, const ray* r, double ray_tmin, double ray_tmax, hit_record* rec) {
    hit_record temp_rec;
    int hit_anything = 0;
    double closest_so_far = ray_tmax;

    for (size_t i = 0; i < list->size; i++) {
        if (list->objects[i]->hit(list->objects[i], r, ray_tmin, closest_so_far, &temp_rec)) {
            hit_anything = 1;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }

    return hit_anything;
}

void hittable_list_destroy(hittable_list* list) {
    hittable_list_clear(list);
    free(list);
}

#endif