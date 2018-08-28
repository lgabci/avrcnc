//$fn = 100;

module _square_bar(w, h, l, r) {
  hull() {
    for (i = [[-1, -1], [-1, 1], [1, 1], [1, -1]]) {
      translate([i[0] * (w / 2 - r), i[1] * (h / 2 - r), 0]) {
        cylinder(h=l, r=r, center=true);
      }
    }
  }
}

module square_tube(dim, pos1, pos2, a, zrot) {
  w = dim[0];
  h = dim[1];
  d = dim[2];
  r = dim[3];

  x = pos2[0] - pos1[0];
  y = pos2[1] - pos1[1];
  z = pos2[2] - pos1[2];
  l = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

  rot = [acos(z / l), 0, atan2(y, x) + 90];

  translate([pos1[0] + x / 2, pos1[1] + y / 2, pos1[2] + z / 2]) {
    rotate(rot) {
      rotate([0, 0, zrot]) {
        difference() {
          _square_bar(w, h, l, r);
          _square_bar(w - 2 * d, h - 2 * d, l + 1, r);

          ha = [45, 0, -45, 0];
          wa = [0, 45, 0, -45];
          mwa = [h, 0, h, 0];
          mha = [0, w, 0, w];

          if (a[0] > 0 && a[0] < 5) {
            translate([0, 0, (l - mwa[a[0] - 1] - mha[a[0] - 1]) / 2]) {
              rotate([ha[a[0] - 1], wa[a[0] - 1], 0]) {
                translate([0, 0, max(w, h) / 2]) {
                  cube([w / cos(wa[a[0] - 1]) + 1, h / cos(ha[a[0] - 1]) + 1, max(w, h)], center=true);
                }
              }
            }

          }
          if (a[1] > 0 && a[1] < 5) {
            translate([0, 0, -(l - mwa[a[1] - 1] - mha[a[1] - 1]) / 2]) {
              rotate(-[ha[a[1] - 1], wa[a[1] - 1], 0]) {
                translate([0, 0, -max(w, h) / 2]) {
                  cube([w / cos(wa[a[1] - 1]) + 1, h / cos(ha[a[1] - 1]) + 1, max(w, h)], center=true);
                }
              }
            }
          }

        }
      }
    }
  }


}


square_tube([20, 40, 2, 2], [50, 0, 0],   [0, 200, 0],   [3, 3], 0);
square_tube([20, 40, 2, 2], [50, 0, 105], [0, 200, 105], [1, 1], 0);
square_tube([40, 20, 2, 2], [50, 0, 0],   [50, 0, 105],   [2, 2], atan2(200, -50) - 90);
square_tube([40, 20, 2, 2], [0, 200, 0],  [0, 200, 105], [4, 4], atan2(200, -50) - 90);

//square_tube([20, 40, 2, 2], [20, 100, 0], [55, 100, 0], [0, 0], 0);
//square_tube([20, 40, 2, 2], [20,  50, 0], [55, 50, 0], [0, 0], 0);
