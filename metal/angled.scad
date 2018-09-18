include <ends.scad>;

module angled(l, dim = [20, 20, 2, 3], a = [[0, 0], [0, 0]], rot = [0, 0, 0], pos = [0, 0, 0]) {
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

l1 = 200;
l2 = 100;
w = 50;
h = 50;

angled(l1, [w, h, 15, 5], a = [[45, 45], [0, 0]], rot = [0, 0, 0], pos = [0, 0, 0]);

/*
square_tube(l1, [w, h, 2, 3], a = [[45, 0], [45, 0]], rot = [0, 0, 0], pos = [10, 20, 100]);
square_tube(l1, [w, h, 2, 3], a = [[45, 0], [45, 0]], rot = [0, 0, 180], pos = [l2 - w + 10, 20, 100]);

square_tube(l2, [w, h, 2, 3], a = [[45, 0], [45, 0]], rot = [0, 90, 0], pos = [(l2 - w) / 2 + 10, 20, l1 - w / 2]);
square_tube(l2, [w, h, 2, 3], a = [[45, 0], [45, 0]], rot = [0, -90, 0], pos = [(l2 - w) / 2 + 10, 20, w / 2]);

square_tube(l2 - 2 * w, [w, h, 2, 3], a = [[0, 0], [0, 0]], rot = [0, 90, 0], pos = [(l2 - w) / 2 + 10, 20, l1 / 2]);
*/
