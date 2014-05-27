function left_child_index(index)
{
    return index * 2 + 1;
}

function right_child_index(index)
{
    return index * 2 + 2;
}

function heapify(a, root_index, heap_size)
{
    if (left_child_index(root_index) >= heap_size)
    {
        return;
    }

    lindex = left_child_index(root_index);
    rindex = right_child_index(root_index);

    large_index = root_index;
    if (lindex < heap_size && a[lindex] > a[large_index])
    {
        large_index = lindex;
    }

    if (rindex < heap_size && a[rindex] > a[large_index])
    {
        large_index = rindex;
    }

    if (large_index != root_index)
    {
        temp = a[root_index];
        a[root_index] = a[large_index];
        a[large_index] = temp;
        heapify(a, large_index, heap_size);
    }
}

function create_heap(a)
{
    for(i = a.size() - 1; i >= 0; i = i - 1)
    {
        heapify(a, i, a.size());
    }
}

function heap_sort(a)
{
    create_heap(a);

    heap_size = a.size();
    while(heap_size > 0)
    {
        temp = a[0];
        a[0] = a[heap_size - 1];
        a[heap_size - 1] = temp;
        heap_size = heap_size - 1;

        heapify(a, 0, heap_size);
    }
}

number_array = [5, 13, 100, 2, 25, 7, 17, 10, 8, 4];
heap_sort(number_array);
print(number_array);
