#include <string.h>
#include <memory>

typedef struct point
{
    int x, y;
} POINT;

// a segment is a string like: (x, y)
// eg: (10, 20)
// eg: (5123, -15123)
POINT segment_to_point(char* segment)
{
    // buffer will contain the segment without the parentheses:
    int buffer_length = strlen(segment) - 2;
    char* buffer = (char*)malloc(sizeof(char) * (buffer_length + 1));
    memcpy(buffer, segment + 1, buffer_length);
    buffer[buffer_length] = '\0';

    // find the comma which separates x and y:
    // (we could also use strtok, but since there's just two parts, this is more simple)
    for (int i = 0; i != buffer_length; ++i)
    {
        if (buffer[i] == ',')
        {
            POINT p;
            p.x = atoi(buffer);         // reads digits until the comma (automatically stops)
            p.y = atoi(buffer + i + 1); // reads digits after the comma until the end
            return p;
        }
    }

    // in case there is no comma:
    // (this should never happen)
    return POINT();
}

POINT* Exam(char* pInput, int* pnPoints)
{
    // assume there won't be over 100 POINTS:
    POINT* points = (POINT*)malloc(sizeof(POINT) * 100);
    int point_count = 0;

    // break the input into segments; each segment is separated by a semicolon:
    char* current_segment = strtok(pInput, ";");
    while (current_segment != NULL)
    {
        // skip whitespace between segments:
        while (*current_segment == ' ')
            ++current_segment;

        // turn the segment into a POINT and put it in the array:
        points[point_count] = segment_to_point(current_segment);
        ++point_count;

        // next segment:
        current_segment = strtok(NULL, ";");
    }

    // resize the array to match its actual size:
    // (we allocated enough space for 100 POINTS; we can free up the memory we didn't use now)
    points = (POINT*)realloc(points, sizeof(POINT) * point_count);

    *pnPoints = point_count;
    return points;
}

int main()
{
    // test:
    char test_str[] = "(130, -512312);  (0, 0); (123, -123);   (512312523, -125121233); (0, 0)";
    int point_count = 0;
    POINT* points = Exam(test_str, &point_count);

    // ADDITIONAL CONDITION - NO DUPLICATE POINTS ALLOWED:

    // create a new (filtered) array of points, which shall contain only non-duplicates:
    POINT* filtered_points = (POINT*)malloc(sizeof(POINT) * point_count);
    int filtered_point_count = 0;
    // go through all (unfiltered) points, and only copy them into the new array if they aren't already in it:
    for (int i = 0; i != point_count; ++i)
    {
        POINT p = points[i];

        int duplicate = 0;
        for (int j = 0; j != filtered_point_count; ++j)
        {
            if (filtered_points[j].x == p.x && filtered_points[j].y == p.y)
            {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate)
        {
            filtered_points[filtered_point_count] = p;
            ++filtered_point_count;
        }
    }
    // if any points were filtered out, then free unused memory by resizing the array:
    filtered_points = (POINT*)realloc(filtered_points, sizeof(POINT) * filtered_point_count);

    // print and verify:
    for (int i = 0; i != filtered_point_count; ++i)
        printf("x: %d\ny: %d\n\n", filtered_points[i].x, filtered_points[i].y);
}
