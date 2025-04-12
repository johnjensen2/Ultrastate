$fn=360;

/* Tank boat link starting at 22mm in ID
5 mm thick walls and M5 set screw
*/



PVC_OD=22;
lock_thickness=10;
Wall_thickness=5.1;
screw_diameter=4.8;

difference()
{

translate([-25,-30,-5])
cube([50,30,155]);

hull()
{
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2, $fn=6);

translate([0,5,0])
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2, $fn=6);
}

translate([0,0,135])

hull()
{
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2, $fn=6);

translate([0,5,0])
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2, $fn=6);
}


translate([0,0,-7])
cylinder(h=165,d=PVC_OD);

}