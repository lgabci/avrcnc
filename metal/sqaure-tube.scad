
//$fn = 100;

module square_bar(dim) {
  w = dim[0];
  h = dim[1];
  l = dim[2];
  r = dim[3];

  hull() {
    for (i = [[-1, -1], [-1, 1], [1, 1], [1, -1]]) {
      translate([i[0] * (w / 2 - r), i[1] * (h / 2 - r), 0]) {
        cylinder(h=l, r=r, center=true);
      }
    }
  }
}

module square_tube(dim, l, angle, pos, rot) {
  w = dim[0];
  h = dim[1];
  d = dim[2];
  r = dim[3];
  
  translate(pos) {
    rotate(rot) {
      difference() {
        square_bar([w, h, l, r]);
        square_bar([w - 2 * d, h - 2 * d, l, r]);
      }
    }
  }

  translate([0, 0, l / 2 - w * sin(angle[0])]) {
    rotate([angle[0], 0, 0]) {
      color("red") {
        cube([w / cos(angle[0]), h, 1], center=true);
      }
    }
  }
}

square_tube([20, 40, 2, 2], 80, [60, 0], [0, 0, 0], [0, 0, 0]);

/*
linear_extrude(twist = 180) {
    hull() {
circle(r = 1);
translate([3, 0, 0]) {
  circle(r = 1);
}
}
}
*/
