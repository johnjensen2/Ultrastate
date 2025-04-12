$fn=360;

pvc_od=22;
wall=5;
clamp_lenght=50;
pvc2=pvc_od+wall;
plate_height=5;
screw_hole_dia=5;

difference()
{
hull()
{
translate([0,0,0])
cylinder(h=clamp_lenght,d=pvc_od+wall);

translate([-pvc2/2,pvc2/2,0])
cube([pvc2,5,clamp_lenght]);
}

translate([0,0,-1])
cylinder(h=clamp_lenght+2,d=pvc_od);

//screw holes

translate([0,0,10])
rotate([-90,0,0])
cylinder(h=20+plate_height,d=screw_hole_dia);

translate([0,0,clamp_lenght-10])
rotate([-90,0,0])
cylinder(h=20+plate_height,d=screw_hole_dia);

translate([0,0,clamp_lenght/2])
rotate([-90,0,0])
cylinder(h=20+plate_height,d=screw_hole_dia);




translate([-pvc2/2-1,0,10])
rotate([0,90,0])
cylinder(h=pvc2+2,d=screw_hole_dia);

translate([-pvc2/2-1,0,clamp_lenght-10])
rotate([0,90,0])
cylinder(h=pvc2+2,d=screw_hole_dia);

translate([-pvc2/2-1,0,clamp_lenght/2])
rotate([0,90,0])
cylinder(h=pvc2+2,d=screw_hole_dia);

}