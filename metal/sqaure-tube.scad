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

module square_tube(dim, z, a, pos, rot) {
  x = dim[0];
  y = dim[1];
  d = dim[2];
  r = dim[3];

  q = 50;

  translate(pos) {
    rotate(rot) {
      difference() {
        square_bar([x, y, z, r]);
        square_bar([x - 2 * d, y - 2 * d, z + 1, r]);


  color("red") {
    translate([0, 0, (z - tan(a[0]) * y) / 2]) {
      rotate([a[0], 0, 0]) {
        translate([0, 0, q / 2]) {
          cube([4 * x, 3 * y, q], center=true);
        }
      }
    }
  }
  color("green") {
    translate([0, 0, (z - tan(a[1]) * x) / 2]) {
      rotate([0, a[1], 0]) {
        translate([0, 0, q / 2]) {
          cube([4 * x, 3 * y, q], center=true);
        }
      }
    }
  }

      }
    }
  }

q = 1;
  color("blue") {
    translate([0, 0, (z - tan(a[1]) * x - tan(a[0]) * y) / 2]) {
      rotate([a[0], a[1], 0]) {
        translate([0, 0, q / 2]) {
          cube([4 * x, 3 * y, q], center=true);
        }
      }
    }
  }


}

square_tube([20, 20, 2, 2], 80, [45, 45], [0, 0, 0], [0, 0, 0]);

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
