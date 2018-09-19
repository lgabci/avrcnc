/* AVR-CNC angled iron */

include <ends.scad>;

module angled(l = 100, dim = [50, 50, 5, 3], a = [[0, 0], [0, 0]], rot = [0, 0, 0], pos = [0, 0, 0]) {
  w = dim[0];
  h = dim[1];
  d = dim[2];
  r = dim[3];

  translate(pos) {
    rotate(rot) {
      difference() {
        union() {
          translate([0, -(h - d) / 2, 0]) {
            cube([w, d, l], center = true);
          }
          translate([-(w - d) / 2, 0, 0]) {
            cube([d, h, l], center = true);
          }
          if (r > 0) {
            translate([-w / 2 + d + r / 2, -h / 2 + d + r / 2, 0]) {
              cube([r, r, l], center = true);
            }
          }
        }
        translate([-w / 2 + d + r, -h / 2 + d + r, 0]) {
          cylinder(h = l + 1, r = r, center = true);
        }
        
        ends([l, w, h], a);
      }
    }
  }

}
