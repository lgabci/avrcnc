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
          _square_bar(w - 2 * d, h - 2 * d, l + 1, r - d);


          translate([0, 0, (l - h / tan(a[0])) / 2]) {
            rotate([a[0], 0, a[1]]) {
              #cube([3 * w, 2 * h, 0.01], center = true);
            }
          }
        }
      //}
    }
  }


}


square_tube(200, [20, 40, 2, 3], a = [45, 65]);
//square_tube([20, 40, 2, 3], [50, 0, 105], [0, 200, 105], [1, 1], 0);
//square_tube([40, 20, 2, 3], [50, 0, 0],   [50, 0, 105],   [2, 2], atan2(200, -50) - 90);
//square_tube([40, 20, 2, 3], [0, 200, 0],  [0, 200, 105], [4, 4], atan2(200, -50) - 90);

//square_tube([20, 40, 2, 2], [20, 100, 0], [55, 100, 0], [0, 0], 0);
//square_tube([20, 40, 2, 2], [20,  50, 0], [55, 50, 0], [0, 0], 0);
