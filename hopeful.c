//cutting by a1 this round, cutting by a2 next round
    if(parent == NULL || n < 1)
    {
        return;
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
        return;
    }

    //populate (partially empty) parent     
    parent->loc->x = median.x;
    parent->loc->y = median.y;
    
    if(n > 2)
    {
        bool(*f)(const location, const location);

        if (depth % 2 == 0)
        {
            f = y_sorted;
        }
        else
        {
            f = x_sorted;
        }

        //split into four arrays -- two left, two right
        location a1_right[right_size]; location a1_left[left_size]; location a2_right[right_size]; location a2_left[left_size];

        int i;
        for(i = 0; i < n; i++)
        {
            if(i < mid_index)
            {
                a1_left[i] = a1[i];
            }
            else if(i > mid_index)
            {
                a1_right[i - mid_index] = a1[i];
            }

            if(f(a2[i], median))
            {
                a2_left[i] = a2[i];
            }
            else if(f(median, a2[i]))
            {
                a2_right[i] = a2[i];
            }
        }

        parent->r_child = malloc(sizeof(node));
        parent->l_child = malloc(sizeof(node));
        
        if(parent->r_child == NULL || parent->l_child == NULL)
        {
            free(parent->loc);
            free(parent->r_child);
            free(parent->l_child);  
            
            return;
        }

        //initialize children
        parent->r_child->parent = parent;
        parent->l_child->parent = parent;

        subtree_create(a2_right, a1_right, parent->r_child, right_size, depth+1);
        subtree_create(a2_left, a1_left, parent->l_child, left_size, depth+1);

    }
    else
    {
        parent->r_child = NULL;
        parent->l_child = NULL;
    }