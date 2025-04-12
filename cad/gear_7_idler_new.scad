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
fudge=2;


difference()
{
hull()
{
translate([-50,40,25])
rotate([90,0,0])
cylinder(h=80,d=PVC_OD+10);


translate([50,40,25])
rotate([90,0,0])
cylinder(h=80,d=PVC_OD+10);

translate([0,0,9.5])
cube([132,80,1], center=true);

}

translate([0,0,38])
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2+5);


translate([0,0,-1])
cylinder(h=50,d=PVC_OD);


translate([25,25,-1])
cylinder(h=50,d=PVC_OD);


translate([-25,25,-1])
cylinder(h=50,d=PVC_OD);


translate([25,-25,-1])
cylinder(h=50,d=PVC_OD);


translate([-25,-25,-1])
cylinder(h=50,d=PVC_OD);


translate([-50,41,25])
rotate([90,0,0])
cylinder(h=82,d=PVC_OD);


translate([50,41,25])
rotate([90,0,0])
cylinder(h=82,d=PVC_OD);
}