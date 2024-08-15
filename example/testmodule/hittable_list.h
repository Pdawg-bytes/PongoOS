#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "sphere.h"
#include "moving_sphere.h"

typedef struct hittable_list
{
	hittable_object object;
	struct hittable_list* next;
} hittable_list;

hittable_list* hittable_list_create(hittable_object object)
{
	hittable_list* new;

	new = malloc(sizeof(hittable_list));
	if (new)
	{
		new->object = object;
		new->next = NULL;
	}
	return (new);
}

void hittable_list_push(hittable_list** lst, hittable_list* new)
{
	hittable_list* temp;

	temp = *lst;
	*lst = new;
	(*lst)->next = temp;
}

void hittable_list_drop(hittable_list* lst)
{
	if (lst)
	{
		free(lst->object.pointer);
		free(lst);
	}
}

void hittable_list_clear(hittable_list** lst)
{
	hittable_list* temp;

	if (lst)
	{
		while (*lst)
		{
			temp = (*lst)->next;
			hittable_list_drop(*lst);
			(*lst) = temp;
		}
		lst = NULL;
	}
}

static int hittable_object_hit(hittable_object* object, ray* r, double t_min, double t_max, hit_record* rec)
{
	int is_hit;

	switch (object->geometry)
	{
	case sphere_geometry:
		is_hit = sphere_hit(object->pointer, r, t_min, t_max, rec);
		break;
	case moving_sphere_geometry:
		is_hit = moving_sphere_hit(object->pointer, r, t_min, t_max, rec);
		break;
	}
	if (is_hit)
		rec->material = object->material;
	return (is_hit);
}

int hittable_list_hit(hittable_list* current, ray* r, double t_min, double t_max, hit_record* rec)
{
	hit_record temp_rec;
	double closest_so_far = t_max;
	int hit_anything = FALSE;

	while (current)
	{
		if (hittable_object_hit(&current->object, r, t_min, t_max, &temp_rec))
			if (temp_rec.t < closest_so_far)
			{
				hit_anything = TRUE;
				closest_so_far = temp_rec.t;
				*rec = temp_rec;
			}
		current = current->next;
	}
	return (hit_anything);
}

#endif