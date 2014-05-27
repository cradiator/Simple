number_array = [9, 2, 3, 8, 6, 1];

function calculate_inversion(a, start, end)
{
    if (end - start <= 1)
    {
        return 0;
    }

    mid = (start + end) / 2;
    left  = calculate_inversion(a, start, mid);
    right = calculate_inversion(a, mid, end);

    temp_array   = array(end - start);
    left_cursor  = start;
    right_cursor = mid;
    temp_cursor  = 0;

    inversion = 0;
    for(temp_cursor = 0; temp_cursor < temp_array.size(); temp_cursor = temp_cursor + 1)
    {
        if ((right_cursor >= end) || (left_cursor < mid && a[left_cursor] <= a[right_cursor]))
        {
            temp_array[temp_cursor] = a[left_cursor];
            left_cursor = left_cursor + 1;
        }
        else
        {
            temp_array[temp_cursor] = a[right_cursor];
            right_cursor = right_cursor + 1;
            inversion = inversion + (mid - left_cursor);
        }
    }

    for(i = start; i < end; i = i + 1)
    {
        a[i] = temp_array[i - start];
    }

    return left + right + inversion;
}

result = calculate_inversion(number_array, 0, number_array.size());
print(result);

