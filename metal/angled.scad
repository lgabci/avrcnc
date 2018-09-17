module angled(l, dim = [20, 20, 2, 3], a = [[0, 0], [0, 0]], rot = [0, 0, 0], pos = [0, 0, 0]) {
  w = dim[0];
  h = dim[1];
  d = dim[2];
  r = dim[3];

  translate(pos) {
    rotate(rot) {
      difference() {
        //cube([w, h, l], center = true);
        //translate([d, d, 0]) {
        //  cube([w, h, l + 1], center = true);
        //}
        union() {
          translate([0, -(h - d) / 2, 0]) {
            cube([w, d, l], center = true);
          }
          translate([-(w - d) / 2, 0, 0]) {
            cube([d, h, l], center = true);
          }
          if (r > 0) {
            translate([-w / 2 + r, -h / 2 + r, 0]) {
              cube([r, r, l], center = true);
            }
          }
        }
        translate([-w / 2 + d + r, -h / 2 + d + r, 0]) {
          cylinder(h = l + 1, r = r, center = true);
        }

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

l1 = 200;
l2 = 100;
w = 50;
h = 50;

angled(l1, [w, h, 5, 5], a = [[45, 45], [0, 1]], rot = [0, 0, 0], pos = [0, 0, 0]);
/*
square_tube(l1, [w, h, 2, 3], a = [[45, 0], [45, 0]], rot = [0, 0, 0], pos = [10, 20, 100]);
square_tube(l1, [w, h, 2, 3], a = [[45, 0], [45, 0]], rot = [0, 0, 180], pos = [l2 - w + 10, 20, 100]);

square_tube(l2, [w, h, 2, 3], a = [[45, 0], [45, 0]], rot = [0, 90, 0], pos = [(l2 - w) / 2 + 10, 20, l1 - w / 2]);
square_tube(l2, [w, h, 2, 3], a = [[45, 0], [45, 0]], rot = [0, -90, 0], pos = [(l2 - w) / 2 + 10, 20, w / 2]);

square_tube(l2 - 2 * w, [w, h, 2, 3], a = [[0, 0], [0, 0]], rot = [0, 90, 0], pos = [(l2 - w) / 2 + 10, 20, l1 / 2]);
*/
