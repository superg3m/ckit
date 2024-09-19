// make easy graphs

void ckit_line_graph_create(const char* x_axis_name, double x_axis_scale, const char* y_axis_name, double y_axis_scale);
void ckit_line_graph_set_data(double* x_axis_data, double* y_axis_data);
// I should be able to scrub through the line graph. I think all I need to do is keep track of the mouse postiion and then render a rectangle vertically
// then I just take the x position of the two closes points and lerp them to get the value.

void ckit_bar_graph_create(const char* x_axis_name, double x_axis_scale, const char* y_axis_name, double y_axis_scale);
void ckit_bar_graph_set_data(double* x_axis_data, double* y_axis_data);