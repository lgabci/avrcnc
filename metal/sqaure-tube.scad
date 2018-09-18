/* square tube */

include <ends.scad>;

module _square_bar(w, h, l, r) {
  hull() {
    for (i = [[-1, -1], [-1, 1], [1, 1], [1, -1]]) {
      translate([i[0] * (w / 2 - r), i[1] * (h / 2 - r), 0]) {
        cylinder(h = l, r = r, center = true);
      }
    }
  }
}

module square_tube(l, dim = [20, 20, 2, 3], a = [[0, 0], [0, 0]], rot = [0, 0, 0], pos = [0, 0, 0]) {
  w = dim[0];
  h = dim[1];
  d = dim[2];
  r = dim[3];

  translate(pos) {
    rotate(rot) {
      difference() {
        _square_bar(w, h, l, r);
        _square_bar(w - 2 * d, h - 2 * d, l + 1, r - d);

        ends([l, w, h], a);
      }
    }
  }


}

square_tube(l = 100, a = [[0, 0], [45, 0]]);
