/* square tube */

module _square_bar(w, h, l, r) {
  hull() {
    for (i = [[-1, -1], [-1, 1], [1, 1], [1, -1]]) {
      translate([i[0] * (w / 2 - r), i[1] * (h / 2 - r), 0]) {
        cylinder(h = l, r = r, center = true);
      }
    }
  }
}

module square_tube(l, dim = [20, 20, 2, 3], a = [[0, 0], [0, 0]], rot = [0, 0, 0], pos = [0, 0, 0]) {
  w = dim[0];
  h = dim[1];
  d = dim[2];
  r = dim[3];

  translate(pos) {
    rotate(rot) {
      difference() {
        _square_bar(w, h, l, r);
        _square_bar(w - 2 * d, h - 2 * d, l + 1, r - d);

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
            polyhedron(points = [[-w2, -h2, m * (l2 - ht)], [w2, -h2, m * (l2 - wt - ht)],
                                  [w2,  h2,  m * (l2 - wt)], [-w2, h2, m * (l2)],
                                  [-w2, -h2, m * (lm - ht)], [w2, -h2, m * (lm - wt - ht)],
                                  [w2,  h2,  m * (lm - wt)], [-w2, h2, m * (lm)]],
              faces = [[0, 1, 2, 3], [0, 3, 7, 4],
                       [5, 6, 2, 1], [4, 5, 1, 0],
                       [6, 7, 3, 2], [7, 6, 5, 4]],
              convexity = 1);
          }
        }

      }
    }
  }


}
