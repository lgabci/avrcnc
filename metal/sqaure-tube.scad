//$fn = 100;

module _square_bar(w, h, l, r) {
  hull() {
    for (i = [[-1, -1], [-1, 1], [1, 1], [1, -1]]) {
      translate([i[0] * (w / 2 - r), i[1] * (h / 2 - r), 0]) {
        cylinder(h = l, r = r, center = true);
      }
    }
  }
}

module square_tube(l, dim = [20, 20, 2, 3], pos = [0, 0, 0], rot = [0, 0, 0], a = [0, 0]) {
  w = dim[0];
  h = dim[1];
  d = dim[2];
  r = dim[3];

  translate(pos) {
    rotate(rot) {
      //rotate([0, 0, zrot]) {
        difference() {
          _square_bar(w, h, l, r);
          //cube([w, h, l], center = true);
          _square_bar(w - 2 * d, h - 2 * d, l + 1, r - d);


          //translate([0, 0, (l - h / cos(a[1]) / tan(a[0])) / 2]) {
          //  rotate([a[0], 0, a[1]]) {
          //    #cube([3 * w, 2 * h, 0.01], center = true);
          //  }
          //}

          if (a != [0, 0]) {
            w2 = w / 2;
            h2 = h / 2;
            l2 = l / 2;
            
            wt = w * tan(a[0]);
            ht = h * tan(a[1]);
            lm = l2 + ht + wt + 1;

            polyhedron(points = [[-w2, -h2, l2], [w2, -h2, l2 - wt], [w2, h2, l2 - wt - ht], [ -w2, h2, l2 - ht],
                                  [-w2, -h2, lm], [w2, -h2, lm - wt], [w2, h2, lm - wt - ht], [ -w2, h2, lm - ht]],
              faces = [[0, 1, 2, 3], [0, 3, 7, 4], [5, 6, 2, 1], [4, 5, 1, 0], [6, 7, 3, 2], [7, 6, 5, 4]],
              convexity = 0);
          }


        }
      //}
    }
  }


}


square_tube(200, [20, 40, 2, 3], a = [0, 45]);
square_tube(200 + tan(45) * 40, [40, 80, 2, 3], a = [0, 45]);
//square_tube([20, 40, 2, 3], [50, 0, 105], [0, 200, 105], [1, 1], 0);
//square_tube([40, 20, 2, 3], [50, 0, 0],   [50, 0, 105],   [2, 2], atan2(200, -50) - 90);
//square_tube([40, 20, 2, 3], [0, 200, 0],  [0, 200, 105], [4, 4], atan2(200, -50) - 90);

//square_tube([20, 40, 2, 2], [20, 100, 0], [55, 100, 0], [0, 0], 0);
//square_tube([20, 40, 2, 2], [20,  50, 0], [55, 50, 0], [0, 0], 0);
