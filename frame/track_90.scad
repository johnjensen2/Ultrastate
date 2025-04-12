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
translate([-25,20,25])
rotate([90,0,0])
cylinder(h=40,d=PVC_OD+10);


translate([25,20,25])
rotate([90,0,0])
cylinder(h=40,d=PVC_OD+10);


translate([0,15,9])
cylinder(h=32,d=PVC_OD);



translate([0,-15,9])
cylinder(h=32,d=PVC_OD);


}

translate([0,0,-1])
cylinder(h=50,d=PVC_OD);


translate([-25,80,25])
rotate([90,0,0])
cylinder(h=160,d=PVC_OD);

translate([25,80,25])
rotate([90,0,0])
cylinder(h=160,d=PVC_OD);


//screw holes


translate([0,80,25])
rotate([90,0,0])
cylinder(h=160,d=5);

translate([0,45,25])
rotate([90,0,0])
cylinder(h=30,d=9);

translate([0,-15,25])
rotate([90,0,0])
cylinder(h=30,d=9);

translate([-50,0,25])
rotate([0,90,0])
cylinder(h=100,d=5);

translate([-50,0,25])
rotate([0,90,0])
cylinder(h=10,d=8);

translate([40,0,25])
rotate([0,90,0])
cylinder(h=10,d=8);
}



