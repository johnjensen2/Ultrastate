$fn=360;

/* Tank boat link starting at 22mm in ID
5 mm thick walls and M5 set screw
*/



PVC_OD=21.5;
lock_thickness=10;
Wall_thickness=5;
screw_diameter=4.8;

difference()
{
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2, $fn=6);
translate([0,0,-1])
cylinder(h=lock_thickness+2,d=PVC_OD);

rotate([0,0,30])
translate([-PVC_OD,0,screw_diameter])
rotate([0,90,0])
cylinder(h=
PVC_OD*2,d=screw_diameter);

}