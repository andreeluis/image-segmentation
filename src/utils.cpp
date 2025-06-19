bool is_inside(int row, int col, int rows, int cols)
{
  return row >= 0 && row < rows && col >= 0 && col < cols;
}

int pixel_index(int row, int col, int cols)
{
  return row * cols + col;
}