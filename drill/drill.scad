/* drill press */
$fn = 100;

holder(h = 20, r = 20, w = 2);

module nut(d1, d2, h) {
  r1 = d1 / 2;
  r2 = d2 / 2;
  ro = r1 / sin(60);

  difference() {
    cylinder(h = h, r = ro, center = true);
    cylinder(h = h, r = r2, center = true);
    for (i = [0:5]) {
      rotate(i * 60, 0, 0) {
        translate([0, ro / 2 + r1, 0]) {
          cube([ro, ro, h], center = true);
        }
      }
    }
  }
}

module holder(h, r, w) {
  gap = 2;
  rnut = 5;
  dnut = 4;

  difference() {
    cylinder(h = h, r = r + w, center = true);
    cylinder(h = h, r = r, center = true);
    translate([0, - gap / 2, - h / 2]) {
      cube([r + w, gap, h]);
    }
  }

  deg = asin((gap + dnut) / 2 / (r + w));
  for (i = [-1, 1]) {
    rotate([0, 0, i * deg]) {
      translate([r + w + rnut, 0, 0]) {
        rotate([90, 30, -i * deg]) {
          nut(d1 = 2 * rnut, d2 = 2 * rnut * 0.6, h = dnut);
        }
      }
    }
  }
}
