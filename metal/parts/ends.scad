/* AVR-CNC angled endings */

module ends(dim = [1, 1, 1], a = [[0, 0], [0, 0]]) {
  l = dim[0];
  w = dim[1];
  h = dim[2];

  for (i = [0 : 1]) {
    if (a[i] != [0, 0]) {
      q = 1.1;
      w2 = w / 2 * q;
      h2 = h / 2 * q;

      wt = w * tan(a[i][0]) * q;
      ht = h * tan(a[i][1]) * q;

      l2 = (l + w * tan(a[i][0]) * (q - 1) + h * tan(a[i][1]) * (q - 1)) / 2 +
        (wt < 0 ? wt / q : 0) + (ht < 0 ? ht / q : 0);
      
      lm = l2 + abs(ht) + abs(wt);

      m = i == 0 ? 1 : -1;
      
      top =    [[-w2, -h2, m * (lm)], [w2, -h2, m * (lm - wt)],
                [w2,  h2,  m * (lm - wt - ht)], [-w2, h2, m * (lm - ht)]];
      bottom = [[-w2, -h2, m * (l2)], [w2, -h2, m * (l2 - wt)],
                [w2,  h2,  m * (l2 - wt - ht)], [-w2, h2, m * (l2 - ht)]];
      
      points = i == 0 ? concat(bottom, top) : concat (top, bottom);

      polyhedron(points = points,
        faces = [[0, 1, 2, 3], [0, 3, 7, 4],
                 [5, 6, 2, 1], [4, 5, 1, 0],
                 [6, 7, 3, 2], [7, 6, 5, 4]],
        convexity = 1);
    }
  }
  
}
