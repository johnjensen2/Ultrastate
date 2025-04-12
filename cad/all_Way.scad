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
cylinder(h=lock_thickness*2,d=PVC_OD+Wall_thickness*2);



translate([0,lock_thickness,lock_thickness])
rotate([90,0,0])
cylinder(h=lock_thickness*2,d=PVC_OD+Wall_thickness*2);



translate([-lock_thickness,0,lock_thickness])
rotate([90,0,90])
cylinder(h=lock_thickness*2,d=PVC_OD+Wall_thickness*2);
}

translate([0,0,-1])
cylinder(h=lock_thickness*2+2,d=PVC_OD);

translate([0,lock_thickness,lock_thickness])
rotate([90,0,0])
cylinder(h=lock_thickness*2+2,d=PVC_OD);


translate([-lock_thickness,0,lock_thickness])
rotate([90,0,90])
cylinder(h=lock_thickness*2+2,d=PVC_OD);
}



