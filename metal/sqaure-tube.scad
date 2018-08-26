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

module square_tube(dim, pos1, pos2, a) {
  w = dim[0];
  h = dim[1];
  d = dim[2];
  r = dim[3];

  x = pos2[0] - pos1[0];
  y = pos2[1] - pos1[1];
  z = pos2[2] - pos1[2];
  l = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

  rot = [acos(z / l), 0, atan2(y, x) + 90];
  //rot = [0, 0, 0];

  translate(pos1) {
    rotate(rot) {
      difference() {
        _square_bar(w, h, l, r);
        _square_bar(w - 2 * d, h - 2 * d, l + 1, r);
      }
      q = 0.1;
      translate([0, 0, (l - tan(a[0]) * h) / 2]) {
        rotate([a[0], 0, a[1]]) {
          translate([0, 0, q / 2]) {
            color("lightgreen") {
              cube([100, 100, q], center=true);
            }
          }
        }
      }
    }
  }


}


square_tube([20, 40, 2, 2], [0, 0, 0], [0, 0, 200], [45, 0]);

translate([50, 100, 25]) {
  color("red") {
    sphere(r=1);
  }
}
translate([-50, -100, -25]) {
  color("blue") {
    sphere(r=1);
  }
}
