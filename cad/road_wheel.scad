$fn=360;

/* Tank boat link starting at 22mm in ID
5 mm thick walls and M5 set screw
gear will use inner retainer as pivot point
*/


PVC_OD=22;
lock_thickness=25;
Wall_thickness=5;
screw_diameter=4.8;
angles=360/7;
fudge=5;
main_dia=100;
cover_screw_dia=3.2;

difference()
{
union()
{
cylinder(h=10,d=88);

cylinder(h=3,d=100);





}
//cover holes
translate([15,15,-1])
cylinder(h=29,d=cover_screw_dia);

translate([15,-15,-1])
cylinder(h=29,d=cover_screw_dia);

translate([-15,-15,-1])
cylinder(h=29,d=cover_screw_dia);

translate([-15,15,-1])
cylinder(h=29,d=cover_screw_dia);

translate([0,0,-1])
cylinder(h=lock_thickness+2,d=PVC_OD);



translate([0,0,-2])
cylinder(h=2,d=90);

translate([0,0,25])
cylinder(h=2,d=90);
}
