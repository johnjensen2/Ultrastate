$fn=360;

/* Tank boat link starting at 22mm in ID
5 mm thick walls and M5 set screw
gear will use inner retainer as pivot point
*/


PVC_OD=22;
lock_thickness=25;
Wall_thickness=5;
screw_diameter=4.8;
angles=360/10;
fudge=2;

difference()
{
union()
{
cylinder(h=10,d=150);

translate([0,0,10])
cylinder(h=10,d=PVC_OD+Wall_thickness*2);
}


translate([0,0,-1])
cylinder(h=lock_thickness+2,d=PVC_OD);

for(a=[0:angles:360])
{
rotate([0,0,a])

hull()
{
translate([75,0,-1])
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2+fudge);

translate([100,0,-1])
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2+fudge);

}

}

translate([-PVC_OD,0,15])
rotate([0,90,0])
cylinder(h=
PVC_OD*2,d=screw_diameter);

//counter_sinks
translate([PVC_OD-8,0,15])
rotate([0,90,0])
cylinder(h=
PVC_OD*2,d=10);

translate([-PVC_OD+8,0,15])
rotate([0,-90,0])
cylinder(h=
PVC_OD*2,d=10);

}