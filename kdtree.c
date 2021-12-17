#include <math.h>
#include <limits.h>
#include <string.h>

#include "kdtree.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "location.h"

typedef struct _node node;

struct _node
{
    node *parent;
    location *loc;
    node *l_child;
    node *r_child;
};
struct _kdtree
{
    node* root;
};



node* subtree_create(location a1[], location a2[], node* parent, int n, int depth);
bool search(const node* n, const location* p, int depth);
void tree_destroy(node* n);
void tree_for_each(const node* n, void (*f)(const location *, void *), void *arg);
bool tree_add(node* n, node* new, int depth);
void nearest_search(node* n, const location *p, location *neighbor, double *d, location lower_left, location upper_right, int depth);
bool x_sorted(const location p1, const location p2);
bool y_sorted(const location p1, const location p2);
void merge(int n1, const location a1[], int n2, const location a2[], location out[], bool (*f)(const location, const location));
void mergeSort(int n, const location a[], location out[], bool (*f)(const location, const location));

/* 
 * Create a balanced kdtree
 */
kdtree *kdtree_create(const location *pts, int n)
{
    kdtree* kdt = malloc(sizeof(kdtree));

    if(kdt != NULL)
    {
        if(pts == NULL || n == 0)
        {
            return kdt;
        }

        node* root = malloc(sizeof(node));
        
        if (root != NULL)
        {

            root->parent = NULL;
            root->r_child = NULL;
            root->l_child = NULL;
            root->loc = NULL;

            //sort points by x and y
            location x_array[n];
            mergeSort(n, pts, x_array, x_sorted);

            location y_array[n];
            mergeSort(n, pts, y_array, y_sorted);

            kdt->root = subtree_create(x_array, y_array, root, n, 0);
        }
        else
        {
            free(kdt);
            free(root);
            return NULL;
        }
    }
    else
    {
        free(kdt);
        return NULL;
    }

    return kdt;
}

node* subtree_create(location a1[], location a2[], node* parent, const int n, const int depth)
{
    //cutting by a1 this round, cutting by a2 next round
    if(parent == NULL || n < 1)
    {
        return parent;
    }

    //find midpoint index
    int mid_index = (n - 1) / 2;
    int left_size = mid_index;
    int right_size = n - left_size - 1;

    location median = a1[mid_index];

    parent->loc = malloc(sizeof(location));
    if(parent->loc == NULL)
    {
        free(parent->loc);

        parent->l_child = NULL;
        parent->r_child = NULL;

        return parent;
    }

    //populate (partially empty) parent     
    parent->loc->x = median.x;
    parent->loc->y = median.y;
    
    bool(*f)(const location, const location);

    if (depth % 2 == 0)
    {
        f = x_sorted;
    }
    else
    {
        f = y_sorted;
    }
    
    if(n > 2)
    {
        //split into four arrays -- two left, two right
        location a1_right[right_size]; location a1_left[left_size]; location a2_right[right_size]; location a2_left[left_size];
        
        int j = 0;
        int k = 0;
        int q = 0;
        int h = 0;
        for(int i = 0; i < n; i++)
        {
            if(i < mid_index) //sort array a1
            {
                a1_left[h] = a1[i];
                h++;
            }
            else if(i > mid_index)
            {
                a1_right[q] = a1[i];
                q++;
            }

            if(f(a2[i], median)) //sort array a2
            {
                a2_left[j] = a2[i];
                j++;
            }
            else if(f(median, a2[i]))
            {
                a2_right[k] = a2[i];
                k++;
            }
        }

        node* new_r_child = malloc(sizeof(node));
        node* new_l_child = malloc(sizeof(node));
  
        if(new_r_child == NULL || new_l_child == NULL)
        {
            free(new_r_child);
            free(new_l_child); 

            parent->l_child = NULL;
            parent->r_child = NULL;

            return parent;
        }

        //initialize children
        new_r_child->parent = parent;
        new_r_child->loc = NULL;

        new_l_child->parent = parent;
        new_l_child->loc = NULL;
        
        parent->l_child = subtree_create(a2_left, a1_left, new_l_child, left_size, depth+1);
        parent->r_child = subtree_create(a2_right, a1_right, new_r_child, right_size, depth+1);

    }
    else if(n == 2)
    {
        node* new_node = malloc(sizeof(node));
        location* point = malloc(sizeof(location));

        if(new_node == NULL || point == NULL )
        {
            free(new_node);
            free(point);

            parent->l_child = NULL;
            parent->r_child = NULL;

            return parent;
        }

        point->x = a1[1].x;
        point->y = a1[1].y;

        new_node->parent = parent;
        new_node->l_child = NULL;
        new_node->r_child = NULL;
        new_node->loc = point;

        if(f(a1[1], median)) //goes left
        {
            parent->l_child = new_node;
            parent->r_child = NULL;  
        }
        else if(f(median, a1[1])) //goes right
        {
            parent->r_child = new_node;
            parent->l_child = NULL;
        }
        else
        {
            free(new_node);
            free(point);
        }

        return parent;
    }
    else
    {
        parent->r_child = NULL;
        parent->l_child = NULL;
    }

    return parent;
}





/*
 * Adds a point to a tree
 */
bool kdtree_add(kdtree *t, const location *p)
{
    if (t!= NULL && p != NULL)
    {
        node* new = malloc(sizeof(node));
        location* point = malloc(sizeof(location));

        if (new == NULL || point == NULL )
        {
            free(new);
            free(point);
            return false;
        }

        new->r_child = NULL;
        new->l_child = NULL;

        point->x = p->x;
        point->y = p->y;
            
        new->loc = point;


        if(t->root == NULL)
        {
            new->parent = NULL;
            t->root = new;
            
            return true;
        }


        return tree_add(t->root, new, 0);
    }
    
    else return false;  
    
}

bool tree_add(node* n, node* new, int depth)
{ 
    if(n->loc->x == new->loc->x && n->loc->y == new->loc->y)
    {
        free(new->loc);
        free(new);
        return false;
    }

    bool (*f)(const location p1, const location p2); //check against x or y depending on depth
    if(depth % 2 == 0)
    {
        f = x_sorted;
    }
    else
    {
        f = y_sorted;
    }
    depth++;
    
    if(f(*(n->loc), *(new->loc))) //should be on the right
    {
        if (n->r_child == NULL)
        {

            n->r_child = new;
        }
        else
        {
            return tree_add(n->r_child, new, depth);
        } 
    }
    else //should be on the left
    {
        if(n->l_child == NULL)
        {  
            n->l_child = new;
        }
        else
        {
            return tree_add(n->l_child, new, depth);
        }
    }

    new->parent = n;
    return true;
}




/* 
 * Figure out if the tree contains a certain location
 */
bool kdtree_contains(const kdtree *t, const location *p)
{
    if(t == NULL || p == NULL)
    {
        return false;
    }

    return search(t->root, p, 0);
}

bool search(const node* n, const location* p, int depth)
{
    if(n == NULL)
    {
        return false;
    }

    if(n->loc->x == p->x && n->loc->y == p->y)
    {
        return true;
    }

    bool(*f)(const location, const location);

    if(depth % 2 == 0)
    {
       f = x_sorted; 
    }
    else
    {
        f = y_sorted;
    }

    if(f(*(n->loc), *p))
    {
        return search(n->r_child, p, depth+1);
    }
    else if (f(*p, *(n->loc)))
    {
        return search(n->l_child, p, depth+1);
    }
    else return false;
}





/**
 * Copies the nearest neighbor to the given point among points in the
 * given tree to the given location.  Ties are broken arbitrarily.
 If p is equal to a point in the tree then p is
 * copied into neighbor and distance is set to zero.
 *
 * @param t a pointer to a valid k-d tree, non-NULL
 * @param p a pointer to a valid location, non-NULL
 * @param d a pointer to a double, non-NULL
 * @param neighbor a pointer to a valid location, non-NULL
 */
void kdtree_nearest_neighbor(kdtree *t, const location *p, location *neighbor, double *d)
{
    if(t != NULL && p != NULL && neighbor != NULL && d != NULL)
    {
        if(kdtree_contains(t, p))
        {
            *d = 0;
            neighbor->x = p->x;
            neighbor->y = p->y;
        }
        else
        {
            *d = INFINITY;
            if(t->root != NULL)
            {
                location lower_left = {-INFINITY, -INFINITY};
                location upper_right = {INFINITY, INFINITY};
                nearest_search(t->root, p, neighbor, d, lower_left, upper_right, 0);
            }
        }
    }
}
void nearest_search(node* n, const location *p, location *neighbor, double *d, location lower_left, location upper_right, int depth)
{
    if(n == NULL)
    {
        return;
    }

    double current_dist;
    if((current_dist = location_distance(p, n->loc)) < *d) //IF CURRENT POINT IS CLOSER MAKE IT THE NEAREST NEIGHBOR
    {
        *d = current_dist;
        neighbor->x = n->loc->x;
        neighbor->y = n->loc->y;
    }
    
    location right_ll, right_ur, left_ll, left_ur;
    
    right_ll.x = lower_left.x;
    right_ll.y = lower_left.y;
    right_ur.x = upper_right.x;
    right_ur.y = upper_right.y;

    left_ll.x = lower_left.x;
    left_ll.y = lower_left.y;
    left_ur.x = upper_right.x;
    left_ur.y = upper_right.y;
    
    if(depth % 2 == 0) //if cutting by x
    {
        right_ll.x = n->loc->x;
        left_ur.x = n->loc ->x; 
    }
    else //if cutting by y.
    {
        right_ll.y = n->loc->y;
        left_ur.y = n->loc->y;
    }

    double dist_to_rr = location_distance_to_rectangle(neighbor, &right_ll, &right_ur);
    double dist_to_lr = location_distance_to_rectangle(neighbor, &left_ll, &left_ur);
    
    //FIGURE OUT WHERE TO LOOK 
    //if the distance to the region of the right child is less than current distance, look right
    if(dist_to_rr < *d)
    {
        nearest_search(n->r_child, p, neighbor, d, right_ll, right_ur, depth+1);
    }
    //if the distance to the region of the left child is less than the current distance, look left
    if(dist_to_lr < *d)
    {
        nearest_search(n->l_child, p, neighbor, d, left_ll, left_ur, depth+1);
    }
}





/* 
 * Perform function f on each location in node; nothing else is changed in the tree
 */
void kdtree_for_each(const kdtree* r, void (*f)(const location *, void *), void *arg)
{
    if(r->root != NULL)
    {
        tree_for_each(r->root, f, arg);
    }
}

void tree_for_each(const node* n, void (*f)(const location *, void *), void *arg)
{
    f(n->loc, arg);

    if(n->l_child != NULL)
    {
        tree_for_each(n->l_child, f, arg);
    }
    if(n->r_child != NULL)
    {
        tree_for_each(n->r_child, f, arg);
    }
}





/* 
 * Destory the tree freeing all allocated memory 
 */
void kdtree_destroy(kdtree *t)
{   
    if(t != NULL)
    {
        tree_destroy(t->root);
        free(t); 
    }   
}

void tree_destroy(node* n)
{
    if (n!= NULL)
    {
        if (n->l_child != NULL)
        {
            tree_destroy(n->l_child);
        }
        if (n->r_child != NULL)
        {
            tree_destroy(n->r_child);
        }

        free(n->loc);
        free(n);
    }
}

bool x_sorted(const location p1, const location p2)
{
    if(p1.x < p2.x)
    {
        return true;
    }
    else if(p1.x > p2.x)
    {
        return false;
    }
    else if (p1.y < p2.y)
    {
        return true;
    }
    else return false;
}

bool y_sorted(const location p1, const location p2)
{
    if(p1.y < p2.y)
    {   
        return true;
    }
    else if (p1.y > p2.y)
    {
        return false;
    }
    else if (p1.x < p2.x)
    {
        return true;
    }
    else return false;
}

/* merge sorted arrays a1 and a2, putting result in out */
void merge(int n1, const location a1[], int n2, const location a2[], location out[], bool (*f)(const location, const location))
{
    int i1;
    int i2;
    int iout;

    i1 = i2 = iout = 0;

    while(i1 < n1 || i2 < n2) 
    {
        if(i2 >= n2 || ((i1 < n1) && f(a1[i1], a2[i2]))) 
        {
            /* a1[i1] exists and is smaller */
            out[iout++] = a1[i1++];
        } 
        else 
        {
            /* a1[i1] doesn't exist, or is bigger than a2[i2] */
            out[iout++] = a2[i2++];
        }
    }
}

void mergeSort(int n, const location a[], location out[], bool (*f)(const location, const location))
{
    location* a1;
    location* a2;

    if(n < 2)
    {
        /* 0 or 1 elements is already sorted */
        memcpy(out, a, sizeof(location)*n);
    }
    else
    {
        /* sort into temp arrays */
        a1 = malloc(sizeof(location) * (n/2));
        a2 = malloc(sizeof(location) * (n - n/2));

        mergeSort(n/2, a, a1, f);
        mergeSort(n - n/2, a + n/2, a2, f);

        merge(n/2, a1, n - n/2, a2, out, f);

        /* free the temp arrays */
        free(a1);
        free(a2);
    }
}


