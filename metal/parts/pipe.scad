/* AVR-CNC pipe */

include <ends.scad>;

module pipe(l = 100, dim = [20, 16], a = [[0, 0], [0, 0]], rot = [0, 0, 0], pos = [0, 0, 0]) {
  R = dim[0];
  r = dim[1];

  translate(pos) {
    rotate(rot) {
      difference() {
        cylinder(h = l, r = R, center = true);
        cylinder(h = l + 1, r = r, center = true);
        
        //ends(dim = [l, 2 * R, 2 * R], a = a);
      }
      translate([0, 0, tan(a[0][0]) * tan(a[0][1]) * R * sin(a[0][0]) * sin(a[0][1])]) {
        #ends(dim = [l, 2 * R, 2 * R], a = a);
      }
    }
  }

}

pipe(l = 200, a = [[45, 5], [0, 0]]);
