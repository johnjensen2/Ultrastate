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
main_dia=175;
cover_screw_dia=3.2;

difference()
{
union()
{
cylinder(h=14,d=main_dia);

}
//cover holes
translate([15,15,-1])
cylinder(h=19,d=cover_screw_dia);

translate([15,-15,-1])
cylinder(h=19,d=cover_screw_dia);

translate([-15,-15,-1])
cylinder(h=19,d=cover_screw_dia);

translate([-15,15,-1])
cylinder(h=19,d=cover_screw_dia);

translate([0,0,-1])
cylinder(h=lock_thickness+2,d=PVC_OD);

for(a=[0:angles:360])
{
rotate([0,0,a])

hull()
{
translate([main_dia/2,0,-1])
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2+fudge);

translate([main_dia/2+10,0,-1])
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2+fudge+10);

}

}


}